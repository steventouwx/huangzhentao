package com.example.binderipc;

/**
 * Pure business logic without Android dependencies.
 */
public final class CalculatorCore {
    private static final String DEFAULT_NAME = "Guest";

    private CalculatorCore() {
    }

    public static int safeAdd(int left, int right) {
        long result = (long) left + right;
        if (result > Integer.MAX_VALUE || result < Integer.MIN_VALUE) {
            throw new ArithmeticException("Integer overflow: " + left + " + " + right);
        }
        return (int) result;
    }

    public static String sanitizeName(String rawName) {
        if (rawName == null) {
            return DEFAULT_NAME;
        }
        String trimmed = rawName.trim();
        return trimmed.isEmpty() ? DEFAULT_NAME : trimmed;
    }

    public static String buildGreeting(String rawName, int serverPid) {
        return "Hello, " + sanitizeName(rawName) + "! [serverPid=" + serverPid + "]";
    }
}
