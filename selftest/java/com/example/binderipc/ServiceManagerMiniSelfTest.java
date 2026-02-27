package com.example.binderipc;

import com.example.binderipc.miniaidl.IRemoteCalculatorMini;
import com.example.binderipc.minibinder.MiniBinder;
import com.example.binderipc.minibinder.MiniBinderProxy;
import com.example.binderipc.minibinder.MiniIBinder;
import com.example.binderipc.miniservicemanager.MiniServiceManager;
import com.example.binderipc.miniservicemanager.RemoteCalculatorMiniSystemService;

public final class ServiceManagerMiniSelfTest {
    private ServiceManagerMiniSelfTest() {
    }

    public static void main(String[] args) {
        RemoteCalculatorMiniSystemService service = new RemoteCalculatorMiniSystemService(4242);
        MiniServiceManager.addService("remote_calculator", service.asBinder());

        // Local path: binder object "本体"（可 queryLocalInterface）
        MiniIBinder localBinder = MiniServiceManager.getService("remote_calculator");
        assertTrue(localBinder instanceof MiniBinder, "local binder should be MiniBinder");
        assertTrue(localBinder.queryLocalInterface(IRemoteCalculatorMini.Stub.DESCRIPTOR) != null, "local queryLocalInterface should be non-null");

        IRemoteCalculatorMini localApi = IRemoteCalculatorMini.Stub.asInterface(localBinder);
        assertTrue(localApi == service, "local asInterface should return same service instance");
        assertEquals(42, localApi.add(7, 35), "local add");
        assertEquals("Hello, Alice! [serverPid=4242]", localApi.greet(" Alice "), "local greet");

        // Remote path: binder proxy（queryLocalInterface 恒为 null，必须走 Proxy.transact）
        MiniIBinder remoteBinder = MiniServiceManager.getServiceRemote("remote_calculator");
        assertTrue(remoteBinder instanceof MiniBinderProxy, "remote binder should be MiniBinderProxy");
        assertTrue(remoteBinder.queryLocalInterface(IRemoteCalculatorMini.Stub.DESCRIPTOR) == null, "remote queryLocalInterface should be null");

        IRemoteCalculatorMini remoteApi = IRemoteCalculatorMini.Stub.asInterface(remoteBinder);
        assertTrue(remoteApi != service, "remote asInterface should NOT return service instance");
        assertEquals(42, remoteApi.add(7, 35), "remote add");
        assertEquals("Hello, Guest! [serverPid=4242]", remoteApi.greet("   "), "remote greet blank->Guest");

        boolean overflowHappened = false;
        try {
            remoteApi.add(Integer.MAX_VALUE, 1);
        } catch (ArithmeticException expected) {
            overflowHappened = true;
        }
        assertTrue(overflowHappened, "overflow should throw");

        System.out.println("ALL_TESTS_PASSED");
    }

    private static void assertEquals(int expected, int actual, String caseName) {
        if (expected != actual) {
            throw new AssertionError(caseName + " failed, expected=" + expected + ", actual=" + actual);
        }
    }

    private static void assertEquals(String expected, String actual, String caseName) {
        if (!expected.equals(actual)) {
            throw new AssertionError(caseName + " failed, expected=" + expected + ", actual=" + actual);
        }
    }

    private static void assertTrue(boolean condition, String caseName) {
        if (!condition) {
            throw new AssertionError(caseName + " failed");
        }
    }
}

