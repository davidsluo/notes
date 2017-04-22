#include "Buffer.h"
#include <sys/stat.h>
#include <ncurses.h>
#include <fcntl.h>
#include <unistd.h>

Buffer::Buffer() {
    f_curs = FileCursor(&content);
}

void Buffer::init(ScreenCursor screen_cursor, WINDOW* pad) {
    this->s_curs = screen_cursor;
    this->pad = pad; 
    this->p_curs = PadCursor {s_curs.max_row, s_curs.max_col, 0, 0};
}

void Buffer::insert_line(string line, int index) {
    if (index == -1)
        content.push_back(line);
    else
        content.insert(content.begin() + index, line);

    resize_pad();
}

void Buffer::del_line(int index) {
    if (index == -1)
        content.erase(content.begin() + f_curs.row); 
    else
        content.erase(content.begin() + index);
}

void Buffer::insert_char(char c) {
    // If newline,
    if (c == '\n') {
        // If not at end of line,
        if (f_curs.col < f_curs.get_max_col()) {
            // Add string from file cursor's current position to the end of the line
            // to the next line and delete it from the current line
            insert_line(f_curs.get_line().substr(
                        f_curs.col, 
                        f_curs.get_max_col() - f_curs.col), 
                    f_curs.row + 1);

            f_curs.get_line().erase(f_curs.col, f_curs.get_max_col() - f_curs.col);
        } 
        // If at end of line,
        else {
            // Add a new line.
            insert_line("", f_curs.row + 1);
        }
        // Return the cursors to the first column and move down
        f_curs.col = 0;
        s_curs.col = s_curs.min_col;
        move_down();
    } 
    // If any other character,
    else {
        // Insert the character at the current position...
        content[f_curs.row].insert(f_curs.col, 1, c);
        // And move right
        move_right();
        // f_curs.col++;
        // if (s_curs.col + 1 <= s_curs.max_col)
        //     s_curs.col++;
        // else if (p_curs.col + 1 <= p_curs.max_col)
        //     p_curs.col++;
    }

    resize_pad();
}

void Buffer::backspace_char() {
    // If we are in the first column,
    if (f_curs.col == 0) {
        // And if there is a line to delete,
        if (f_curs.row - 1 >= 0) {
            // Move cursors to end of the previous line,
            f_curs.col = (int) content[f_curs.row - 1].size();
            s_curs.col = s_curs.max_col;
            p_curs.col = p_curs.max_col;

            // Append the previous line with this line, delete this line, and move up.
            content[f_curs.row - 1] += content[f_curs.row];
            del_line();
            move_up();
        }
    }

    // If we are not in the first column, 
    else {
        // Move left and delete the character at the cursor.
        // f_curs.col--;
        // if (s_curs.col - 1 >= s_curs.min_col)
        //     s_curs.col--;
        // else if (p_curs.col - 1 >= 0)
        //     p_curs.col--;
        move_left();
        content[f_curs.row].erase(f_curs.col, 1);
    }

    resize_pad();
}

void Buffer::move_up() {
    // If we can move up,
    if(f_curs.row - 1 >= 0) {
        // Move the file cursor up,
        f_curs.row--;
        // If we can move the screen cursor up,
        if (s_curs.row - 1 >= s_curs.min_row)
            // Move it up.
            s_curs.row--;
        // Otherwise, try to move the pad up.
        else if (p_curs.row - 1 >= 0)
            p_curs.row--;
    }
    // If our current position on the previous row (now the current row)
    // is beyond the end of the line,
    if(f_curs.col > (int) content[f_curs.row].size()) {
        // Move to the end of the line.
        move_EOL();
    }

    // Lastly, commit the movement to screen.
    wmove(pad, s_curs.row, s_curs.col);
}

void Buffer::move_down() {
    // If we can move down,
    if (f_curs.row + 1 <= f_curs.get_max_row()) {
        // Move the file cursor down,
        f_curs.row++;
        // If we can move the screen cursor down,
        if(s_curs.row + 1 <= s_curs.max_row)
            s_curs.row++;
        // Otherwise, try to move the pad up.
        else if (p_curs.row + 1 <= p_curs.max_row)
            p_curs.row++;
    }
    // If our current position on the next row (now the current row)
    // is beyond the end of the line,
    if(f_curs.col > (int) content[f_curs.row].size()) {
        // Move to the end of the line.
        move_EOL();
    }

    wmove(pad, s_curs.row, s_curs.col);
}

