#include "ItemType.h"
#include <iostream>

/**
 * Constructor
 */
ItemType::ItemType() {
}

/**
 * Compares `this` ItemType to `item` ItemType.
 *
 * @param item The ItemType to compare to.
 * @return The relationship between `this` and `item`.
 */
// It's logically impossible for this to return anything other than as defined
// in the RelationType enum, so this warning is suppressed.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type"
RelationType ItemType::compareTo(ItemType &item) {
    if (this->getValue() == item.getValue()) return EQUAL;
    if (this->getValue() < item.getValue()) return LESS;
    if (this->getValue() > item.getValue()) return GREATER;
}
#pragma GCC diagnostic pop

/**
 * Prints the current value of this ItemType to stdout.
 */
void ItemType::print() {
    std::cout << this->getValue() << std::endl;    
}

/**
 * Initializes this ItemType to `number`.
 *
 * @param number The value to set to.
 */
void ItemType::initialize(int number) {
    this->value = number; 
}

/**
 * Get the current value of this ItemType.
 *
 * @return The value.
 */
int ItemType::getValue() const {
    return this->value;
}
