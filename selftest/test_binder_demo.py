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
SERVICE_MANAGER_COMPAT_PATH = ROOT / "app/src/main/java/com/example/binderipc/systemservice/ServiceManagerCompat.java"
SYSTEM_SM_EXAMPLE_PATH = ROOT / "app/src/main/java/com/example/binderipc/systemservice/SystemServiceManagerExample.java"
JAVA_SELFTEST_PATH = ROOT / "selftest/java/com/example/binderipc/CalculatorCoreSelfTest.java"
JAVA_SM_SELFTEST_PATH = ROOT / "selftest/java/com/example/binderipc/ServiceManagerMiniSelfTest.java"
JAVA_SYSTEM_SM_STYLE_SELFTEST_PATH = ROOT / "selftest/java/com/example/binderipc/systemservice/SystemServiceManagerStyleSelfTest.java"
JAVA_MINI_BINDER_DIR = ROOT / "selftest/java/com/example/binderipc/minibinder"
JAVA_MINI_AIDL_DIR = ROOT / "selftest/java/com/example/binderipc/miniaidl"
JAVA_MINI_SM_DIR = ROOT / "selftest/java/com/example/binderipc/miniservicemanager"
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

    def test_system_service_manager_example_contract(self):
        self.assertTrue(SERVICE_MANAGER_COMPAT_PATH.exists(), f"ServiceManagerCompat not found: {SERVICE_MANAGER_COMPAT_PATH}")
        self.assertTrue(SYSTEM_SM_EXAMPLE_PATH.exists(), f"SystemServiceManagerExample not found: {SYSTEM_SM_EXAMPLE_PATH}")

        compat_content = SERVICE_MANAGER_COMPAT_PATH.read_text(encoding="utf-8")
        example_content = SYSTEM_SM_EXAMPLE_PATH.read_text(encoding="utf-8")

        compat_required = [
            'Class.forName("android.os.ServiceManager")',
            'getDeclaredMethod("addService", String.class, IBinder.class)',
            'getDeclaredMethod("getService", String.class)',
            "public static void addService(String serviceName, IBinder serviceBinder)",
            "public static IBinder getService(String serviceName)",
        ]
        for snippet in compat_required:
            self.assertIn(snippet, compat_content, f"ServiceManagerCompat missing key snippet: {snippet}")

        example_required = [
            "ServiceManagerCompat.addService(SERVICE_NAME, new EchoBinderService())",
            "ServiceManagerCompat.getService(SERVICE_NAME)",
            "remote.transact(TRANSACTION_ECHO, data, reply, 0)",
            'public static final String SERVICE_NAME = "demo.system_echo"',
        ]
        for snippet in example_required:
            self.assertIn(snippet, example_content, f"SystemServiceManagerExample missing key snippet: {snippet}")

    def test_java_core_logic_runner(self):
        javac_cmd = shutil.which("javac")
        java_cmd = shutil.which("java")
        self.assertIsNotNone(javac_cmd, "javac not found in environment")
        self.assertIsNotNone(java_cmd, "java not found in environment")

        self.assertTrue(CALCULATOR_CORE_PATH.exists(), f"CalculatorCore not found: {CALCULATOR_CORE_PATH}")
        self.assertTrue(JAVA_SELFTEST_PATH.exists(), f"Java self-test class not found: {JAVA_SELFTEST_PATH}")
        self.assertTrue(JAVA_SM_SELFTEST_PATH.exists(), f"Java ServiceManager self-test class not found: {JAVA_SM_SELFTEST_PATH}")
        self.assertTrue(
            JAVA_SYSTEM_SM_STYLE_SELFTEST_PATH.exists(),
            f"Java SystemServiceManager style self-test class not found: {JAVA_SYSTEM_SM_STYLE_SELFTEST_PATH}",
        )
        self.assertTrue(JAVA_MINI_BINDER_DIR.exists(), f"MiniBinder dir not found: {JAVA_MINI_BINDER_DIR}")
        self.assertTrue(JAVA_MINI_AIDL_DIR.exists(), f"MiniAIDL dir not found: {JAVA_MINI_AIDL_DIR}")
        self.assertTrue(JAVA_MINI_SM_DIR.exists(), f"MiniServiceManager dir not found: {JAVA_MINI_SM_DIR}")

        classes_dir = ROOT / "build/selftest-classes"
        if classes_dir.exists():
            shutil.rmtree(classes_dir)
        classes_dir.mkdir(parents=True, exist_ok=True)

        mini_sources = (
            list(JAVA_MINI_BINDER_DIR.rglob("*.java"))
            + list(JAVA_MINI_AIDL_DIR.rglob("*.java"))
            + list(JAVA_MINI_SM_DIR.rglob("*.java"))
        )
        self.assertTrue(mini_sources, "Mini binder sources not found")

        compile_proc = subprocess.run(
            [
                javac_cmd,
                "-d",
                str(classes_dir),
                str(CALCULATOR_CORE_PATH),
                str(JAVA_SELFTEST_PATH),
                str(JAVA_SM_SELFTEST_PATH),
                str(JAVA_SYSTEM_SM_STYLE_SELFTEST_PATH),
                *[str(p) for p in mini_sources],
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

        run_proc_sm = subprocess.run(
            [
                java_cmd,
                "-cp",
                str(classes_dir),
                "com.example.binderipc.ServiceManagerMiniSelfTest",
            ],
            check=False,
            capture_output=True,
            text=True,
        )
        self.assertEqual(
            0,
            run_proc_sm.returncode,
            f"Java ServiceManager self-test failed.\nstdout:\n{run_proc_sm.stdout}\nstderr:\n{run_proc_sm.stderr}",
        )
        self.assertIn("ALL_TESTS_PASSED", run_proc_sm.stdout)

        run_proc_system_sm_style = subprocess.run(
            [
                java_cmd,
                "-cp",
                str(classes_dir),
                "com.example.binderipc.systemservice.SystemServiceManagerStyleSelfTest",
            ],
            check=False,
            capture_output=True,
            text=True,
        )
        self.assertEqual(
            0,
            run_proc_system_sm_style.returncode,
            "Java SystemServiceManager style self-test failed.\n"
            f"stdout:\n{run_proc_system_sm_style.stdout}\n"
            f"stderr:\n{run_proc_system_sm_style.stderr}",
        )
        self.assertIn("ALL_TESTS_PASSED", run_proc_system_sm_style.stdout)


if __name__ == "__main__":
    unittest.main()
