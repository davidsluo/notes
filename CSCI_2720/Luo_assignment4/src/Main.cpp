#include <iostream>
#include <fstream>

#include "Sorting.cpp"

using std::cout;
using std::endl;
using std::cin;

using std::fstream;
using std::string;

int main(int argc, char *argv[]) {


    fstream fs;
    if (argc > 1)
        fs.open(argv[1], fstream::in);
    else
        exit(EXIT_FAILURE);

    vector<int> data;

    if (fs.is_open()) {
        int input;

        while (fs >> input)
            data.push_back(input);
        fs.close();
    }

    string command;

    while (command != "e") {
        cout << "(i) insertion sort, (m) merge sort, (q) quick sort, (e) exit" << endl;
        cout << "Enter a command: ";
        cin >> command;

        vector<int> sorted = data;

        // Insertion Sort
        if (command == "i") {
            ulong comparisons = insertionSort(sorted);

            cout << "Insertion Sort" << endl;
            for (auto item : sorted)
                cout << item << " ";
            cout << endl;

            cout << "Comparisons: " << comparisons << endl;
        } else if (command == "m") {
            ulong comparisons = mergeSort(sorted);

            cout << "Merge Sort" << endl;
            for (auto item : sorted)
                cout << item << " ";
            cout << endl;

            cout << "Comparisons: " << comparisons << endl;
        } else if (command == "q") {
            ulong comparisons = quickSort(sorted);

            cout << "Quick Sort" << endl;
            for (auto item : sorted)
                cout << item << " ";
            cout << endl;

            cout << "Comparisons: " << comparisons << endl;
        } else if (command == "e")
            break;
        else
            cout << "Command not recognized. Try again." << endl;
    }
}
