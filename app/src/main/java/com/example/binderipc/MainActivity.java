package com.example.binderipc;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.os.Process;
import android.os.RemoteException;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends Activity {
    private EditText nameInput;
    private TextView resultView;
    private IRemoteCalculator remoteCalculator;
    private boolean isBound;

    private final ServiceConnection connection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            remoteCalculator = IRemoteCalculator.Stub.asInterface(service);
            isBound = true;
            resultView.setText("Service bound. clientPid=" + Process.myPid());
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            remoteCalculator = null;
            isBound = false;
            resultView.setText("Service disconnected.");
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        nameInput = findViewById(R.id.inputName);
        resultView = findViewById(R.id.textResult);
        Button bindButton = findViewById(R.id.buttonBind);
        Button unbindButton = findViewById(R.id.buttonUnbind);
        Button callButton = findViewById(R.id.buttonCall);

        bindButton.setOnClickListener(this::bindRemoteService);
        unbindButton.setOnClickListener(v -> unbindRemoteService());
        callButton.setOnClickListener(v -> callRemoteService());
    }

    private void bindRemoteService(View ignored) {
        if (isBound) {
            resultView.setText("Service already bound.");
            return;
        }
        Intent intent = new Intent(this, RemoteCalculatorService.class);
        boolean bindResult = bindService(intent, connection, Context.BIND_AUTO_CREATE);
        resultView.setText(bindResult ? "Binding remote service..." : "Bind failed. Check config.");
    }

    private void unbindRemoteService() {
        if (!isBound) {
            resultView.setText("Service is not bound.");
            return;
        }
        unbindService(connection);
        isBound = false;
        remoteCalculator = null;
        resultView.setText("Service unbound.");
    }

    private void callRemoteService() {
        if (!isBound || remoteCalculator == null) {
            resultView.setText("Bind service first.");
            return;
        }
        String name = nameInput.getText().toString();
        try {
            String greetMsg = remoteCalculator.greet(name);
            int addResult = remoteCalculator.add(7, 35);
            resultView.setText(
                    "clientPid=" + Process.myPid() + "\n"
                            + "greet(name) => " + greetMsg + "\n"
                            + "add(7, 35) => " + addResult
            );
        } catch (RemoteException e) {
            resultView.setText("Remote call failed: " + e.getMessage());
        } catch (ArithmeticException e) {
            resultView.setText("Calculation failed: " + e.getMessage());
        }
    }

    @Override
    protected void onDestroy() {
        if (isBound) {
            unbindService(connection);
        }
        super.onDestroy();
    }
}
