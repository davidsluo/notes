#include <ncurses.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <menu.h>
#include <form.h>

#include "Buffer.h"
//#include "Dialog.h"

#define CURSOR_DEBUG false

using namespace std;

void init();
void init_editor();
void init_buffer();
void print_footer();

void print_cursor(
        int min_row, int min_col, 
        int max_row, int max_col, 
        int curr_row, int curr_col,
        const char * name, 
        int index, int divisor);

void showmenu();
void createpopup(const char* title);
void create_err(const char* err);
void open();
void save();
void saveas();
void cleanexit();
void doresize();
char* trimwhitespace(char *str);

WINDOW *menuwin;
WINDOW *editor;
WINDOW *popup; 
WINDOW *errwin;
MENU *menu;
ITEM **items;
FIELD *field;
FORM *form;

Buffer buffer;
int screen_rows, screen_cols;
bool running = true;
char * program_name;
const int nchoices = 4;

int main(int argc, char * argv[]) {
    string filename;
    program_name = argv[0];

    init();

    if (has_colors() == FALSE){
        endwin();
        cout << "Terminal does not support colors." << endl;
        exit(1); 
    }
    start_color();
    init_color(COLOR_WHITE, 128, 128, 128);
    init_color(COLOR_YELLOW, 255, 99, 71);
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_WHITE);

    filename = argc == 1 ? "" : argv[1];

    try {
        buffer.open(filename);
    } catch (const char * s) {
        endwin();
        perror(program_name);
        cerr << s << endl;
        return EXIT_FAILURE;
    }

    //Dialog dialog(editor, &buffer);
   
    int c;
    do {
        print_footer();
        buffer.print();
        buffer.refresh();
        refresh();

        c = wgetch(editor);
        // c = getch();

        switch(c) {
            case '\\':
            case KEY_F(1):
                //dialog.show_menu(); 
                showmenu(); 
                break;
            case 127:
            case '\b':
            case KEY_BACKSPACE:
                buffer.backspace_char();
                break;
            case '\n':
            case KEY_ENTER:
                buffer.insert_char('\n');
                break;
         // case '\t':
         //     // Because I cant even.
         //     buffer.insert_char(' ');
         //     buffer.insert_char(' ');
         //     buffer.insert_char(' ');
         //     buffer.insert_char(' ');
         //     break;
            case KEY_LEFT:
                buffer.move_left();
                break;
            case KEY_RIGHT:
                buffer.move_right();
                break;
            case KEY_UP:
                buffer.move_up();
                break;
            case KEY_DOWN:
                buffer.move_down();
                break;
            case KEY_RESIZE:
                doresize();
                break;
            default:
                buffer.insert_char((char) c);
                break;
        }
    } while (running);

    endwin();

    return EXIT_SUCCESS;
}

/**
 * Start the text editor stuffs.
 */
void init() {
    initscr();
    noecho();
    cbreak();

    getmaxyx(stdscr, screen_rows, screen_cols);

    init_editor();
    init_buffer();
}

/**
 * Initialize the text editor portion of the program.
 */
void init_editor() {
    if (!CURSOR_DEBUG) 
        editor = newpad(screen_rows - 1, screen_cols);
    else
        editor = newpad(screen_rows - 4, screen_cols);
    keypad(editor, true);
}

/**
 * Initialize the file buffer of the program.
 */
void init_buffer() {
    if (!CURSOR_DEBUG)
        buffer.init(ScreenCursor{0, 0, screen_rows - 1, screen_cols}, editor);
    else
        buffer.init(ScreenCursor{0, 0, screen_rows - 4, screen_cols}, editor);
}


/**
 * Print the footer of the program; file stats.
 */
void print_footer() {
    move(screen_rows - 1, 0);
    clrtoeol();

    if (!CURSOR_DEBUG) {
        // F1 Menu
        attron(COLOR_PAIR(1));
        mvprintw(screen_rows - 1, 0, "F1: Menu");
        //attroff(COLOR_PAIR(1));

        // Filename
        //attron(COLOR_PAIR(2)); 
        mvprintw(screen_rows - 1, 
                screen_cols / 2 - buffer.filename.size() / 2, 
                buffer.filename.c_str());
        //attron(COLOR_PAIR(2));

        const char * format = "LINE: %d COL: %d";
        char formatted[128];
        sprintf(formatted, format, buffer.f_curs.row, buffer.f_curs.col);

        mvprintw(screen_rows - 1, 
                screen_cols - strlen(formatted), 
                formatted);
    } else {        
        move(screen_rows - 2, 0);
        clrtoeol();
        move(screen_rows - 3, 0);
        clrtoeol();

        // F1 Menu
        mvprintw(screen_rows - 3, 0, "F1: Menu");

        // Filename
        mvprintw(screen_rows - 2, 0, buffer.filename.c_str());
      
        // Pad Cursor
        print_cursor(
                0, 0,
                buffer.p_curs.max_row, buffer.p_curs.max_col,
                buffer.p_curs.row, buffer.p_curs.col,
                "Pad", 1, 3); // Screen Cursor
        print_cursor(
                buffer.s_curs.min_row, buffer.s_curs.min_col,
                buffer.s_curs.max_row, buffer.s_curs.max_col,
                buffer.s_curs.row, buffer.s_curs.col,
                "Screen", 2, 3);
        // File Cursor
        print_cursor(
                0, 0,
                buffer.f_curs.get_max_row(), buffer.f_curs.get_max_col(),
                buffer.f_curs.row, buffer.f_curs.col,
                "File", 3, 3);
    }

    move(buffer.s_curs.row, buffer.s_curs.col);
    refresh();
}

