#include <iostream>
#include <sstream>
#include <string>

using namespace std;

/**
 * Main
 */
int main() {
    cout << "Enter integers: ";

    string line;
    getline(cin, line);
    stringstream lineStream(line);
    
    // XOR-set this variable to "toggle" the other non-exclusive numbers and 
    // leave you with only the single exclusive number
    unsigned int exclusive = 0;

    unsigned int i;
    while (lineStream >> i) {
        exclusive ^= i;
    }

    cout << "Exclusive: " << exclusive << endl;

    return EXIT_SUCCESS;
}
