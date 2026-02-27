package com.example.binderipc.systemservice;

import android.os.Binder;
import android.os.IBinder;
import android.os.Parcel;
import android.os.Process;
import android.os.RemoteException;
import com.example.binderipc.CalculatorCore;

/**
 * Example of system-style Binder service registration/get with ServiceManager.
 *
 * <p>Usage scope: system app / platform process only. For normal app development,
 * prefer bindService with AIDL.
 */
public final class SystemServiceManagerExample {
    public static final String SERVICE_NAME = "demo.system_echo";
    private static final int TRANSACTION_ECHO = IBinder.FIRST_CALL_TRANSACTION;

    private SystemServiceManagerExample() {
    }

    /**
     * Register a Binder instance into Android system ServiceManager.
     */
    public static void registerSystemEchoService() {
        ServiceManagerCompat.addService(SERVICE_NAME, new EchoBinderService());
    }

    /**
     * Fetch the Binder from ServiceManager and perform a transact call.
     */
    public static String getGreetingFromSystemService(String rawName) {
        IBinder remote = ServiceManagerCompat.getService(SERVICE_NAME);
        if (remote == null) {
            throw new IllegalStateException("service not found: " + SERVICE_NAME);
        }

        Parcel data = Parcel.obtain();
        Parcel reply = Parcel.obtain();
        try {
            data.writeString(rawName);
            boolean ok = remote.transact(TRANSACTION_ECHO, data, reply, 0);
            if (!ok) {
                throw new IllegalStateException("transact failed for TRANSACTION_ECHO");
            }
            return reply.readString();
        } catch (RemoteException e) {
            throw new IllegalStateException("binder transact failed", e);
        } finally {
            reply.recycle();
            data.recycle();
        }
    }

    private static final class EchoBinderService extends Binder {
        @Override
        protected boolean onTransact(int code, Parcel data, Parcel reply, int flags) throws RemoteException {
            if (code == TRANSACTION_ECHO) {
                String rawName = data.readString();
                String greeting = CalculatorCore.buildGreeting(rawName, Process.myPid());
                reply.writeString("SystemService> " + greeting);
                return true;
            }
            return super.onTransact(code, data, reply, flags);
        }
    }
}
