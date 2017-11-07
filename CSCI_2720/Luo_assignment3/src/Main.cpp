#include <fstream>
#include <iostream>
#include <limits>
#include "BinaryTree.h"
#include "ItemType.h"

using std::cout;
using std::cin;
using std::endl;
using std::fstream;
using std::string;
using std::numeric_limits;
using std::streamsize;

int main(int argc, char *argv[]) {

    BinaryTree tree;
    ItemType item;
    int input;

    // Read from file.
    fstream fs;
    if (argc > 0)
        fs.open(argv[1], fstream::in);

    if(fs.is_open()) {
        fs >> input;
        while(!fs.eof()) {
            item.initialize(input);
            tree.insert(item);
            fs >> input;
        }
        fs.close();
    }
        
    // "Shell" for tree.
    cout << "insert (i), delete (d), retrieve (r), length (l), in-order (n), pre-order (p), post-order (o), quit (q)" << endl;
    string command = "";

    while(command != "q") {
        cout << "Enter a command: ";
        cin >> command;
        
        // Insert
        if (command == "i") {
            cout << "Number to insert: ";
            int number;
            cin >> number;
            
            // Handle invalid conversions to int.
            if (cin.fail()) {
                cin.clear();
                cout << "Invalid number." << endl;
            } else {
                item.initialize(number);
                tree.insert(item);
                tree.inOrder();
            }

            // Clear stdin buffer.
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        // Delete
        } else if (command == "d") {
            cout << "Number to delete: ";
            int number;
            cin >> number;

            // Handle invalid conversions to int.
            if (cin.fail()) {
                cin.clear();
                cout << "Invalid number. " << endl;
            } else {
                item.initialize(number);
                tree.deleteItem(item);
                tree.inOrder();
            }

            // Clear stdin buffer.
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        // Retrieve
        } else if (command == "r") {
            cout << "Number to retrieve: ";
            int number;
            cin >> number;

            // Handle invalid conversions to int.
            if (cin.fail()) {
                cin.clear();
                cout << "Invalid number. " << endl;
            } else {
                item.initialize(number);
                bool found = false;
                tree.retrieve(item, found);
                if (found)
                    cout << "Item found in tree." << endl;
                else
                    cout << "Item not in tree." << endl;
            }

            // Clear stdin buffer.
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else if (command == "l") {
            cout << "Length: " << tree.getLength() << endl;
        } else if (command == "n") {
            tree.inOrder();
        } else if (command == "p") {
            tree.preOrder();
        } else if (command == "o") {
            tree.postOrder();
        } else if (command == "q") {
            break;
        } else {
            cout << "Command not recognized. Try again" << endl;
        }
    }

    cout << "Quitting program..." << endl;

    return EXIT_SUCCESS;
}
