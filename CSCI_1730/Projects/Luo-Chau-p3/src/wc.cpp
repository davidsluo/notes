/** 
 * 
 */

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <string>
#include <cerrno>
#include <sys/stat.h>
#include <fcntl.h>
#include <cctype>

using namespace std;

typedef struct {
    int lines=0;
    int words=0;
    int chars=0;
    int bytes=0;
} Counts;

int word_count(const char * filename, Counts * counts);

int main(int argc, char **argv){
    setvbuf(stdout, NULL, _IONBF, 0);
    bool lines  = false, words = false, chars = false, bytes = false; 

    int c;
    while ((c = getopt(argc, argv, "c::m::l::w::")) != -1){
        switch(c){
            case 'l':
                lines  = true;
                break;
            case 'w':
                words = true;
                break; 
            case 'c':
                if (!chars)
                    bytes = true; 
                break;
            case 'm':
                if (!bytes)
                    chars = true;
                break;
        }
    }

    Counts counts = {0, 0, 0, 0};
    Counts totals = {0, 0, 0, 0};

    for (int i = 0; i < argc; i++) {

        if (strlen(argv[i]) > 1 && argv[i][0] == '-')
            continue;

        if (i == 0) {
            if (argc == 1) {
                if (word_count("-", &counts) == -1) {
                    perror(argv[0]);
                    continue;
                }
            } else
                continue;
        } else if (word_count(argv[i], &counts) == -1) {
            perror(argv[0]);
            continue;
        }

        if (argc > 2) {
            totals.lines += counts.lines;
            totals.words += counts.words;
            totals.chars += counts.chars;
            totals.bytes += counts.bytes;
        }

        if (!bytes && !chars && !lines && !words) {

            printf("%d\t%d\t%d\t%d\t%s\n", 
                    counts.lines, 
                    counts.words, 
                    counts.chars, 
                    counts.bytes, 
                    argv[i]);
        } else {
            if (lines) {
                printf("%d\t", counts.lines);
            }
            if (words) {
                printf("%d\t", counts.words);
            }
            if (chars) {
                printf("%d\t", counts.chars);
            }
            if (bytes) {
                printf("%d\t", counts.bytes);
            } 
        }
    }

    if (argc > 2)
        printf("%d\t%d\t%d\t%d\tTotals\n", 
                totals.lines, totals.words, totals.chars, totals.bytes);

    return EXIT_SUCCESS;
}

int word_count(const char * filename, Counts * counts) {
    FILE *file; 

    if (strcmp(filename, "-") == 0)
        file = fdopen(STDIN_FILENO, "r");
    else
        file = fopen(filename, "r");

    if (file == NULL)
        return -1;

    counts->lines=0;
    counts->words=0;
    counts->chars=0;
    counts->bytes=0;

    // Read file
    string text = "";
    for (char c = fgetc(file); c != EOF; c = fgetc(file)){
        text.push_back(c);
    }
    fclose(file);

    
    // Count all the things
    // char count from: 
    // http://en.cppreference.com/w/cpp/string/multibyte/mbrtowc
    setlocale(LC_ALL, "en_US.utf8");
    mbstate_t state = mbstate_t();
    const char * ptr = text.c_str();
    const char * end = ptr + strlen(ptr);
    int len;
    wchar_t wc;
    int prev_char='\0';
    while ((len = mbrtowc(&wc, ptr, end - ptr, &state)) > 0) {
        counts->chars++;
        if (wc == '\n')
            counts->lines++;

        if (!isspace(prev_char) && isspace(wc))
            counts->words++;
        if (wc != '\0' && wc != -1)
            counts->bytes += len;

        prev_char = wc;
        ptr += len;
    }

    return 0;
}
