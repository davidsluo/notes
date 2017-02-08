#include <iostream>
#include <sstream>
#include <cmath>

using namespace std;

/*
 * Returns whether *x* is a prime.
 * Mostly just a translation of https://en.wikipedia.org/wiki/Primality_test#Pseudocode
 * @param x The integer to check
 * @return If *x* is an integer.
 */
bool isPrime(unsigned int x) {

	if (x <= 1)
		return false;
    else if (x <= 3)
        return true;
    else if (x % 2 == 0 || x % 3 == 0)
		return false;
    
    // We can step by 6 because we already eliminated all numbers divisible by 
    // 2 and 3 beforehand.
    // We also only have to go up to the square root of x, which reduces
    // computation time significantly.
	for (unsigned int i = 5; i * i <= x; i+=6)
		if (x % i == 0 || x % (i + 2) == 0)
			return false;

	return true;
}

/**
 * Main. Can accept arguments from command line or prompt for them manually.
 */
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

    // Only accept numbers between 1 and 100,000 inclusive.
    if (input < 1 || input > 100000) {
        cout << "Input out of range (1-100,000)" << endl;
        return EXIT_FAILURE;
    }

	unsigned long primeSum = 0;
	unsigned int primeCount = 0;
	
    // Add *i* to primeSum if it is prime, and repeat until the number of
    // primes desired has been reached.
    for (int i = 0; primeCount < input; i++){
		if (isPrime(i)){
			primeSum += i;
			primeCount++;
		}
	}

	cout << primeSum << endl;

    return EXIT_SUCCESS;
}	
