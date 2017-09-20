#include "LinkedList.h"

LinkedList::LinkedList() {
    this.length = 0;
    this.head = nullptr;
    this.current = nullptr;
}

LinkedList::~LinkedList() {
    this.resetList();

    if (this.head != nullptr) {
        do {
            delete &this.current;
        } while ((this.current = this.getNextNode()) != nullptr); 
    }
}

int LinkedList::lengthIs() const {
    return this.length;
}

void LinkedList::retrieveItem(ItemType &item, bool &found) {
    
}

