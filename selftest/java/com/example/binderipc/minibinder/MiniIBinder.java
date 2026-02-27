package com.example.binderipc.minibinder;

public interface MiniIBinder {
    MiniIInterface queryLocalInterface(String descriptor);

    boolean transact(int code, MiniParcel data, MiniParcel reply);
}

