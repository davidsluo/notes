#include "Cursors.h"

FileCursor::FileCursor() {}

FileCursor::FileCursor(vector<string> * content) {
    this->content = content;
}

int FileCursor::get_max_row() {
    return (int) ((*content).size());
}

int FileCursor::get_max_col() { 
    return (int) ((*content)[row].size());
}

string & FileCursor::get_line() { return (*content)[row]; }
char & FileCursor::get_char() { return get_line()[col]; }
