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
    curs_set(0);

    WINDOW *pad = newpad(1, COLS);

    keypad(pad, TRUE);
    
    //TODO: don't cut off in middle of word?
    int max_rows, max_cols;
    getmaxyx(pad, max_rows, max_cols);
    char c = '\n';
    int num_rows = 1, num_cols = 1;
    while (!infile.eof()){
        infile >> noskipws >> c;
        
        if (num_cols + 1 > max_cols || c == '\n'){
            num_rows++;
            num_cols=1;
            wresize(pad, num_rows, max_cols);
        } else {
            num_cols++;
        }
        pechochar(pad, c);
    }
    infile.close();

    

    int ch;
    int x=0, y=0;
    int left_edge=0, right_edge=COLS - 1;
    int top_edge=0, bottom_edge=LINES - 1;

    prefresh(pad, y, x, top_edge, left_edge, bottom_edge, right_edge);
    while ((ch = wgetch(pad)) != 'q') {
        switch (ch) {
            case 'j':
            case KEY_DOWN:
                if (y + 1 > num_rows - LINES)
                    break;
                y++;
                break;
            case 'k':
            case KEY_UP:
                if (y - 1 < 0)
                    break;
                y--;
                break;
            case KEY_NPAGE:
                if (y + LINES > num_rows - LINES)
                    break;
                y+=LINES;
                break;
            case KEY_PPAGE:
                if (y - LINES < 0)
                    break;
                y-=LINES;
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
    curs_set(1);

    return EXIT_SUCCESS;
}
