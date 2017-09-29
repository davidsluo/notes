#ifndef ITEMTYPE_H
#define ITEMTYPE_H

enum RelationType {
    GREATER, LESS, EQUAL
};

class ItemType {
    public:
        ItemType();
        RelationType compareTo(ItemType &item);
        void print();
        void initialize(int number);
        int getValue() const;
    private:
        int value;
};
#endif
