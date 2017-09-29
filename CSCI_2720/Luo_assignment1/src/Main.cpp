#include <fstream>
#include <iostream>
#include <limits>
#include "LinkedList.h"
#include "ItemType.h"

using std::cout;
using std::cin;
using std::endl;
using std::fstream;
using std::string;
using std::numeric_limits;
using std::streamsize;

int main(int argc, char *argv[]) {

    LinkedList list;
    ItemType item;
    int input;

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

    cout << "Commands - insert (i), delete (d), make empty (e), length (l), print (p)," << endl;
    cout << "get next item (g), reset list (r), quit (q)" << endl;

    string command = "";

    while(command != "q") {
        cout << "Enter a command: ";
        cin.clear();
        cin >> command;

        if (command == "i") {
            cout << "Number to insert: ";
            int number;
            cin >> number;

            if (cin.fail()) {
                cin.clear();
                cout << "Invalid number." << endl;
            } else {
                item.initialize(number);
                list.insertItem(item);
                list.print();
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else if (command == "d") {
            cout << "Number to delete: ";
            int number;
            cin >> number;

            if (cin.fail()) {
                cin.clear();
                cout << "Invalid number. " << endl;
            } else {
                item.initialize(number);
                list.deleteItem(item);
                list.print();
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else if (command == "e") {
            list.makeEmpty();
            list.print();
        } else if (command == "l") {
            cout << "Length: " << list.lengthIs() << endl;
        } else if (command == "p") {
            list.print();
        } else if (command == "g") {
            list.getNextItem(item);
            cout << "Next item: " << item.getValue() << endl;
        } else if (command == "r") {
            list.resetList();
        } else if (command == "q") {
            break;
        } else {
            cout << "Command not recognized. Try again" << endl;
        }
    }

    cout << "Quitting program..." << endl;

    return EXIT_SUCCESS;
}
