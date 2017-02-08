#include <iostream>

using namespace std;

/**
 * Main
 */
int main() {
    unsigned long long x;
    cout << "Enter an integer between 1 and 18446744073709551615 inclusive: ";
    cin >> x;
    
    // If the binary ends in a 1, add 1 to *bitsum* and rightshift binary by 1.
    // Repeat until x is 0.
    int bitsum = 0;
    while (x > 0) {
        if (x % 2 == 1)
            bitsum++;
        x >>= 1;
    }
    cout << bitsum << endl;

    return EXIT_SUCCESS;
}
