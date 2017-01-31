#include <iostream>

using namespace std;

typedef unsigned int uint;

int main() {
    uint seconds;
    cout << "Enter in the number of seconds as an integer: ";
    cin >> seconds;
    
    uint hours = seconds / 3600;
    uint minutes = (seconds - hours * 3600) / 60;
    uint secs = seconds - hours * 3600 - minutes * 60;

    cout << seconds << " seconds is equivalent to " 
        << hours << " hours, " 
        << minutes << " minutes, "
        << secs << " seconds." << endl;
}

