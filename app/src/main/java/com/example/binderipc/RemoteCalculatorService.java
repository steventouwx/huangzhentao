package com.example.binderipc;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.Process;

public class RemoteCalculatorService extends Service {

    private final IRemoteCalculator.Stub binder = new IRemoteCalculator.Stub() {
        @Override
        public int add(int left, int right) {
            return CalculatorCore.safeAdd(left, right);
        }

        @Override
        public String greet(String name) {
            return CalculatorCore.buildGreeting(name, Process.myPid());
        }
    };

    @Override
    public IBinder onBind(Intent intent) {
        return binder;
    }
}
