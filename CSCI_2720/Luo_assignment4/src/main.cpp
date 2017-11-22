#include "Sorting.cpp"
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;
using std::fstream;

int main(int argc, char *argv[]) {


    fstream fs;
    if (argc > 1)
        fs.open(argv[1], fstream::in);
    else
        exit(EXIT_FAILURE);

    vector<int> data;

    if (fs.is_open()) {
        int input;
        fs >> input;
        while (!fs.eof()) {
            data.push_back(input);
            fs >> input;
        }
        fs.close();
    }

//    vector<int> data = {10,9,8,7,6,5,4,3,2,1};
//    vector<int> data = {5,4,3,2,1};

    for (int i : data)
        cout << i << " ";
    cout << endl;

//    ulong comparisons = insertionSort(data);
//    ulong comparisons = mergeSort(data);
    ulong comparisons = quickSort(data);

    for (int i : data)
        cout << i << " ";
    cout << endl;
    cout << comparisons;
}
