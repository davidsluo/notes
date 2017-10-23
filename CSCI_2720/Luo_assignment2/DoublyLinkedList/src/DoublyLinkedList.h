#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include "ItemType.h"

template <class T>
struct NodeType {
    T info;
    NodeType<T> *next;
    NodeType<T> *prev;
    
    // For dynamic allocation.
    NodeType(T info, NodeType<T> *next, NodeType<T> *prev) {
        this->info = info;
        this->next = next;
        this->prev = prev;
    }
};

class DoublyLinkedList {
    public:
        DoublyLinkedList();
        ~DoublyLinkedList();
        int lengthIs() const;
        void retrieveItem(ItemType &item, bool &found);
        void insertItem(ItemType &item);
        void deleteItem(ItemType &item);
        void resetList();
        void getNextItem(ItemType &item);
        void makeEmpty();
        void print();
    private:
        int length;
        NodeType<ItemType> *head;
        NodeType<ItemType> *current;
        NodeType<ItemType> * findNode(ItemType &item);
        void getNextNode();
};

#endif
