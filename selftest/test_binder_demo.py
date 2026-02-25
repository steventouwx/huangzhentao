import shutil
import subprocess
import unittest
import xml.etree.ElementTree as ET
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
MANIFEST_PATH = ROOT / "app/src/main/AndroidManifest.xml"
AIDL_PATH = ROOT / "app/src/main/aidl/com/example/binderipc/IRemoteCalculator.aidl"
MAIN_ACTIVITY_PATH = ROOT / "app/src/main/java/com/example/binderipc/MainActivity.java"
CALCULATOR_CORE_PATH = ROOT / "app/src/main/java/com/example/binderipc/CalculatorCore.java"
JAVA_SELFTEST_PATH = ROOT / "selftest/java/com/example/binderipc/CalculatorCoreSelfTest.java"
ANDROID_NS = "{http://schemas.android.com/apk/res/android}"


class BinderDemoSelfTest(unittest.TestCase):
    def test_manifest_has_remote_process_service(self):
        self.assertTrue(MANIFEST_PATH.exists(), f"Manifest not found: {MANIFEST_PATH}")

        tree = ET.parse(MANIFEST_PATH)
        root = tree.getroot()
        app_node = root.find("application")
        self.assertIsNotNone(app_node, "Manifest missing application node")

        service_nodes = app_node.findall("service")
        self.assertTrue(service_nodes, "Manifest has no service declarations")

        target = None
        for service in service_nodes:
            if service.get(ANDROID_NS + "name") == ".RemoteCalculatorService":
                target = service
                break
        self.assertIsNotNone(target, "Missing .RemoteCalculatorService declaration")
        self.assertEqual(":remote", target.get(ANDROID_NS + "process"), "Service must use :remote process")
        self.assertEqual("false", target.get(ANDROID_NS + "exported"), "Service should not be exported")

    def test_aidl_contract_contains_expected_methods(self):
        self.assertTrue(AIDL_PATH.exists(), f"AIDL file not found: {AIDL_PATH}")
        content = AIDL_PATH.read_text(encoding="utf-8")

        self.assertIn("interface IRemoteCalculator", content)
        self.assertIn("int add(int left, int right);", content)
        self.assertIn("String greet(String name);", content)

    def test_main_activity_contains_bind_and_remote_call(self):
        self.assertTrue(MAIN_ACTIVITY_PATH.exists(), f"MainActivity not found: {MAIN_ACTIVITY_PATH}")
        content = MAIN_ACTIVITY_PATH.read_text(encoding="utf-8")

        required_snippets = [
            "bindService(intent, connection, Context.BIND_AUTO_CREATE)",
            "IRemoteCalculator.Stub.asInterface(service)",
            "remoteCalculator.greet(name)",
            "remoteCalculator.add(7, 35)",
        ]
        for snippet in required_snippets:
            self.assertIn(snippet, content, f"MainActivity missing key snippet: {snippet}")

    def test_java_core_logic_runner(self):
        javac_cmd = shutil.which("javac")
        java_cmd = shutil.which("java")
        self.assertIsNotNone(javac_cmd, "javac not found in environment")
        self.assertIsNotNone(java_cmd, "java not found in environment")

        self.assertTrue(CALCULATOR_CORE_PATH.exists(), f"CalculatorCore not found: {CALCULATOR_CORE_PATH}")
        self.assertTrue(JAVA_SELFTEST_PATH.exists(), f"Java self-test class not found: {JAVA_SELFTEST_PATH}")

        classes_dir = ROOT / "build/selftest-classes"
        if classes_dir.exists():
            shutil.rmtree(classes_dir)
        classes_dir.mkdir(parents=True, exist_ok=True)

        compile_proc = subprocess.run(
            [
                javac_cmd,
                "-d",
                str(classes_dir),
                str(CALCULATOR_CORE_PATH),
                str(JAVA_SELFTEST_PATH),
            ],
            check=False,
            capture_output=True,
            text=True,
        )
        self.assertEqual(
            0,
            compile_proc.returncode,
            f"javac compile failed.\nstdout:\n{compile_proc.stdout}\nstderr:\n{compile_proc.stderr}",
        )

        run_proc = subprocess.run(
            [
                java_cmd,
                "-cp",
                str(classes_dir),
                "com.example.binderipc.CalculatorCoreSelfTest",
            ],
            check=False,
            capture_output=True,
            text=True,
        )
        self.assertEqual(
            0,
            run_proc.returncode,
            f"Java self-test failed.\nstdout:\n{run_proc.stdout}\nstderr:\n{run_proc.stderr}",
        )
        self.assertIn("ALL_TESTS_PASSED", run_proc.stdout)


if __name__ == "__main__":
    unittest.main()
