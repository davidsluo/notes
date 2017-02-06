#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main() {
    cout << "Enter integers: ";

    string line;
    getline(cin, line);
    stringstream lineStream(line);

    unsigned int exclusive = 0;

    unsigned int i;
    while (lineStream >> i) {
        exclusive ^= i;
    }

    cout << "Exclusive: " << exclusive << endl;

    return EXIT_SUCCESS;
}
