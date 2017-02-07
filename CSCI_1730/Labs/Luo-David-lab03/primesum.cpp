#include <iostream>
#include <sstream>
#include <cmath>

using namespace std;

bool isPrime(unsigned int x) {

	if (x <= 1)
		return false;
    else if (x <= 3)
        return true;
    else if (x % 2 == 0 || x % 3 == 0 || x % 5 == 0 || x % 7 == 0 || x % 11 == 0)
		return false;

	for (unsigned int i = 5; i * i <= x; i+=6)
		if (x % i == 0 || x % (i + 2) == 0)
			return false;

	return true;
}

int main(int argc, char* argv[]) {
	
	unsigned int input;
    if (argc != 2){
    	cout << "Enter an integer between 1 and 100,000 inclusive: ";
    	cin >> input;
    } else {
        // This was mainly so I could time how fast this is.
        stringstream convert(argv[1]);
        convert >> input;
    }

    if (input < 1 || input > 100000) {
        cout << "Input out of range (1-100,000)" << endl;
        return EXIT_FAILURE;
    }

	unsigned long primeSum = 0;
	unsigned int primeCount = 0;
	
	int i = 0;
	while (primeCount < input) {
		if (isPrime(i)){
			primeSum += i;
			primeCount ++;

			//cout << i << endl;
		}
		i++;
	}

	cout << primeSum << endl;

    return EXIT_SUCCESS;
}	
