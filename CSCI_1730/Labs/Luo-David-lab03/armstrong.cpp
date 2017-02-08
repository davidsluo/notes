#include <cmath>
#include <iostream>

using namespace std;

/**
 * Checks if *x* is an Armstrong number
 * @param x The number to check
 */
bool isArmstrong(int x) {
    if (x < 100 || x > 999)
        return false;
   
    // separate x into its digits
    int hundreds = x / 100;
    int tens = (x - hundreds * 100) / 10;
    int ones = x - hundreds * 100 - tens * 10;

    return (pow(hundreds, 3) + pow(tens, 3) + pow(ones, 3)) == x;
}

/**
 * Main function
 */
int main() {
    // all 3 digit numbers
    for (int i = 100; i < 1000; i++) {
        if (isArmstrong(i)) {
            cout << i  << endl;
        }
    }

    return EXIT_SUCCESS;
}
