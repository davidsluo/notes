#include <fstream>
#include <iostream>
#include <limits>
#include "DoublyLinkedList.h"
#include "ItemType.h"

using std::cout;
using std::cin;
using std::endl;
using std::fstream;
using std::string;
using std::numeric_limits;
using std::streamsize;

int main(int argc, char *argv[]) {

    DoublyLinkedList list;
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
            list.insertItem(item);
            fs >> input;
        }
        fs.close();
    }
        
    // "Shell" for list.
    cout << "Commands - insert (i), delete (d), length (l), print (p), quit (q)" << endl;

    string command = "";

    while(command != "q") {
        cout << "Enter a command: ";
        cin >> command;

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
                list.insertItem(item);
                list.print();
            }

            // Clear stdin buffer.
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
                list.deleteItem(item);
                list.print();
            }

            // Clear stdin buffer.
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else if (command == "l") {
            cout << "Length: " << list.lengthIs() << endl;
        } else if (command == "p") {
            list.print();
        } else if (command == "q") {
            break;
        } else {
            cout << "Command not recognized. Try again" << endl;
        }
    }

    cout << "Quitting program..." << endl;

    return EXIT_SUCCESS;
}
