#include <iostream>
#include "LinkedList.h"

using namespace std;

/**
 * Constructor
 */
LinkedList::LinkedList() {
    this->length = 0;
    this->head = nullptr;
    this->current = nullptr;
}

/**
 * Destructor
 */
LinkedList::~LinkedList() {
    this->makeEmpty();
}

/**
 * Get length of the list.
 */
int LinkedList::lengthIs() const {
    return this->length;
}

/**
 * Get if `item` is in the list.
 *
 * @param item The item to search for
 * @param found The variable to store the result of teh search.
 */
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

/**
 * Insert an item into the list.
 *
 * @param item The item to insert.
 */
void LinkedList::insertItem(ItemType &item) {
    if (this->length == 0) {
        this->head = new NodeType(item, nullptr);
        this->length++;
        return;
    } else {
        // Decouples this method from the user's iterator.
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

/**
 * Delete an item from the list.
 *
 * @param item The item to remove.
 */
void LinkedList::deleteItem(ItemType &item) {
    // Decouples this method from the user's iterator.
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

/**
 * Reset the user's iterator.
 */
void LinkedList::resetList() {
    this->current = nullptr;
}

/**
 * Get the next item in the list.
 *
 * @param item The variable to store the item in.
 */
void LinkedList::getNextItem(ItemType &item) {
    this->getNextNode();
    if (this->current != nullptr)
        item = this->current->info;
}

/**
 * Remove all items from the list.
 */
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

/**
 * Print the contents of this list, space delimited, to stdout.
 */
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

/**
 * Get the next node in the list.
 */
void LinkedList::getNextNode() {
    if (this->current != nullptr)
        this->current = this->current->next;
    else
        this->current = this->head;
}
