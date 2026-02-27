import unittest
import xml.etree.ElementTree as ET
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
MANIFEST_PATH = ROOT / "app/src/main/AndroidManifest.xml"
AIDL_PATH = ROOT / "app/src/main/aidl/com/example/binderipc/IRemoteCalculator.aidl"
MAIN_ACTIVITY_PATH = ROOT / "app/src/main/java/com/example/binderipc/MainActivity.java"
SERVICE_MANAGER_COMPAT_PATH = ROOT / "app/src/main/java/com/example/binderipc/systemservice/ServiceManagerCompat.java"
SYSTEM_SM_EXAMPLE_PATH = ROOT / "app/src/main/java/com/example/binderipc/systemservice/SystemServiceManagerExample.java"
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
            "private static final String SERVICE_MANAGER_CLASS = \"android.os.ServiceManager\"",
            "Class.forName(SERVICE_MANAGER_CLASS)",
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


if __name__ == "__main__":
    unittest.main()
