#include <iostream>

using namespace std;

int main() {
    unsigned long long x;
    cout << "Enter an integer between 1 and 18446744073709551615 inclusive: ";
    cin >> x;
    
    int bitsum = 0;
    while (x > 0) {
        //cout << x << endl;
        if (x % 2 == 1)
            bitsum++;
        x >>= 1;
    }
    cout << bitsum << endl;

    return EXIT_SUCCESS;
}
