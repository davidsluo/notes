#ifndef BUFFER_H
#define BUFFER_H

#include <string>
#include <vector>
#include <ncurses.h>
#include "Cursors.h"

using namespace std;

class Buffer {
    public:
        Buffer();

        void init(ScreenCursor screen_cursor, WINDOW* pad);

        void insert_char(char c);
        void backspace_char();

        void move_up();
        void move_down();
        void move_left();
        void move_right();

        void move_EOL();
        void move_BOL();

        void print();

        void open(string fn="");
        void save(string fn="");

        void refresh();

        string filename = "";

        FileCursor f_curs;
        ScreenCursor s_curs;
        PadCursor p_curs;

    private:
        vector<string> content;

        WINDOW *pad;

        void insert_line(string line, int index=-1);
        void del_line(int index=-1);
        void resize_pad();

};

#endif
