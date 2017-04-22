/* cal.cpp
 *  Usage: ./cal [[month] year]
 *  
 * Print a calendar with the following format:
 *
 *      April 2017
 * Su Mo Tu We Th Fr Sa
 *                    1
 *  2  3  4  5  6  7  8
 *  9 10 11 12 13 14 15
 * 16 17 18 19 20 21 22
 * 23 24 25 26 27 28 29
 * 30
 * 
 * The current day is highlighted.
 *  
 * [month] specifies the month to be displayed, represented as a 
 *     decimal integer. Default is the current month.
 * [year] the calendar displays, from 1 to 9999. Default is 
 *     current year.
 * 
 * Use the Julian calendar for dates from Jan 1, 1 through Sep 2, 1752.
 * Use the Gregorian calendar for dates from Sept 14, 1752 through Dec 31, 9999.
 */

#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <math.h>
#include <cstring>

// For colors
#define HIGHLIGHT "\x1B[30m\x1B[47m"
#define RESET "\x1B[0m"

using namespace std;

char mon[][12] = { 
    "January", "Febuary", "March", 
    "April", "May", "June", 
    "July", "August", "September", 
    "October", "November", "December" 
};
int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
char weekday[][7] = { "Su", "Mo", "Tu", "We", "Th", "Fr", "Sa" };

void printcal(struct tm *timeinfo);
bool is_gregorian(struct tm *time);
bool is_leap_year(struct tm *time);

int main(int argc, char **argv){
    // Turn off buffering
    setvbuf(stdout, NULL, _IONBF, 0);

    // Get the current day, year, month.
    time_t t;
    struct tm *timeinfo;

    time(&t); 
    timeinfo = localtime(&t);

    // Usage: ./cal [[month] year]
    if (argc > 3) {
        fprintf(stderr, "Usage: %s [[month] year]", argv[0]);
        return EXIT_FAILURE;
    }
    // Specified year
    else if (argc == 2) {
        if (atoi(argv[1]) < 1 || atoi(argv[1]) > 9999) {
            fprintf(stderr, "Enter a year between 1 and 9999.");
            return EXIT_FAILURE;
        }
        timeinfo->tm_year = atoi(argv[1]) - 1900;
        for (int i = 0; i < 12; i++) {
            timeinfo->tm_mon = i;
            mktime(timeinfo);
            printcal(timeinfo);
        }
    }
    // Specified month and year
    else if (argc == 3) {
        if (atoi(argv[1]) < 1 || atoi(argv[1]) > 12) {
            fprintf(stderr, "Enter a month between 1 and 12.");
            return EXIT_FAILURE;
        }
        if (atoi(argv[2]) < 1 || atoi(argv[2]) > 9999) {
            fprintf(stderr, "Enter a year between 1 and 9999.");
            return EXIT_FAILURE;
        }
        timeinfo->tm_mon = atoi(argv[1]) - 1;
        timeinfo->tm_year = atoi(argv[2]) - 1900;
        mktime(timeinfo);
        printcal(timeinfo);
    } 
    // Today
    else {
        mktime(timeinfo);
        printcal(timeinfo);
    }

    return EXIT_SUCCESS;
}

void printcal(struct tm *timeinfo){
    // get today
    time_t current_time;
    struct tm curr_time_info;
    time(&current_time); 
    localtime_r(&current_time, &curr_time_info);
    mktime(&curr_time_info);

    // print out the current month and year, centered.
    char month_year[21] = "";
    sprintf(month_year, "%s %d", mon[timeinfo->tm_mon], timeinfo->tm_year + 1900);
    printf("%*s\n",  10 + (int) strlen(month_year) / 2, month_year); 

    // print the headers for the days of the week
    for (int i = 0; i < 7; i++){
        printf("%s ", weekday[i]); 
    }
    printf("\n"); 

    // Get day of the week of the first day of the month
    struct tm first_dom = {};
    first_dom.tm_mday = 0;
    first_dom.tm_mon = timeinfo->tm_mon;
    first_dom.tm_year = timeinfo->tm_year;
    mktime(&first_dom);

    // get maximum day of the year, accounting for leap years
    int max_days = (timeinfo->tm_mon == 1 && is_leap_year(timeinfo))
        ? 29 : days[timeinfo->tm_mon];

    
    for (int i = -first_dom.tm_wday - 1; i < max_days; i++) {
        // skip over first row if it's all blank
        if (i == -7)
            i += 7;

        // skip over days from transition between Julian and Gregorian calendar
        if (timeinfo->tm_year + 1900 == 1752 && timeinfo->tm_mon == 8) {
            if (i < -2)
                i = -2;
            if (i > 1 && i < 13) {
                i = 13;
            }
        }

        // previous month's days
        if (i < 0)
            printf("   ");
        // highlight today
        else if (i + 1 == curr_time_info.tm_mday &&
                timeinfo->tm_mon == curr_time_info.tm_mon &&
                timeinfo->tm_year == curr_time_info.tm_year)

            printf(HIGHLIGHT "%2d" RESET " ", i + 1);
        // print everything else
        else
            printf("%2d ", i + 1);

        if (timeinfo->tm_year + 1900 == 1752 && timeinfo->tm_mon == 8) {
            // hardcode for September 1752 because i dont feel like thinking
            if (i == -3 || i == 15 || i == 22 || i == 29)
                printf("\n");
        } else {
            // print newline if new week or end of month
            if ((i + timeinfo->tm_wday - 2) % 7 == 0 || i + 1 == max_days)
                printf("\n");
        }
    }

    printf("\n");

}

/**
 * @return if the time in the struct is in the Gregorian era
 * Time between Gregorian and Julian calendar returns false
 */
bool is_gregorian(struct tm *time) {

    if (time->tm_year + 1900 < 1752)
        return false;
    if (time->tm_year + 1900 > 1752)
        return true;
    if (time->tm_year + 1900 == 1752) {
        if (time->tm_mon < 8)
            return false;
        if (time->tm_mon > 8)
            return true;
        if (time->tm_mon == 8) {
            if (time->tm_mday <= 2)
                return false;
            if (time->tm_mday >= 14)
                return true;
        }
    }

    return false;
}

// Checks for leap year, accounting for the Gregorian and Julian calendars
bool is_leap_year(struct tm *time) {
    int year = time->tm_year + 1900;
    if (is_gregorian(time)) {
        if (year % 400 == 0)
            return true;
        if (year % 100 == 0)
            return false;
        if (year % 4 == 0)
            return true;
        return false;
    } else {
        return year % 4 == 0;
    }
}

