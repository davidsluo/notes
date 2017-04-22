package MyStack.Exceptions;

/**
 * @author David Luo
 *         Thrown when attempting to pop from an empty stack.
 */
public class EmptyStackException extends RuntimeException {
    public EmptyStackException(String s) {
        super(s);
    }
}
