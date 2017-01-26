#include <iostream>
#include <string>
#include <sstream>
#include <limits>

using namespace std;

int main() {

  cout << "Enter integers: ";

  int min = numeric_limits<int>::max();
  int max = numeric_limits<int>::min();

  string line;
  getline(cin, line);
  stringstream lineStream(line);

  int number;

  while (lineStream >> number) {
    if (number > max) {
      max = number;
    }
    if (number < min) {
      min = number;
    }
  }

  cout << "Min:\t" << min << endl;
  cout << "Max:\t" << max << endl;
}
