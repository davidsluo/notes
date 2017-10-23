#ifndef CIRCULARLINKEDLIST_H
#define CIRCULARLINKEDLIST_H

#include "ItemType.h"

template <class T>
struct NodeType {
    T info;
    NodeType *next;
    
    // For dynamic allocation.
    NodeType(T info, NodeType<T> *next) {
        this->info = info;
        this->next = next;
    }
};

class CircularLinkedList {
    public:
        CircularLinkedList();
        ~CircularLinkedList();
        void findItem(NodeType<ItemType>* listData, ItemType item, NodeType<ItemType>* &location, NodeType<ItemType>* &predLoc, bool &found);
        void insertItem(ItemType &item);
        void deleteItem(ItemType &item);
        int lengthIs() const;
        void print();
    private:
        int length;
        NodeType<ItemType> *head;
        NodeType<ItemType> *current;
};

#endif
