#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2){
        cerr << "Requries exactly 1 argument." << endl;
        return EXIT_FAILURE;
    }

    ifstream infile(argv[1]);
    
    initscr();
    char c;
    while (!infile.eof()){
        infile >> noskipws >> c;
        addch(c);
    }

    refresh();
    getch();
    endwin();

    return EXIT_SUCCESS;
}
