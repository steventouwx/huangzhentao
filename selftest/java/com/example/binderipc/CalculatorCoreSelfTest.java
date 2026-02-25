package com.example.binderipc;

public final class CalculatorCoreSelfTest {

    private CalculatorCoreSelfTest() {
    }

    public static void main(String[] args) {
        assertEquals(9, CalculatorCore.safeAdd(4, 5), "normal add");
        assertEquals(-1, CalculatorCore.safeAdd(-3, 2), "negative add");
        assertEquals("Guest", CalculatorCore.sanitizeName(null), "null fallback");
        assertEquals("Guest", CalculatorCore.sanitizeName("   "), "blank fallback");
        assertEquals("Alice", CalculatorCore.sanitizeName("  Alice "), "trim name");
        assertEquals(
                "Hello, Alice! [serverPid=12345]",
                CalculatorCore.buildGreeting(" Alice ", 12345),
                "build greeting"
        );

        boolean overflowHappened = false;
        try {
            CalculatorCore.safeAdd(Integer.MAX_VALUE, 1);
        } catch (ArithmeticException expected) {
            overflowHappened = true;
        }
        assertTrue(overflowHappened, "overflow should throw");

        System.out.println("ALL_TESTS_PASSED");
    }

    private static void assertEquals(int expected, int actual, String caseName) {
        if (expected != actual) {
            throw new AssertionError(caseName + " failed, expected=" + expected + ", actual=" + actual);
        }
    }

    private static void assertEquals(String expected, String actual, String caseName) {
        if (!expected.equals(actual)) {
            throw new AssertionError(caseName + " failed, expected=" + expected + ", actual=" + actual);
        }
    }

    private static void assertTrue(boolean condition, String caseName) {
        if (!condition) {
            throw new AssertionError(caseName + " failed");
        }
    }
}
