package com.example.binderipc.miniservicemanager;

import com.example.binderipc.CalculatorCore;
import com.example.binderipc.miniaidl.IRemoteCalculatorMini;

public final class RemoteCalculatorMiniSystemService extends IRemoteCalculatorMini.Stub {
    private final int serverPid;

    public RemoteCalculatorMiniSystemService(int serverPid) {
        this.serverPid = serverPid;
    }

    @Override
    public int add(int left, int right) {
        return CalculatorCore.safeAdd(left, right);
    }

    @Override
    public String greet(String name) {
        return CalculatorCore.buildGreeting(name, serverPid);
    }
}

