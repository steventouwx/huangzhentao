package com.example.binderipc.minibinder;

public final class MiniBinderProxy implements MiniIBinder {
    private final MiniIBinder remote;

    public MiniBinderProxy(MiniIBinder remote) {
        this.remote = remote;
    }

    @Override
    public MiniIInterface queryLocalInterface(String descriptor) {
        return null;
    }

    @Override
    public boolean transact(int code, MiniParcel data, MiniParcel reply) {
        return remote.transact(code, data, reply);
    }
}

