//
// Created by david on 11/21/17.
//

#include "vector"

using std::vector;

typedef unsigned long ulong;

template<class T>
void swap(T &one, T &two) {
//    if (&one == &two)
//        return;
    T temp = one;
    one = two;
    two = temp;
}

template<class T>
ulong insertionSort(vector<T> &data) {
    ulong comparisons = 0;

    for (auto i = 1; i < data.size(); i++) {
        T item = data[i];

        auto j = i - 1;
        while (j >= 0) {
            comparisons++;
            if (data[j] > item) {
                data[j + 1] = data[j];
                j--;
            } else
                break;
        }
        data[j + 1] = item;
    }

    return comparisons;
}

//template<class T>
//ulong mergeSort(vector<T> &data, ulong start, ulong end) {
//    ulong comparisons = 0;
//
//    // Trivially sorted.
//    if (data.size() <= 1)
//        return comparisons;
//
//}

template<class T>
ulong mergeSort(vector<T> &data) {
    ulong comparisons = 0;

    // Trivially sorted.
    if (data.size() <= 1)
        return comparisons;

    // Split into two equal size lists.
    vector<T> left;
    vector<T> right;

    for (ulong i = 0; i < data.size(); i++) {
        if (i < data.size() / 2)
            left.push_back(data[i]);
        else
            right.push_back(data[i]);
    }

    // Sort each branch.
    comparisons += mergeSort(left);
    comparisons += mergeSort(right);

    data.clear();

    // Comparing and merging.
    int l = 0, r = 0;
    while (l < left.size() && r < right.size()) {
        comparisons++;
        if (left[l] <= right[r]) {
            data.push_back(left[l]);
            l++;
        } else {
            data.push_back(right[r]);
            r++;
        }
    }

    // Add anything that's left.
    for (; l < left.size(); l++) {
        data.push_back(left[l]);
    }
    for (; r < left.size(); r++) {
        data.push_back(right[r]);
    }

    return comparisons;
}


template<class T>
ulong quickSort(vector<T> &data, ulong start, ulong end) {
    ulong comparisons = 0;

    // Trivially sorted.
    if (end <= start)
        return 0;

    // Median pivot.
    T pivot = data[(end - start) / 2 + start];
//    T pivot = data[start];

    // Move from the ends towards the center, and swap if necessary.
    ulong l = start;
    ulong r = end;

    while (true) {
        while (true) {
            comparisons++;
            if (data[l] < pivot)
                l++;
            else
                break;
        }

        while (true) {
            comparisons++;
            if (data[r] > pivot)
                r--;
            else
                break;
        }

        if (l >= r)
            break;

        swap(data[r], data[l]);
        l++;
        r--;
    }

    // Do this recursively on both sides of the pivot.
    comparisons += quickSort(data, start, r);
    comparisons += quickSort(data, r + 1, end);

    return comparisons;
}

template<class T>
ulong quickSort(vector<T> &data) {
    return quickSort(data, 0, data.size() - 1);
}
