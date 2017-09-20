#include "ItemType.h"
#include <iostream>

using namespace std;

/**
 * Constructor
 */
ItemType::ItemType() {
    // ??? i don't know what to do here.
}

/**
 * Compares `this` ItemType to `item` ItemType.
 *
 * @param item The ItemType to compare to.
 * @return The relationship between `this` and `item`.
 */
RelationType ItemType::compareTo(ItemType &item) {
    if (this.getValue() == item.getValue()) return RelationType.EQUAL;
    if (this.getValue() < item.getValue()) return RelationType.LESS;
    if (this.getValue() > item.getValue()) return RelationType.GREATER;

    return NULL;
}

/**
 * Prints the current value of this ItemType to stdout.
 */
void ItemType::print() {
    std::cout << this.getValue() << std::endl;    
}

/**
 * Initializes this ItemType to `number`.
 *
 * @param number The value to set to.
 */
void ItemType::initialize(int number) {
    this.value = number; 
}

/**
 * Get the current value of this ItemType.
 *
 * @return The value.
 */
int ItemType::getValue() const {
    return this.value();
}
