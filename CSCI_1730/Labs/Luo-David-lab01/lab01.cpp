#include <iostream>
#include <cstdlib>
#include <bitset>
#include <cmath>

/**
* @author David Luo
*/
using namespace std;
typedef unsigned int uint;

/**
* Sets a value at the specified index of the unsigned int.
* @param var The variable to be modified
* @param k The number of times to divide *var*
* @param i The index to insert at
* @param val The value to insert at index *i*
*/
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

/**
* Gets the value located at a specified index.
* @param var The variable that stores the ints
* @param k The number of times to divide *var*
* @param i The index to "get"
*/
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

/**
* Main test method.
*/
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
