package com.example.binderipc.minibinder;

import java.util.ArrayList;
import java.util.List;

public final class MiniParcel {
    private final List<Object> data = new ArrayList<>();
    private int cursor = 0;

    public static MiniParcel obtain() {
        return new MiniParcel();
    }

    public void writeInt(int value) {
        data.add(Integer.valueOf(value));
    }

    public int readInt() {
        Object v = data.get(cursor++);
        return ((Integer) v).intValue();
    }

    public void writeString(String value) {
        data.add(value);
    }

    public String readString() {
        Object v = data.get(cursor++);
        return (String) v;
    }
}

