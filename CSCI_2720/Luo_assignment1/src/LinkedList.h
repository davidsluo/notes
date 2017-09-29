#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "ItemType.h"

struct NodeType {
    ItemType info;
    NodeType *next;
    
    // For dynamic allocation.
    NodeType(ItemType info, NodeType *next) {
        this->info = info;
        this->next = next;
    }
};

class LinkedList {
    public:
        LinkedList();
        ~LinkedList();
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
        NodeType *head;
        NodeType *current;
        NodeType * findNode(ItemType &item);
        void getNextNode();
};

#endif
