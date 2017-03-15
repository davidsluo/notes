#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {

    // reject invalid number of args
    if (argc != 2){
        cerr << "Requries exactly 1 argument." << endl;
        return EXIT_FAILURE;
    }

    
    initscr();
  //noecho();
  //cbreak();
    curs_set(0);

    int max_rows, max_cols;
    getmaxyx(stdscr, max_rows, max_cols);

    WINDOW *pad = newpad(1, max_cols);

    keypad(pad, TRUE);
   
    // Print file to pad, increasing size of pad with each newline required.
    char c;
    int num_rows = 1, num_cols = 1;
    ifstream infile(argv[1]);
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
    int left_edge=0, right_edge=max_cols - 1;
    int top_edge=0, bottom_edge=max_rows - 1;
    
    // initial refresh
    prefresh(pad, y, x, top_edge, left_edge, bottom_edge, right_edge);

    // handle keypresses
    while ((ch = wgetch(pad)) != 'q') {
        switch (ch) {
            case 'j':
            case KEY_DOWN:
                if (y + 1 > num_rows - max_rows)
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
                if (y + max_rows > num_rows - max_rows)
                    break;
                y+=max_rows;
                break;
            case KEY_PPAGE:
                if (y - max_rows < 0)
                    break;
                y-=max_rows;
                break;
                
          //case 'l':
          //case KEY_RIGHT:
          //    if (x + 1 > max_cols)
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

        // refresh pad, scrolling to new coorinates.
        prefresh(pad, y, x, top_edge, left_edge, bottom_edge, right_edge);

    }

    endwin();
    curs_set(1);

    return EXIT_SUCCESS;
}
