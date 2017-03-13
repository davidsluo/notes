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
    noecho();
    keypad(stdscr, TRUE);

    char c;
    while (!infile.eof()){
        infile >> noskipws >> c;
        addch(c);
    }

    int ch, x=0, y=0;
    while ((ch = getch()) != KEY_ENTER) {
        switch (ch) {
            case KEY_DOWN:
                y++;
                break;
            case KEY_UP:
                y--;
                break;
            case KEY_LEFT:
                x--;
                break;
            case KEY_RIGHT:
                x++;
                break;
        }
        mvwin(stdscr, y,x);
        refresh();
    }

    endwin();

    return EXIT_SUCCESS;
}
