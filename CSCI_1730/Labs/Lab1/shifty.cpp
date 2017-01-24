#include <iostream>
#include <cstdlib>
#include <bitset>
#include <cmath>

// Author: David Luo

using namespace std;

void setValue(unsigned int & var, unsigned int k, unsigned int i, unsigned int val) {
  // Generate number to filter out unwanted digits
  unsigned int filter = 0;
  for(int j = 32 / k; j > 0; j--) {
    filter += 1;
    if (j != 1) {
      filter <<= 1;
    }
  }
  filter <<= i * 32 / k;

  // (var & ~filter) = var with the space we want to set "zero'd" out.
  // + (val << i * 32 / k) = add our value (the input) to var, shifted over to the correct index.
  var = (var & ~filter) + (val << i * 32 / k);
}

unsigned int getValue(unsigned int var, unsigned int k, unsigned int i) {
  // Generate number to filter out relevvant digits
  unsigned int filter = 0;
  for(int j = 32 / k; j > 0; j--) {
    filter += 1;
    if (j != 1) {
      filter <<= 1;
    }
  }
  filter <<= i * 32 / k;

  // (var & filter) get the relevant digits
  // >> (i * 32 / k) shift over back to the right
  return (var & filter) >> (i * 32 / k);
}

int main() {
  unsigned int var = 0;
  int k;
  cout << "How many splits(k)?" << endl;
  cin >> k;
  for (int i = 0; i < k; i++) {
    cout << "Insert value between 0 and " << pow(2,32/k) - 1 << " at index " << i <<":\t";
    int value;
    cin >> value;
    setValue(var, k, i, value);
  }
  cout << bitset<32>(var) << endl;
  for (int i = 0; i < k; i++) {
    int value = getValue(var, k, i);
    cout << "value at index " << i << ":\t" << bitset<32>(value) << "\t" << value << endl;
  }
  return EXIT_SUCCESS;
}
