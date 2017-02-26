#include <iostream>
#include "Matrix.h"

using namespace std;

int main() {

    // Construction
    Matrix a(2,2);

    // at()
    a.at(0,0) = 1;
    a.at(0,1) = 2;
    a.at(1,0) = 3;
    a.at(1,1) = 4;

    // Stream insertion
    cout << "Matrix a intialized manually." << endl;
    cout << a << endl;

    cout << endl;

    // Initialization from list
    Matrix b = {{ 5, 6 },
                { 7, 8 }};
    cout << "Matrix b initialized from list." << endl;
    cout << b << endl;

    cout << endl;

    // numRows(), numCols(), function call overloading
    cout << "Element at i,j in Matrix b." << endl;
    for (uint i = 0; i < b.numRows(); i++) {
        for (uint j = 0; j < b.numCols(); j++) {
            cout << "(" << i << ", " << j << "): " << b(i,j) << endl;
        }
    }

    cout << endl;

    // Function call assignment
    cout << "Changing element 0,0 to 6 using a function call." << endl;
    b(0,0) = 6;
    cout << b << endl;

    cout << endl;

    // Copy assingment
    Matrix c = b;
    cout << "Matrix c constructed from Matrix b." << endl;
    cout << c << endl;

    cout << endl;

    Matrix d(2,2);
    d = c;
    cout << "Matrix d constructed by default," << endl;
    cout << "then copied from Matrix c." << endl;
    cout << d << endl;

    cout << endl;

    // Operator overloading and commutitivity
    cout << "Operations on Matrix a" << endl;
    cout << "Matrix a" << endl << a << endl;

    cout << endl;

    cout << "a + 1" << endl << a + 1 << endl;
    cout << "a - 1" << endl << a - 1 << endl;
    cout << "a * 2" << endl << a * 2 << endl;
    cout << "a / 2" << endl << a / 2 << endl;

    cout << endl;

    cout << "1 + a" << endl << 1 + a << endl;
    cout << "1 - a" << endl << 1 - a << endl;
    cout << "2 * a" << endl << 2 * a << endl;
    cout << "2 / a" << endl << 2 / a << endl;

    cout << endl;

    // Matrix operators
    cout << "a + a" << endl << a + a << endl;
    cout << "a - a" << endl << a - a << endl;
    cout << "a * a" << endl << a * a << endl;

}
