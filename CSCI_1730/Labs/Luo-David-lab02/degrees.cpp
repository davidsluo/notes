#include <iostream>

using namespace std;

double convertToFahrenheit(double celcius) {
	return celcius * 9.0 / 5.0 + 32.0;
}

int main() {
	double temp;
	cout << "Enter a temperature in Celcius: ";	
	cin >> temp;
	cout << "Temperature in Fahrenheit: " << convertToFahrenheit(temp) 
		<< endl;
}
