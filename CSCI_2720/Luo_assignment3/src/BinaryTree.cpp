#include <iostream>
#include "BinaryTree.h"

using namespace std;

/**
 * Constructor
 */
BinaryTree::BinaryTree() {
    this->length = 0;
    this->root = nullptr;
}

/**
 * Destructor
 */
BinaryTree::~BinaryTree() {
    destruct(this->root);
}

void BinaryTree::destruct(NodeType *root) {
    if (root != nullptr) {
        destruct(root->left);
        destruct(root->right);
        delete root;
    } 
}

/**
 * Insert an item into the tree.
 */
void BinaryTree::insert(ItemType &key) {
    this->insert(key, this->root);
}

/**
 * Private recursive insert method.
 */
void BinaryTree::insert(ItemType &key, NodeType *&root) {
    if (root == nullptr) {
        root = new NodeType(key);
        this->length++;
    } else {
        switch (key.compareTo(root->info)) {
            case LESS:
                if (root->left == nullptr) {
                    root->left = new NodeType(key);
                    this->length++;
                    return;
                }
                this->insert(key, root->left);
                return; 
            case GREATER:
                if (root->right == nullptr) {
                    root->right = new NodeType(key);
                    this->length++;
                    return;
                }
                this->insert(key, root->right);
                return; 
            case EQUAL:
                return;
        }
    }
}

/**
 * Delete an item from the tree.
 */
void BinaryTree::deleteItem(ItemType &key) {
    return deleteItem(key, this->root);
}

/**
 * Private recursive delete method.
 */
void BinaryTree::deleteItem(ItemType &key, NodeType *&root) {
    if (root == nullptr)
        return;
    
    switch(key.compareTo(root->info)) {
        case LESS:
            deleteItem(key, root->left);
            return;
        case GREATER:
            deleteItem(key, root->right);
            return;
        case EQUAL:
            NodeType *temp = root;
            if (root->left == nullptr) {
                root = root->right;
                delete temp;
                this->length--;
            } else if (root->right == nullptr) {
                root = root->left;
                delete temp;
                this->length--;
            } else {
                while (temp->right != nullptr)
                    temp = temp->right;
                ItemType newKey = temp->info;
                root->info = newKey;
                deleteItem(newKey, root->left);
                this->length--;
            }
    }
}

/**
 * Retrieve an item from the tree.
 */
void BinaryTree::retrieve(ItemType &key, bool &found) const {
    retrieve(key, found, this->root);
}

/**
 * Private recursive retrieve method.
 */
void BinaryTree::retrieve(ItemType &key, bool &found, NodeType *root) const {
    found = false;
    if (root == nullptr) {
        found = false;
    } else {
        switch(key.compareTo(root->info)) {
            case LESS:
                retrieve(key, found, root->left);
                return;
            case GREATER:
                retrieve(key, found, root->right);
                return;
            case EQUAL:
                found = true;
                return;
        }
    }
}

/**
 * Print the tree in pre-order.
 */
void BinaryTree::preOrder() const {
    preOrder(this->root);
    std::cout << std::endl;
}

/**
 * Private recursive preOrder method;
 */
void BinaryTree::preOrder(NodeType *root) const {
    if (root != nullptr) {
        std::cout << root->info.getValue() << " ";
        preOrder(root->left);
        preOrder(root->right);
    }
}

/**
 * Print the tree in order.
 */
void BinaryTree::inOrder() const {
    inOrder(this->root);
    std::cout << std::endl;
}

/**
 * Private recursive inOrder method;
 */
void BinaryTree::inOrder(NodeType *root) const {
    if (root != nullptr) {
        inOrder(root->left);
        std::cout << root->info.getValue() << " ";
        inOrder(root->right);
    }
}

/**
 * Print the tree in post-order.
 */
void BinaryTree::postOrder() const {
    postOrder(this->root);
    std::cout << std::endl;
}

/**
 * Private recursive postOrder method;
 */
void BinaryTree::postOrder(NodeType *root) const {
    if (root != nullptr) {
        postOrder(root->left);
        postOrder(root->right);
        std::cout << root->info.getValue() << " ";
    }
}
/**
 * Get the length of the tree.
 */
int BinaryTree::getLength() const {
    return this->length;
}

