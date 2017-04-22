package LinkedList;

import java.io.*;
import java.util.Iterator;

/**
 * An implementation of a Sorted Doubly Linked List
 *
 * @param <T> The type of data this List stores
 * @author David Luo
 */
public class SortedList<T extends Comparable> implements Serializable, Iterable<T> {
    private int size = 0;
    private Node<T> head = null;

    /**
     * Sorts and adds a Comparable object to this list
     *
     * @param obj the object to add
     * @return if adding the object was successful
     */
    public boolean add(T obj) {
        Node<T> cursor = head;
        Node<T> node = new Node<>(obj);

        if (obj == null)
            return false;

        // Loop through list until it finds a node with an element that is greater than <obj>
        while (cursor != null) {
            int comparison = cursor.getData().compareTo(obj);
            if (comparison == 0)
                return false;
            else if (comparison > 0) {

                node.setNext(cursor);

                if (cursor.hasPrev()) {
                    node.setPrev(cursor.prev());
                    cursor.prev().setNext(node);
                } else {
                    head = node;
                }

                cursor.setPrev(node);
                size++;
                return true;

            }
//            else if (comparison < 0) {
//                // do nothing?
//            }

            if (cursor.hasNext()) {
                cursor = cursor.next();
            } else
                break;
        }

        if (cursor != null) {
            cursor.setNext(node);
            node.setPrev(cursor);
        } else {
            head = node;
        }
        size++;
        return true;

    }

    /**
     * Remove an object from the list
     *
     * @param obj The object to remove
     * @return If removing the object was successful
     */
    public boolean remove(T obj) {
        Node<T> cursor = head;
        if (cursor == null)
            return false;
        while (cursor != null) {
            if (cursor.getData().equals(obj)) {
                if (cursor.hasPrev()) {
                    cursor.prev().setNext(cursor.next());
                } else {
                    head = cursor.next();
                }
                if (cursor.hasNext()) {
                    cursor.next().setPrev(cursor.prev());
                }
                size--;
                return true;
            }

            cursor = cursor.next();
        }

        return false;

    }

    /**
     * Get the object at the specified index in the list
     *
     * @param index The index of the object to get
     * @return The object
     */
    public T get(int index) {
        Node<T> cursor = head;
        if (index < 0 || index >= size())
            throw new IndexOutOfBoundsException();
        else {
            for (int i = 0; i < index; i++) {
                cursor = cursor.next();
            }
            return cursor.getData();
        }
    }

    /**
     * @return If this list is empty
     */
    public boolean isEmpty() {
        return head == null;
    }

    /**
     * @return How many elements are in the list
     */
    public int size() {
        return size;
    }

    /**
     * Print the toString for each element in the list
     */
    public void printList() {
        for (T obj : this) {
            System.out.println(obj.toString());
        }
    }

    public SortedList<T> merge(SortedList<? extends T> otherList) {
        SortedList merged = this;
        for (Object obj : otherList) {
            merged.add((Comparable) obj);
        }

        return merged;
    }

    /**
     * Is `this` list a prefix of `otherList`?
     *
     * @param otherList the list that `this` list might be a prefix of
     * @return if this list is a prefix of otherList
     */
    public boolean isPrefix(SortedList otherList) {
        if (this.size() > otherList.size())
            return false;
        for (int i = 0; i < this.size(); i++) {
            if (this.get(i).compareTo(otherList.get(i)) != 0) {
                return false;
            }
        }
        return true;
    }

    /**
     * Get the index of the specified object
     *
     * @param obj The object to search for
     * @return The index the object is in the list or -1 if not in the list
     */
    public int indexOf(T obj) {
        int index = -1;
        for (T element : this) {
            if (element.equals(obj)) {
                return ++index;
            } else
                index++;
        }
        return -1;
    }

    /**
     * @return An iterator for this list
     */
    @Override
    public Iterator<T> iterator() {
        return new Iterator<T>() {

            private Node<T> cursor = null;

            @Override
            public boolean hasNext() {
                if (cursor == null)
                    return head != null && head.next() != null;
                else
                    return cursor.next() != null;
            }

            @Override
            public T next() {
                if (cursor == null) {
                    cursor = head;
                    return cursor.getData();
                } else {
                    cursor = cursor.next();
                    return cursor.getData();
                }
            }
        };
    }

    /**
     * Serializes the given SortedList to a file named "serialized"
     *
     * @param list The SortedList to serialize.
     */
    public static void serialze(SortedList list) {
        try (FileOutputStream fileOut = new FileOutputStream("serialized");
             ObjectOutputStream objOut = new ObjectOutputStream(fileOut)) {

            objOut.writeObject(list);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * Deserializes the file named "serialized" into a SortedList
     *
     * @return The deserialized SortedList
     */
    public static SortedList deserialize() {
        SortedList list = null;

        try (FileInputStream fileIn = new FileInputStream("serialized");
             ObjectInputStream objIn = new ObjectInputStream(fileIn)) {

            list = (SortedList) objIn.readObject();

        } catch (IOException | ClassNotFoundException e) {
            e.printStackTrace();
        }

        return list;
    }
}
