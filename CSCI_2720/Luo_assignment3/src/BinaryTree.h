#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "ItemType.h"

struct NodeType {
    ItemType info;
    NodeType *left;
    NodeType *right;

    // For dynamic allocation.
    NodeType(ItemType info, NodeType *left, NodeType *right) {
        this->info = info;
        this->left = left;
        this->right = right;
    }

    NodeType(ItemType info) : NodeType(info, nullptr, nullptr) {
    }
};

class BinaryTree {
    public:
        BinaryTree();
        ~BinaryTree();
        void insert(ItemType &key);
        void deleteItem(ItemType &key);
        void retrieve(ItemType &key, bool &found) const;
        void preOrder() const;
        void inOrder() const;
        void postOrder() const;
        int getLength() const;
    private:
        int length;
        NodeType *root;
        void insert(ItemType &key, NodeType *&root);
        void deleteItem(ItemType &key, NodeType *&root);
        void retrieve(ItemType &key, bool &found, NodeType *root) const;
        void preOrder(NodeType *root) const;
        void inOrder(NodeType *root) const;
        void postOrder(NodeType *root) const;
};

#endif
