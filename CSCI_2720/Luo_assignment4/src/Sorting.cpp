//
// Created by david on 11/21/17.
//

#include "vector"

using std::vector;

typedef unsigned long ulong;

template<class T>
void swap(T &one, T &two) {
    T temp = one;
    one = two;
    two = temp;
}

template<class T>
ulong insertionSort(vector<T> &data) {
    ulong comparisons = 0;

    for (ulong i = 1; i < data.size(); i++) {
        T item = data[i];

        ulong j = i - 1;
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

template<class T>
ulong mergeSort(vector<T> &data, ulong start, ulong end) {
    ulong comparisons = 0;

    // Trivially sorted.
    if (end - start < 2)
        return comparisons;

    ulong middle = (end + start) / 2;

    comparisons += mergeSort(data, start, middle);
    comparisons += mergeSort(data, middle, end);

    vector<T> temp;

    // Comparing and merging.
    ulong l, r;
    for (l = start, r = middle; l < middle && r < end;) {
        comparisons++;
        if (data[l] < data[r]) {
            temp.push_back(data[l]);
            l++;
        } else {
            temp.push_back(data[r]);
            r++;
        }
    }

    for (; l < middle; l++)
        temp.push_back(data[l]);
    for (; r < end; r++)
        temp.push_back(data[r]);

    for (ulong i = start; i < end; i++) {
        data[i] = temp[i - start];
    }

    return comparisons;
}

template<class T>
ulong mergeSort(vector<T> &data) {
    return mergeSort(data, 0, data.size());
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
