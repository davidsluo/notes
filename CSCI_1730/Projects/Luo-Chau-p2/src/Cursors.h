#ifndef CURSORS_H
#define CURSORS_H

#include <vector>
#include <string>

using namespace std;
/*
Coordinates are in (row, col) notation.
$-------------------------------------------------------------------------------------------------+
|$(0, 0)                                                                                          |
|    #(PadCursor.row,                                                                             |
|      PadCursor.col)         ScreenCursor.min_row                                                |
|                    #------------------------------------+                                       |
|                    |                                    |                                       |
|                    |                                    |                                       |
|        ScreenCursor|                                    |ScreenCursor                           |
|            .min_col|                                    |    .max_col                           |
|                    |                                    |                                       |
|                    |                                    |                                       |
|                    +------------------------------------+                                       |
|                             ScreenCursor.max_row                                                |
|                                                                                                 |
|                                                                                                 |
|                                                                                                 |
|                                                                                                 |
|                                                                                                 |
|                                                                                                 |
|                                                                                                 |
|                                                                                                 |
|                                                                                                 |
|                                                                                                 |
|                                                                                                 |
|                                                                      @(FileCursor.get_max_row(),|
|                                                                        FileCursor.get_max_col())|
+-------------------------------------------------------------------------------------------------@
*/
class FileCursor {
    private:
        vector<string> * content;
    public:
        FileCursor();
        FileCursor(vector<string> * content);

        int row=0, col=0;

        int get_max_row();
        int get_max_col();
        string & get_line();
        char & get_char();
};

typedef struct {
    int min_row=0, min_col=0;
    int max_row=0, max_col=0;

    int row=0, col=0;
} ScreenCursor;

typedef struct {
    int max_row=0, max_col=0;

    int row=0, col=0;
} PadCursor;

#endif
