#include <cmath>
#include <iostream>

using namespace std;

bool isArmstrong(int x) {
    if (x < 100 || x > 999)
        return false;
    
    int hundreds = x / 100;
    int tens = (x - hundreds * 100) / 10;
    int ones = x - hundreds * 100 - tens * 10;

    //cout << x << "\t" << hundreds << " " << tens << " " << ones << endl;
    
    return (pow(hundreds, 3) + pow(tens, 3) + pow(ones, 3)) == x;
}

int main() {
    for (int i = 100; i < 1000; i++) {
        if (isArmstrong(i)) {
            cout << i  << endl;
        }
    }

    return EXIT_SUCCESS;
}
