package Test;

import MyStack.Exceptions.EmptyStackException;
import MyStack.Exceptions.FullStackException;
import MyStack.IntegerStackEmulator;
import MyStack.Stack;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.JUnitCore;

import static org.junit.Assert.*;

/**
 * @author David Luo
 *         Tests the MyStack.Stack class.
 */
public class MyStackTester {

    private Stack<String> stack;

    public static void main(String[] args) {
        if (args.length == 1 && args[0].equalsIgnoreCase("test")) {
            JUnitCore.main("Test.MyStackTester");
        } else {
            IntegerStackEmulator emulator = new IntegerStackEmulator();
        }
    }

    @Before
    public void makeStack() {
        stack = new Stack<>();
    }

    @Test
    public void testDefaultConstructor() {
        Stack<String> stack = new Stack<>();
        assertEquals(Stack.DEFAULT_CAPACITY, stack.maxSize());
    }

    @Test
    public void testConstructor() {
        Stack<String> stack = new Stack<>(3);
        assertEquals(3, stack.maxSize());
    }

    @Test
    public void testIsEmpty() {
        Stack<String> stack = new Stack<>(1);
        assertTrue(stack.isEmpty());
        stack.push("1");
        assertFalse(stack.isEmpty());
    }

    @Test
    public void testIsFull() {
        Stack<String> stack = new Stack<>(1);
        assertFalse(stack.isFull());
        stack.push("1");
        assertTrue(stack.isFull());
    }

    @Test
    public void testSize() {
        for (int i = 0; i < 10; i++) {
            stack.push(String.valueOf(i));

            assertEquals(i + 1, stack.size());
        }
    }

    @Test
    public void testMaxSize() {
        assertEquals(Stack.DEFAULT_CAPACITY, stack.maxSize());
    }

    @Test
    public void testPeek() {
        stack.push("asdf");
        assertEquals("asdf", stack.peek());
        assertEquals(1, stack.size());
    }

    @Test
    public void testPush() {
        stack.push("a");
        assertEquals("a", stack.peek());
        stack.push("b");
        assertEquals("b", stack.peek());
        stack.push("c");
        assertEquals("c", stack.peek());
        stack.push("d");
        assertEquals("d", stack.peek());
        stack.push("e");
        assertEquals("e", stack.peek());
    }

    @Test
    public void testPop() {
        stack.push("a");
        stack.push("b");
        stack.push("c");
        stack.push("d");
        stack.push("e");
        assertEquals(5, stack.size());
        assertEquals("e", stack.pop());
        assertEquals(4, stack.size());
        assertEquals("d", stack.pop());
        assertEquals(3, stack.size());
        assertEquals("c", stack.pop());
        assertEquals(2, stack.size());
        assertEquals("b", stack.pop());
        assertEquals(1, stack.size());
        assertEquals("a", stack.pop());
        assertEquals(0, stack.size());
    }

    @Test(expected = FullStackException.class)
    public void testFullStackException() {
        stack = new Stack<>(0);
        stack.push("a");
    }

    @Test(expected = EmptyStackException.class)
    public void testEmptyStackException() {
        stack.pop();
    }

    @Test
    public void testToString() {
        stack.push("a");
        stack.push("b");
        stack.push("c");
        assertEquals("a\nb\nc\n", stack.toString());
    }
}
