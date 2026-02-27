package com.example.binderipc.miniaidl;

import com.example.binderipc.minibinder.MiniBinder;
import com.example.binderipc.minibinder.MiniIBinder;
import com.example.binderipc.minibinder.MiniIInterface;
import com.example.binderipc.minibinder.MiniParcel;

public interface IRemoteCalculatorMini extends MiniIInterface {
    int add(int left, int right);

    String greet(String name);

    abstract class Stub extends MiniBinder implements IRemoteCalculatorMini {
        public static final String DESCRIPTOR = "com.example.binderipc.miniaidl.IRemoteCalculatorMini";
        private static final int TRANSACTION_add = 1;
        private static final int TRANSACTION_greet = 2;

        public Stub() {
            attachInterface(this, DESCRIPTOR);
        }

        public static IRemoteCalculatorMini asInterface(MiniIBinder obj) {
            if (obj == null) {
                return null;
            }
            MiniIInterface iin = obj.queryLocalInterface(DESCRIPTOR);
            if (iin != null && iin instanceof IRemoteCalculatorMini) {
                return (IRemoteCalculatorMini) iin;
            }
            return new Proxy(obj);
        }

        @Override
        public MiniIBinder asBinder() {
            return this;
        }

        @Override
        protected boolean onTransact(int code, MiniParcel data, MiniParcel reply) {
            switch (code) {
                case TRANSACTION_add: {
                    int left = data.readInt();
                    int right = data.readInt();
                    int result = add(left, right);
                    reply.writeInt(result);
                    return true;
                }
                case TRANSACTION_greet: {
                    String name = data.readString();
                    String result = greet(name);
                    reply.writeString(result);
                    return true;
                }
                default:
                    return false;
            }
        }

        private static final class Proxy implements IRemoteCalculatorMini {
            private final MiniIBinder remote;

            private Proxy(MiniIBinder remote) {
                this.remote = remote;
            }

            @Override
            public MiniIBinder asBinder() {
                return remote;
            }

            @Override
            public int add(int left, int right) {
                MiniParcel data = MiniParcel.obtain();
                MiniParcel reply = MiniParcel.obtain();
                data.writeInt(left);
                data.writeInt(right);
                boolean ok = remote.transact(TRANSACTION_add, data, reply);
                if (!ok) {
                    throw new IllegalStateException("transact failed for add");
                }
                return reply.readInt();
            }

            @Override
            public String greet(String name) {
                MiniParcel data = MiniParcel.obtain();
                MiniParcel reply = MiniParcel.obtain();
                data.writeString(name);
                boolean ok = remote.transact(TRANSACTION_greet, data, reply);
                if (!ok) {
                    throw new IllegalStateException("transact failed for greet");
                }
                return reply.readString();
            }
        }
    }
}

