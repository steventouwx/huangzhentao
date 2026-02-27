package com.example.binderipc.systemservice;

import android.os.IBinder;
import java.lang.reflect.Method;

/**
 * Reflection wrapper for android.os.ServiceManager (@hide).
 *
 * <p>NOTE:
 * <ul>
 *   <li>This API is hidden for normal applications.</li>
 *   <li>Registering a service usually requires system/privileged identity.</li>
 *   <li>Use this class only in system app / platform scenarios.</li>
 * </ul>
 */
public final class ServiceManagerCompat {
    private static final String SERVICE_MANAGER_CLASS = "android.os.ServiceManager";
    private static volatile Method addServiceMethod;
    private static volatile Method getServiceMethod;

    private ServiceManagerCompat() {
    }

    public static void addService(String serviceName, IBinder serviceBinder) {
        if (serviceName == null || serviceName.trim().isEmpty()) {
            throw new IllegalArgumentException("serviceName is blank");
        }
        if (serviceBinder == null) {
            throw new IllegalArgumentException("serviceBinder is null");
        }
        try {
            Method method = getAddServiceMethod();
            method.invoke(null, serviceName, serviceBinder);
        } catch (ReflectiveOperationException e) {
            throw new IllegalStateException(
                    "Failed to call ServiceManager.addService. "
                            + "Requires system/privileged context.",
                    e
            );
        }
    }

    public static IBinder getService(String serviceName) {
        if (serviceName == null || serviceName.trim().isEmpty()) {
            throw new IllegalArgumentException("serviceName is blank");
        }
        try {
            Method method = getGetServiceMethod();
            Object result = method.invoke(null, serviceName);
            return (IBinder) result;
        } catch (ReflectiveOperationException e) {
            throw new IllegalStateException(
                    "Failed to call ServiceManager.getService. "
                            + "Requires system/privileged context.",
                    e
            );
        }
    }

    private static Method getAddServiceMethod() throws ReflectiveOperationException {
        Method local = addServiceMethod;
        if (local != null) {
            return local;
        }
        synchronized (ServiceManagerCompat.class) {
            if (addServiceMethod == null) {
                Class<?> clazz = Class.forName(SERVICE_MANAGER_CLASS);
                addServiceMethod = clazz.getDeclaredMethod("addService", String.class, IBinder.class);
            }
            return addServiceMethod;
        }
    }

    private static Method getGetServiceMethod() throws ReflectiveOperationException {
        Method local = getServiceMethod;
        if (local != null) {
            return local;
        }
        synchronized (ServiceManagerCompat.class) {
            if (getServiceMethod == null) {
                Class<?> clazz = Class.forName(SERVICE_MANAGER_CLASS);
                getServiceMethod = clazz.getDeclaredMethod("getService", String.class);
            }
            return getServiceMethod;
        }
    }
}
