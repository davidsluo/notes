#include <iostream>
#include <cstdlib>
#include <bitset>
#include <cmath>

// Author: David Luo

using namespace std;
typedef unsigned int uint;

void setValue(uint & var, uint k, uint i, uint val) {
  // Generate number to filter out unwanted digits
  uint mask = 0;
  for(int j = 32 / k; j > 0; j--) {
    mask += 1;
    if (j != 1) {
      mask <<= 1;
    }
  }
  mask <<= i * 32 / k;

  // (var & ~mask) = var with the space we want to set "zero'd" out.
  // + (val << i * 32 / k) = add our value (the input) to var, shifted over to the correct index.
  var = (var & ~mask) + (val << i * 32 / k);
}

uint getValue(uint var, uint k, uint i) {
  // Generate number to filter out relevant digits
  uint mask = 0;
  for(int j = 32 / k; j > 0; j--) {
    mask += 1;
    if (j != 1) {
      mask <<= 1;
    }
  }
  mask <<= i * 32 / k;

  // (var & mask) get the relevant digits
  // >> (i * 32 / k) shift over back to the right
  return (var & mask) >> (i * 32 / k);
}

int main() {
  uint var = 0;
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
    cout << "Value at index " << i << ":\t" << bitset<32>(value) << "\t" << value << endl;
  }
  return EXIT_SUCCESS;
}
