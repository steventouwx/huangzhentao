package com.example.binderipc.systemservice;

import com.example.binderipc.CalculatorCore;
import com.example.binderipc.minibinder.MiniBinder;
import com.example.binderipc.minibinder.MiniIBinder;
import com.example.binderipc.minibinder.MiniParcel;
import com.example.binderipc.miniservicemanager.MiniServiceManager;

/**
 * JVM self-test that simulates "ServiceManager add/get + transact" flow.
 */
public final class SystemServiceManagerStyleSelfTest {
    private static final String SERVICE_NAME = "demo.system_echo";
    private static final int TRANSACTION_ECHO = 1;

    private SystemServiceManagerStyleSelfTest() {
    }

    public static void main(String[] args) {
        MiniServiceManager.addService(SERVICE_NAME, new EchoMiniBinderService());

        MiniIBinder binder = MiniServiceManager.getService(SERVICE_NAME);
        assertTrue(binder != null, "registered binder should be found");

        MiniParcel data = MiniParcel.obtain();
        MiniParcel reply = MiniParcel.obtain();
        data.writeString("  Alice ");
        boolean ok = binder.transact(TRANSACTION_ECHO, data, reply);
        assertTrue(ok, "transact should succeed");
        assertEquals(
                "SystemService> Hello, Alice! [serverPid=9999]",
                reply.readString(),
                "echo result"
        );

        MiniIBinder missing = MiniServiceManager.getService("missing.service");
        assertTrue(missing == null, "missing service should return null");

        System.out.println("ALL_TESTS_PASSED");
    }

    private static final class EchoMiniBinderService extends MiniBinder {
        @Override
        protected boolean onTransact(int code, MiniParcel data, MiniParcel reply) {
            if (code != TRANSACTION_ECHO) {
                return false;
            }
            String rawName = data.readString();
            String greeting = CalculatorCore.buildGreeting(rawName, 9999);
            reply.writeString("SystemService> " + greeting);
            return true;
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
