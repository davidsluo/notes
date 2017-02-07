#include <iostream>

using namespace std;

bool isPrime(unsigned int x) {

	if (x == 1)
		return false;

	if (x % 2 == 0 && x != 2)
		return false;

	for (unsigned int i = 3; i < x; i+=2)
		if (x % i == 0)

			return false;

	return true;
}

int main() {
	
	unsigned int input;
	cout << "Enter an integer between 1 and 100,000 inclusive ";
	cin >> input;

	unsigned int primeSum = 0;
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
}	
