#include <iostream>

using namespace std;

int calcTime(int start, int duration) {
    int startHour = start / 100;
    int startMinute = start - startHour * 100;

    int durationHour = duration / 100;
    int durationMinute = duration - durationHour * 100;
    
    int endHour = startHour + durationHour;
    int endMinute = startMinute + durationMinute;
    
    if (endMinute > 59) {
        endHour++;
        endMinute -= 60;
    }

    if (endHour > 23)
        endHour -= 24;

    return endHour * 100 + endMinute;
}

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
