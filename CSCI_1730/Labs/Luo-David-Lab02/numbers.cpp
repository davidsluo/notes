#include <bitset>
#include <limits>
#include <iostream>
#include <string>

using namespace std;

string makeBinaryLiteral(unsigned short n) {
	string output = bitset<16>(n).to_string();
	for (int i = output.length(); i > 0; i--) {
		if (i % 4 == 0 && i != output.length()) {
			output.insert(i, "'");
			i--;
				
		}
	}
	output = "0b" + output;
	return output;
}

int main() {
	const unsigned short max = numeric_limits<unsigned short>::max();
	unsigned short i = 0;
	while (i <= max) {
		cout << makeBinaryLiteral(i) << "\t";
		cout << "0t" << oct << i << " \t";
		cout << dec << i << "\t";
		cout << "0x" << hex << i << "\t";
		if (i % 3 == 0)
			cout << "Go";
		if (i % 5 == 0)
			cout << "Dawgs";
		if (i % 3 != 0 && i % 5 != 0)
			cout << "Sic'Em";
		cout << endl;

		if (i == max)
			break;
		else
			i++;
	 }
}
