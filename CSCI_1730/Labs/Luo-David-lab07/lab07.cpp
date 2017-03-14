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
  //noecho();
  //cbreak();

    WINDOW *pad = newpad(LINES, COLS);

    keypad(pad, TRUE);
    //scrollok(pad, TRUE);
    
    //TODO: don't cut off in middle of word?
    char c;
    while (!infile.eof()){
        infile >> noskipws >> c;
        pechochar(pad, c);
    }

    int ch;
    int x=0, y=0;
    int left_edge=0, right_edge=COLS - 1;
    int top_edge=0, bottom_edge=LINES - 1;

    prefresh(pad, y, 0,0,0, COLS, LINES);
    while ((ch = wgetch(pad)) != 'q') {
        switch (ch) {
            case 'j':
            case KEY_DOWN:
                if (y + 1 > LINES)
                    break;
                y++;
                break;
            case 'k':
            case KEY_UP:
                if (y - 1 < 0)
                    break;
                y--;
                break;
          //case 'l':
          //case KEY_RIGHT:
          //    if (x + 1 > COLS)
          //        break;
          //    x++;
          //    break;
          //case 'h':
          //case KEY_LEFT:
          //    if (x - 1 < 0)
          //        break;
          //    x--;
          //    break;
        }
        //mvwin(pad, y,x);
        //prefresh: pad pminrow pmincol sminrow smincol smaxrow smaxcol 
        prefresh(pad, y, x, top_edge, left_edge, bottom_edge, right_edge);

    }

    endwin();

    return EXIT_SUCCESS;
}
