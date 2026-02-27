package com.example.binderipc.miniservicemanager;

import com.example.binderipc.minibinder.MiniBinderProxy;
import com.example.binderipc.minibinder.MiniIBinder;
import java.util.HashMap;
import java.util.Map;

public final class MiniServiceManager {
    private static final Map<String, MiniIBinder> REGISTRY = new HashMap<>();

    private MiniServiceManager() {
    }

    public static void addService(String name, MiniIBinder binder) {
        if (name == null || name.trim().isEmpty()) {
            throw new IllegalArgumentException("service name is blank");
        }
        if (binder == null) {
            throw new IllegalArgumentException("binder is null");
        }
        REGISTRY.put(name, binder);
    }

    public static MiniIBinder getService(String name) {
        return REGISTRY.get(name);
    }

    public static MiniIBinder getServiceRemote(String name) {
        MiniIBinder local = REGISTRY.get(name);
        if (local == null) {
            return null;
        }
        return new MiniBinderProxy(local);
    }
}

