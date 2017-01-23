#include <iostream>
#include <cstdlib>
#include <bitset>

using namespace std;

void setValue(unsigned int & var, unsigned int k, unsigned int i, unsigned int val) {
  int comparator = 15 << (i * 4);
  var = (var & ~comparator) + (val << i * 4);
}

unsigned int getValue(unsigned int var, unsigned int k, unsigned int i) {
  int comparator = 15 << (i * 4);
  return (var & comparator) >> (i * 4);
}

int main() {
  unsigned int var = 44391;
  unsigned int value = 10;

  cout << "before\t" << bitset<16>(var) << endl;
  cout << "insert\t" << bitset<4>(value) << endl;
  setValue(var, 4, 1, value);
  cout << "after\t" << bitset<16>(var) << endl;

  return EXIT_SUCCESS;
}
