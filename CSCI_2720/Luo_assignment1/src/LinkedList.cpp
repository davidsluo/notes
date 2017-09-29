#include <iostream>
#include "LinkedList.h"

using namespace std;

LinkedList::LinkedList() {
    this->length = 0;
    this->head = nullptr;
    this->current = nullptr;
}

LinkedList::~LinkedList() {
    this->makeEmpty();
}

int LinkedList::lengthIs() const {
    return this->length;
}

void LinkedList::retrieveItem(ItemType &item, bool &found) {
    NodeType *cursor = this->head;
    NodeType *prev = nullptr;
    do {
        if (this->head == nullptr) {
            found = false;
            return;
        }
        if (cursor->info.compareTo(item) == EQUAL) {
            // I have no idea why this is necessary,
            // but it says to do this in the post-condition.
            item = cursor->info;
            found = true;
            return;
        }
        prev = cursor;
        cursor = cursor->next;
    } while (cursor != nullptr && prev != nullptr);

    found = false;
}

void LinkedList::insertItem(ItemType &item) {
    if (this->length == 0) {
        this->head = new NodeType(item, nullptr);
        this->length++;
        return;
    } else {
        NodeType *cursor = this->head;
        NodeType *prev = nullptr;

        while (true) {

            RelationType relationship = cursor->info.compareTo(item);

            if (relationship == GREATER) {
                NodeType *newNode = new NodeType(item, cursor);
                if (prev != nullptr)
                    prev->next = newNode;
                else 
                    this->head = newNode;
                this->length++;
                return;
            } else if (relationship == EQUAL) {
                // This wasn't stated in the project sheet, 
                // but it was on Piazza.
                cout << "Item already in list." << endl;
                return;
            } else if (cursor->next == nullptr) {
                cursor->next = new NodeType(item, nullptr);
                this->length++;
                return;
            }

            prev = cursor;
            cursor = cursor->next;
        }
    }
}

void LinkedList::deleteItem(ItemType &item) {
    NodeType *cursor = this->head;
    NodeType *prev = nullptr;
    do {
        if (this->head == nullptr) {
            std::cout << "Item not in list." << std::endl;
            return;
        }
        if (cursor->info.compareTo(item) == EQUAL) {
            if (prev != nullptr)
                prev->next = cursor->next;
            else 
                this->head = cursor->next;

            delete cursor;
            this->length--;
            return;
        }
        prev = cursor;
        cursor = cursor->next;
    } while (cursor != nullptr && prev != nullptr);
    
    std::cout << "Item not in list." << std::endl;
}

void LinkedList::resetList() {
    this->current = nullptr;
}

void LinkedList::getNextItem(ItemType &item) {
    this->getNextNode();
    if (this->current != nullptr)
        item = this->current->info;
}

void LinkedList::makeEmpty() {
    while (this->head != nullptr) {
        NodeType *next = this->head->next;
        delete this->head;
        this->head = next;
    }
    this->current = nullptr;
    this->head = nullptr;
    this->length = 0;
}

void LinkedList::print() {
    if (this->length == 0) {
        std::cout << "Empty list." << std::endl;
        return;
    }

    NodeType *cursor = this->head;

    while (cursor != nullptr) {
        std::cout << cursor->info.getValue();
        std::cout << " ";
        if (cursor != nullptr)
            cursor = cursor->next;
    }

    std::cout << std::endl;
}

void LinkedList::getNextNode() {
    if (this->current != nullptr)
        this->current = this->current->next;
    else
        this->current = this->head;
}