/**
 * Print the cursor debug info.
 */
void print_cursor(
        int min_row, int min_col, 
        int max_row, int max_col, 
        int curr_row, int curr_col,
        const char * name, 
        int index, int divisor) {

    const char *format = "%*s | row: %*d col: %*d";

    char curr[128];
    char min[128];
    char max[128];

    int name_padding = strlen(name) > 3 ? strlen(name) : 3;

    int num_padding = 0;
    for (int i = max_row; i > 0; i /= 10)
        num_padding++;

    sprintf(curr, format, 
            name_padding, name, 
            num_padding, curr_row, 
            num_padding, curr_col);
    sprintf(min,  format, 
            name_padding, "min", 
            num_padding, min_row,  
            num_padding, min_col);
    sprintf(max,  format, 
            name_padding, "max", 
            num_padding, max_row,  
            num_padding, max_col);

    int print_col;
    if (index == 0)
        print_col = 0;
    else
        print_col = screen_cols * index / divisor - strlen(curr);

    mvprintw(screen_rows - 3, print_col, curr);
    mvprintw(screen_rows - 2, print_col, min);
    mvprintw(screen_rows - 1, print_col, max);
}

/* Show a window on the screen and clean up afterward. 
 * @param menu_win window to put the menu into
 * Based off an example given <a href=https://www.tldp.org/HOWTO/NCURSES-Programming-HOWTO/menus.html>here</a> 
 */
