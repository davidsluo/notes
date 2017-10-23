#include <iostream>
#include "DoublyLinkedList.h"

using namespace std;

/**
 * Constructor
 */
DoublyLinkedList::DoublyLinkedList() {
    this->length = 0;
    this->head = nullptr;
    this->current = nullptr;
}

/**
 * Destructor
 */
DoublyLinkedList::~DoublyLinkedList() {
    this->makeEmpty();
}

/**
 * Get length of the list.
 */
int DoublyLinkedList::lengthIs() const {
    return this->length;
}

/**
 * Get if `item` is in the list.
 *
 * @param item The item to search for
 * @param found The variable to store the result of teh search.
 */
void DoublyLinkedList::retrieveItem(ItemType &item, bool &found) {
    NodeType<ItemType> *cursor = this->head;
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
        cursor = cursor->next;
    } while (cursor != nullptr && cursor->prev != nullptr);

    found = false;
}

/**
 * Insert an item into the list.
 *
 * @param item The item to insert.
 */
void DoublyLinkedList::insertItem(ItemType &item) {
    if (this->length == 0) {
        this->head = new NodeType<ItemType>(item, nullptr, nullptr);
        this->length++;
        return;
    } else {
        // Decouples this method from the user's iterator.
        NodeType<ItemType> *cursor = this->head;

        while (true) {

            RelationType relationship = cursor->info.compareTo(item);

            if (relationship == GREATER) {
                NodeType<ItemType> *newNode = new NodeType<ItemType>(item, cursor, cursor->prev);
                if (cursor->prev != nullptr)
                    cursor->prev->next = newNode;
                else
                    this->head = newNode;
                cursor->prev = newNode;

                this->length++;
                return;
            } else if (relationship == EQUAL) {
                // This wasn't stated in the project sheet, 
                // but it was on Piazza.
                cout << "Item already in list." << endl;
                return;
            } else if (cursor->next == nullptr) {
                cursor->next = new NodeType<ItemType>(item, nullptr, cursor);
                this->length++;
                return;
            }

            cursor = cursor->next;
        }
    }
}

/**
 * Delete an item from the list.
 *
 * @param item The item to remove.
 */
void DoublyLinkedList::deleteItem(ItemType &item) {
    // Decouples this method from the user's iterator.
    NodeType<ItemType> *cursor = this->head;
    do {
        if (this->head == nullptr) {
            std::cout << "Item not in list." << std::endl;
            return;
        }
        if (cursor->info.compareTo(item) == EQUAL) {
            if (cursor->prev != nullptr)
                cursor->prev->next = cursor->next;
            else 
                this->head = cursor->next;

            if (cursor->next != nullptr)
                cursor->next->prev = cursor->prev;

            delete cursor;
            this->length--;
            return;
        }
        cursor = cursor->next;
    } while (cursor != nullptr && cursor->prev != nullptr);
    
    std::cout << "Item not in list." << std::endl;
}

/**
 * Reset the user's iterator.
 */
void DoublyLinkedList::resetList() {
    this->current = nullptr;
}

/**
 * Get the next item in the list.
 *
 * @param item The variable to store the item in.
 */
void DoublyLinkedList::getNextItem(ItemType &item) {
    this->getNextNode();
    if (this->current != nullptr)
        item = this->current->info;
}

/**
 * Remove all items from the list.
 */
void DoublyLinkedList::makeEmpty() {
    while (this->head != nullptr) {
        NodeType<ItemType> *next = this->head->next;
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
void DoublyLinkedList::print() {
    if (this->length == 0) {
        std::cout << "Empty list." << std::endl;
        return;
    }

    NodeType<ItemType> *cursor = this->head;

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
void DoublyLinkedList::getNextNode() {
    if (this->current != nullptr)
        this->current = this->current->next;
    else
        this->current = this->head;
}
