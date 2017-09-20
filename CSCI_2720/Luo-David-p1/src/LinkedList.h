struct NodeType {
    ItemType info;
    NodeType *next;
}

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
}
