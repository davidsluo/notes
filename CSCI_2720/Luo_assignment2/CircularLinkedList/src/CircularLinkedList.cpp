#include <iostream>
#include "CircularLinkedList.h"

using namespace std;

/**
 * Constructor
 */
CircularLinkedList::CircularLinkedList() {
    this->length = 0;
    this->head = nullptr;
    this->current = nullptr;
}

/**
 * Destructor
 */
CircularLinkedList::~CircularLinkedList() {
    NodeType<ItemType> *tail = this->head;
    while (tail->next != this->head)
        tail = tail->next;

    tail->next = nullptr;

    while (this->head != nullptr) {
        NodeType<ItemType> *next = this->head->next;
        delete this->head;
        this->head = next;
    }
}

void CircularLinkedList::findItem(NodeType<ItemType>* listData, ItemType item, NodeType<ItemType>* &location, NodeType<ItemType>* &predLoc, bool &found) {
    found = false;
    if (this->length == 0)
        return;
    
    NodeType<ItemType> *cursor = this->head;
    NodeType<ItemType> *prev = this->head;

    while (prev->next != this->head)
        prev = prev->next;

    while (cursor->next != this->head) {
        if (cursor->info.compareTo(item) == EQUAL) {
            found = true;
            location = cursor;
            predLoc = prev;
            return;
        }
    }

    if (!found) {
        location = cursor->next;
        predLoc = prev;
    }
    
}

/**
 * Get length of the list.
 */
int CircularLinkedList::lengthIs() const {
    return this->length;
}

/**
 * Insert an item into the list.
 *
 * @param item The item to insert.
 */
void CircularLinkedList::insertItem(ItemType &item) {
    if (this->length == 0) {
        this->head = new NodeType<ItemType>(item, nullptr);
        this->head->next = this->head;
        this->length++;
        return;
    } else {
        // Decouples this method from the user's iterator.
        NodeType<ItemType> *cursor = this->head;
        NodeType<ItemType> *prev = this->head;

        while (prev->next != this->head)
            prev = prev->next;

        while (true) {

            RelationType relationship = cursor->info.compareTo(item);

            if (relationship == GREATER) {
                NodeType<ItemType> *newNode = new NodeType<ItemType>(item, cursor);
                prev->next = newNode;
                if (cursor == head)
                    this->head = newNode;
                this->length++;
                return;
            } else if (relationship == EQUAL) {
                // This wasn't stated in the project sheet, 
                // but it was on Piazza.
                cout << "Item already in list." << endl;
                return;
            } else {
                if (cursor->next == this->head) {
                    cursor->next = new NodeType<ItemType>(item, this->head);
                    this->length++;
                    return;
                }
            }

            prev = cursor;
            cursor = cursor->next;
        }
    }
}

/**
 * Delete an item from the list.
 *
 * @param item The item to remove.
 */
void CircularLinkedList::deleteItem(ItemType &item) {
    // Decouples this method from the user's iterator.
    NodeType<ItemType> *cursor = this->head;
    NodeType<ItemType> *prev = nullptr;
    do {
        if (this->head == nullptr) {
            std::cout << "Item not in list." << std::endl;
            return;
        } else if (cursor->info.compareTo(item) == EQUAL) {
            if (prev != nullptr)
                prev->next = cursor->next;
            else { 
                NodeType<ItemType> *tail = cursor;
                while (tail->next != this->head)
                    tail = tail->next;
                tail->next = cursor->next;
            }

            delete cursor;
            this->length--;
            return;
        }
        prev = cursor;
        cursor = cursor->next;
    } while (cursor != nullptr && prev != nullptr);

    std::cout << "Item not in list." << std::endl;
}

/**
 * Print the contents of this list, space delimited, to stdout.
 */
void CircularLinkedList::print() {
    if (this->length == 0) {
        std::cout << "Empty list." << std::endl;
        return;
    }

    NodeType<ItemType> *cursor = this->head;

    while (cursor != nullptr) {
        std::cout << cursor->info.getValue();
        std::cout << " ";
        if (cursor->next == this->head)
            break;
        if (cursor != nullptr)
            cursor = cursor->next;
    }

    std::cout << std::endl;
}
