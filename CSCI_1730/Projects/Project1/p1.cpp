#include <iostream>
#include "Matrix.h"

using namespace std;

int main() {
    Matrix a(2,2);

    for (int i = 0; i < a.numRows(); i++) {
        for (int j = 0; j < a.numCols(); j++) {
            a.at(i,j) = (i + 1) * (j + 1);
        }
    }

    cout << a << endl;
}
