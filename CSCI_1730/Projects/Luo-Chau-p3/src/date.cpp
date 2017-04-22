/* date.cpp
 *  Usage: ./date [+format]
 *  
 */

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string.h>
#include <time.h>

using namespace std;

const int MAX_BUFFER_LEN = 80;
const int MAX_FORMAT_LEN = 80;

int main(int argc, char **argv){
    char format[MAX_FORMAT_LEN] = "%a %b %d %T %Z %Y";
    char buffer[MAX_BUFFER_LEN];

    if ((argc == 2) && (argv[1][0] == '+')){
        char *newform = argv[1] + 1;
        strncpy(format, newform, strlen(argv[1]));  //need to be corrected so it can take variable length.
    }

    time_t t;
    struct tm *timeinfo;
    time(&t); 
    timeinfo = localtime(&t);
    
    strftime(buffer, MAX_BUFFER_LEN, format, timeinfo); 

    cout << buffer << endl;
    
    return EXIT_SUCCESS; }
