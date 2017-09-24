#include <fstream>
#include <iostream>
#include "LinkedList.h"
#include "ItemType.h"
int main(int argc, char *argv[]) {

    LinkedList list;
    ItemType item;
    int input;
    std::fstream fs;
    fs.open(argv[1], std::fstream::in);

    if(fs.is_open()) {
        fs >> input;
        while(!fs.eof()) {
            item.initialize(input);
            list.insertItem(item);
            fs >> input;
        }
    }
    else {
        std::cout << "File could not be opened. " <<
            "Try again." << std::endl;
        return 0;
    }

}
