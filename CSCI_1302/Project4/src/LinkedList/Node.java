package LinkedList;

import java.io.Serializable;

/**
 * A node in a Doubly Linked List
 *
 * @param <T> The type of data this Node stores
 * @author David Luo
 */
public class Node<T> implements Serializable {
    private T data;
    private Node<T> prev = null;
    private Node<T> next = null;

    /**
     * Constructor
     *
     * @param data The data to be stored in this node
     */
    public Node(T data) {
        this.data = data;
    }

    /**
     * @return The data stored in this node
     */
    public T getData() {
        return data;
    }

    /**
     * @param data The data to be stored in this node
     */
    public void setData(T data) {
        this.data = data;
    }

    /**
     * @return The previous node
     */
    public Node<T> prev() {
        return prev;
    }

    /**
     * @param prev The previous node
     */
    public void setPrev(Node<T> prev) {
        this.prev = prev;
    }

    /**
     * @return The next node
     */
    public Node<T> next() {
        return next;
    }

    /**
     * @param next The next node
     */
    public void setNext(Node<T> next) {
        this.next = next;
    }

    /**
     * @return If this node has a next node
     */
    public boolean hasNext() {
        return this.next != null;
    }

    /**
     * @return If this node has a previous node
     */
    public boolean hasPrev() {
        return this.prev != null;
    }
}
