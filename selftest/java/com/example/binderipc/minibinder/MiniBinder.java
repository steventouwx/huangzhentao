package com.example.binderipc.minibinder;

public class MiniBinder implements MiniIBinder {
    private MiniIInterface owner;
    private String descriptor;

    protected void attachInterface(MiniIInterface owner, String descriptor) {
        this.owner = owner;
        this.descriptor = descriptor;
    }

    @Override
    public MiniIInterface queryLocalInterface(String descriptor) {
        if (this.descriptor != null && this.descriptor.equals(descriptor)) {
            return owner;
        }
        return null;
    }

    @Override
    public boolean transact(int code, MiniParcel data, MiniParcel reply) {
        return onTransact(code, data, reply);
    }

    protected boolean onTransact(int code, MiniParcel data, MiniParcel reply) {
        return false;
    }
}

