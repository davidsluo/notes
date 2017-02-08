#include <iostream>

using namespace std;

/**
 * Calculate the time
 * @param start The start time.
 * @param duration How long to add to *start*.
 */
int calcTime(int start, int duration) {
    // Separate start and duration into their respective hours and minutes.
    int startHour = start / 100;
    int startMinute = start - startHour * 100;

    int durationHour = duration / 100;
    int durationMinute = duration - durationHour * 100;
    
    // Add together the start and duration
    int endHour = startHour + durationHour;
    int endMinute = startMinute + durationMinute;
    
    // Carry over 1 hour if neccesary
    if (endMinute > 59) {
        endHour++;
        endMinute -= 60;
    }

    // Carry over to the next day if neccessary.
    if (endHour > 23)
        endHour -= 24;

    return endHour * 100 + endMinute;
}

/**
 * Main
 */
int main() {
    int startTime;
    int duration;

    cout << "Enter in a start time: ";
    cin >> startTime;

    cout << "Enter a duration: ";
    cin >> duration;

    cout << "End time is: " << calcTime(startTime, duration) << endl;

    return EXIT_SUCCESS;
}
