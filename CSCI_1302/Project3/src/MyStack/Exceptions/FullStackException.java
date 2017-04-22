package MyStack.Exceptions;

/**
 * @author David Luo
 *         Thrown when attempting to push to a full stack.
 */
public class FullStackException extends RuntimeException {
    public FullStackException(String s) {
        super(s);
    }
}