void showmenu(){
    int scr_rows, scr_cols;
    getmaxyx(stdscr, scr_rows, scr_cols); 
    
    menuwin = subwin(stdscr, scr_rows / 2, scr_cols / 2, 3, 3); 
    
    char choices[4][10] = { "Open", "Save", "Save As", "Exit" };
    void (*funclist[4])() = { open, save, saveas, cleanexit };

    int x = getmaxx(menuwin);

    items = (ITEM **) calloc(nchoices, sizeof(ITEM *));
    for (int i = 0; i < nchoices; i++){
        items[i] = new_item(choices[i], choices[i]); 
        set_item_userptr(items[i], reinterpret_cast<void *>(funclist[i]));  
    }

    menu = new_menu(items);
    keypad(menuwin, TRUE);

    set_menu_win(menu, menuwin);
    set_menu_sub(menu, derwin(menuwin, nchoices, 10, 3, 1));
    set_menu_mark(menu, "* ");
    
    box(menuwin, 0, 0);
    
    attron(COLOR_PAIR(2)); 
    mvwprintw(menuwin, 1, 2, "Editor Menu: ");  //remove hardcoding, just for testing.
    mvwaddch(menuwin, 2, 0, ACS_LTEE);
    mvwhline(menuwin, 2, 1, ACS_HLINE, x - 1); 
    mvwaddch(menuwin, 2, x - 1, ACS_RTEE);
    refresh();

    post_menu(menu);
    wrefresh(menuwin);

    wchar_t c; 
    while ((c = wgetch(menuwin)) != KEY_F(1)){
        switch(c){
            case KEY_DOWN:
                menu_driver(menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(menu, REQ_UP_ITEM);
                break;
            case KEY_ENTER:
            case '\n':
                ITEM* cur;
                void (*p)();

                cur = current_item(menu);
                p = reinterpret_cast<void (*)()>(item_userptr(cur));
                p();
                pos_menu_cursor(menu); 
                break;  
        }

        wrefresh(menuwin);
        refresh(); 
    }
    
    unpost_menu(menu);
    free_menu(menu);
    for (int i = 0; i < nchoices; i++){
        free_item(items[i]); 
    }
    
    wclear(menuwin); 
    wrefresh(menuwin); 
    delwin(menuwin); 
}

/** 
 * Create an error window.
 * @param err error message to be put on error window.
 */
void create_err(const char* err){
    const char* error_title = "ERROR"; 
    int scr_rows, scr_cols;
    getmaxyx(stdscr, scr_rows, scr_cols);
    
    errwin = subwin(stdscr, scr_rows / 4, scr_cols / 4, scr_rows / 2, (scr_cols + 6) / 2);
    
    box(errwin, 0, 0);  
    
    const int x = getmaxx(errwin); 
    mvwprintw(errwin, 1, 2, error_title);  //remove hardcoding, just for testing.
    mvwaddch(errwin, 2, 0, ACS_LTEE);
    mvwhline(errwin, 2, 1, ACS_HLINE, x - 1); 
    mvwaddch(errwin, 2, x - 1, ACS_RTEE);

    mvwprintw(errwin, 3, 1, err);
    wrefresh(errwin); 
    refresh();

    getch();

    wclear(errwin);
    wrefresh(errwin);
    delwin(errwin); 
}

/**
 * Create a popup window that accepts some kind of input.
 * @param title the title of the popup window.
 */
void createpopup(const char* title){
    int rows, cols;

    int displaced = getmaxx(menuwin) + 10;  
    popup = subwin(stdscr, 10, 30, 5, displaced);
    const int x = getmaxx(popup);

    field = new_field(1, x - 8, 3, 0, 0, 0);

    set_field_opts(field, O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE); 

    form = new_form(&field); // crash here double anything()
    scale_form(form, &rows, &cols);
    keypad(popup, TRUE);

    set_field_back(field, A_UNDERLINE); 
    //set_field_type(field, TYPE_ALNUM, 20);

    set_form_win(form, popup);
    set_form_sub(form, derwin(popup, rows, cols, 4, 4));

    box(popup, 0, 0);

    mvwprintw(popup, 1, 2, title);  //remove hardcoding, just for testing.
    mvwaddch(popup, 2, 0, ACS_LTEE);
    mvwhline(popup, 2, 1, ACS_HLINE, x - 1); 
    mvwaddch(popup, 2, x - 1, ACS_RTEE);

    mvwprintw(popup, 3, 1, "File name: ");
    refresh();

    post_form(form);
    wrefresh(popup);
    refresh();
}

/**
 * Create a window that accepts an input for a filename and then opens it.
 */
void open(){
    if (buffer.filename == "")
        saveas();

    createpopup("Open"); 

    char fname[20]; 
    wchar_t ch;
    do {
        ch = wgetch(popup);
        switch(ch){
            case '\n':
                form_driver(form, REQ_VALIDATION); 
                snprintf(fname, 20, "%s", trimwhitespace(field_buffer(field, 0)));
    
                try {
                    wclear(editor); 
                    buffer.open(fname);
                } catch (const char *s){
                    //create_err(strerror(errno)); 
                    create_err(s); 
                }
                 
                break;
            case KEY_BACKSPACE:
                form_driver(form, REQ_DEL_PREV);
                break; 
            case KEY_LEFT:
                form_driver(form, REQ_PREV_CHAR); 
                break;
            case KEY_RIGHT:
                form_driver(form, REQ_NEXT_CHAR); 
                break; 
            default:
                form_driver(form, ch);
                break; 
        }
        wrefresh(popup);
    } while (ch != '\n');

    free_form(form);
    unpost_form(form);
    free_field(field);
    wclear(popup);
    wrefresh(popup); 
    delwin(popup);
}

/**
 * Create a window that takes some input for a new filename.
 */
void saveas(){
    createpopup("Save As"); 

    char fname[20];
    wchar_t ch;
    do {
        ch = wgetch(popup);
        switch(ch){
            case '\n':
                form_driver(form, REQ_VALIDATION); 
                snprintf(fname, 20, "%s", trimwhitespace(field_buffer(field, 0)));
                
                if (fname[0] == '\0'){
                    create_err("Please enter a filename."); 
                    ch = ' '; 
                } else { 
                    try {
                        buffer.save(fname); 
                    } catch (const char *s){
                        create_err(s); 
                    }
                }
                
                break;
            case KEY_BACKSPACE:
                form_driver(form, REQ_DEL_PREV);
                break; 
            case KEY_LEFT:
                form_driver(form, REQ_PREV_CHAR); 
                break;
            case KEY_RIGHT:
                form_driver(form, REQ_NEXT_CHAR); 
                break;       
            default:
                form_driver(form, ch);
                break; 
        }
        wrefresh(popup);
    } while (ch != '\n');

    free_form(form);
    unpost_form(form);
    free_field(field);
    wclear(popup);
    wrefresh(popup); 
    delwin(popup);
}

/**
 * Save the file as its current filename.
 */
void save(){
    if (buffer.filename == ""){
        saveas(); 
    } else {
        try {
            buffer.save(); 
        } catch (const char *s){
            create_err(s); 
            //create_err(strerror(errno)); 
            //clean_exit(); 
        }
    }
}

/**
 * Resize the window when KEY_RESIZE is used.
 */
void doresize(){
    init();
}

/**
 * Exit from the program, cleaning up everything along the way out.
 */
void cleanexit(){
    unpost_menu(menu);
    free_menu(menu);
    for (int i = 0; i < nchoices; i++){
        free_item(items[i]); 
    }
    delwin(menuwin);
    delwin(popup);
    delwin(errwin);
    delwin(editor);
  
    clear();
    endwin();

    exit(EXIT_SUCCESS);
}

/**
 * trimwhitespace 
 * @param str string literal to be cleaned of white spaces.
 * @return a string without trailing whitespace (on either end). 
 * Taken from here -> http://gist.github.com/alan-mushi/c8a6f34d1df18574f643/
 */
char* trimwhitespace(char *str){
    char *end;
    while (isspace(*str))
        str++;

    if (*str == 0)
        return str;

    end = str + strnlen(str, 128) - 1;
    while (end > str && isspace(*end))
        end--; 

    *(end + 1) = '\0';
    return str;
}

