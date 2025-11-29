#pragma once

template<typename T>
requires std::totally_ordered<T>
int BinarySearch(const std::vector<T>& A, const T& target) {
    int IndexHigh = static_cast<int>(A.size()) - 1;
    int IndexLow = 0;

    while (IndexLow <= IndexHigh) {
        int IndexMid = (IndexHigh + IndexLow) / 2;

        if (A[IndexMid] == target) {
            return IndexMid;
        }

        if (A[IndexMid] < target) {
            IndexLow = IndexMid + 1;
        } else {
            IndexHigh = IndexMid - 1;
        }
    }
    return -1;
}