void Buffer::move_left() {
    // If we are not in the first column,
    if(f_curs.col - 1 >= 0) {
        // Move the file cursor left,
        f_curs.col--;
        // If we can move the screen cursor left,
        if (s_curs.col - 1 >= s_curs.min_col)
            // Move it left.
            s_curs.col--;
        // Otherwise, try to move the pad to the left.
        else if (p_curs.col - 1 >= 0)
            p_curs.col--;
    } 
    // If we are in the first column and there is a previous row,
    else if (f_curs.row - 1 >= 0) {
        // Move the cursor to the end of the previous row.
        move_up();
        move_EOL();
    }

    wmove(pad, s_curs.row, s_curs.col);
}

void Buffer::move_right() {
    // If we are not at the end of the line,
    if(f_curs.col + 1 <= f_curs.get_max_col()) {
        // Move the file cursor right.
        f_curs.col++;
        // If we can move the screen cursor right,
        if (s_curs.col + 1 <= s_curs.max_col)
            // Move it right.
            s_curs.col++;
        // Otherwise, try to move the pad to the right.
        else if (p_curs.col + 1 <= p_curs.max_col)
            p_curs.col++;
    } 
    // If we are at the end of the line and there is a next row,
    else if (f_curs.row + 1 <= f_curs.get_max_row()){
        // Move the cursor to the beginning of the next row.
        move_down();
        move_BOL();
    }

    wmove(pad, s_curs.row, s_curs.col);
}

void Buffer::move_EOL() {
    // Move the file cursor to the end of the line.
    int before = f_curs.col;
    f_curs.col = (int) content[f_curs.row].size() - 1;
    int after = f_curs.col;

    int delta = after - before;

    // Then move the screen cursor left the same amount the file cursor moved,
    // excluding full screens it moved.
    s_curs.col += delta % (s_curs.max_col - s_curs.min_col);

    // If that position is not within the pad, move the pad as well.
    // TODO: Verify this works
    if (s_curs.col < p_curs.col)
        p_curs.col = s_curs.col;

    wmove(pad, s_curs.col, s_curs.row);
}

void Buffer::move_BOL() {
    f_curs.col = 0;
    s_curs.col = s_curs.min_col;
    p_curs.col = 0;
}

void Buffer::print() {
    for (int i = 0; i < f_curs.get_max_row(); i++) {
        if (i >= (int) content.size()) {
            wmove(pad, i, 0);
            wclrtoeol(pad);
        } else
            mvwprintw(pad, i, 0, content[i].c_str());
        wclrtoeol(pad);
    }
    wmove(pad, s_curs.row, s_curs.col);
}

void Buffer::refresh() {
    prefresh(pad, p_curs.row, p_curs.col,
            s_curs.min_row, s_curs.min_col,
            s_curs.max_row, s_curs.max_col - 1);
}

void Buffer::open(string fn) {
    if (fn == "") {
        content.push_back("");
    } else {
        this->filename = fn;

        int fd = ::open(filename.c_str(), O_RDWR | O_CREAT);

        if (fd == -1)
            throw "Error opening file";

        char buffer;
        string line;
        int read_size;
        while ((read_size = read(fd, &buffer, 1)) > 0) {
            if (read_size < 0) {
                close(fd);
                throw "Error reading file";
            }

            // // Replace tabs with spaces, because dealing with it properly would take too long.
            // if (buffer == '\t') {
            //     line.append("    ");
            // }

            line.push_back(buffer);

            if (buffer == '\n') {
                content.push_back(line);
                line.clear();
            }
        }

        if (!line.empty())
            content.push_back(line);

        close(fd);
    }
    resize_pad();

    print();
}

void Buffer::save(string fn) {
    string save_name;

    // If saving current file,
    if (fn == "") {
        // If this file is unnamed,
        if (filename == "") {
            throw "No filename.";
        }
        // Otherwise, save as current filename.
        else
            save_name = filename;
    }
    // If saving as another file,
    else {
        save_name = fn;
    }

    int fd = ::open(save_name.c_str(), O_WRONLY | O_CREAT);

    if (fd == -1)
        throw "Error opening file";

    fchmod(fd, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    int write_size = 0;
    for (string line : content) {
        write_size = write(fd, line.c_str(), line.size());
        if (write_size < 0)
            throw "Error writing file";
    }

    close(fd);
}


// Verify this works
void Buffer::resize_pad() {
    int longest_line_len = 0;
    int line_count = content.size();

    int pad_lines, pad_cols;
    getmaxyx(pad, pad_lines, pad_cols);

    for (string line : content)
        if ((int) line.size() > longest_line_len)
            longest_line_len = line.size();

    if (longest_line_len > pad_cols) {
        p_curs.max_col = longest_line_len - s_curs.max_col;
        pad_cols = longest_line_len;
    }

    if (line_count > pad_lines) {
        p_curs.max_row = line_count - s_curs.max_row;
        pad_lines = line_count;
    }

    wresize(pad, pad_lines, pad_cols);
}
