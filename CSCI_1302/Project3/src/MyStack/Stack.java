package MyStack;

import MyStack.Exceptions.EmptyStackException;
import MyStack.Exceptions.FullStackException;

/**
 * {@inheritDoc}
 */
public class Stack<T> implements StackADT<T> {

    public static final int DEFAULT_CAPACITY = 100;
    private T[] stack;
    private int top = -1;

    /**
     * Constructor. Creates a stack of the given size.
     *
     * @param capacity The capacity of the stack.
     */
    @SuppressWarnings("unchecked")
    public Stack(int capacity) {
        stack = (T[]) new Object[capacity];
    }

    /**
     * Default Constructor. Creates a stack of size 100.
     */
    @SuppressWarnings("unchecked")
    public Stack() {
        stack = (T[]) new Object[DEFAULT_CAPACITY];
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void push(T element) throws FullStackException {
        try {
            stack[top + 1] = element;
            top++;
        } catch (ArrayIndexOutOfBoundsException e) {
            throw new FullStackException("MyStack.Stack full");
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public T pop() throws EmptyStackException {
        T popped;
        try {
            popped = stack[top];
            stack[top] = null;
            top--;
            return popped;

        } catch (ArrayIndexOutOfBoundsException e) {
            throw new EmptyStackException("MyStack.Stack empty");
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public T peek() throws EmptyStackException {
        try {
            return stack[top];
        } catch (ArrayIndexOutOfBoundsException e) {
            throw new EmptyStackException("MyStack.Stack empty");
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isEmpty() {
        return size() == 0;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public boolean isFull() {
        return size() == maxSize();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int size() {
//        int count = 0;
//        for (T t : stack) {
//            if (t != null) {
//                count++;
//            }
//        }
//        return count;

        return top + 1;
    }

    /**
     * Return the maximum number of elements that can be stored in this stack.
     *
     * @return the maximum number of elements that can be stored in this stack.
     */
    public int maxSize() {
        return stack.length;
    }

    /**
     * Creates a string representation of the stack.
     *
     * @return a string representation of the stack
     */
    @Override
    public String toString() {
        StringBuilder builder = new StringBuilder();
        for (T obj : stack) {
            if (obj != null) {
                builder.append(obj.toString());
                builder.append("\n");
            }
        }
        return builder.toString();
    }
}
