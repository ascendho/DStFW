#pragma once
#include <concepts>

template<typename T>
requires std::equality_comparable<T>
int LinearScan(const std::vector<T>& A, const T& target) {
    int i = 0;
    int n = static_cast<int>(A.size());
    while (i < n) {
        if (A[i] == target) {
            return i;
        }
        i = i + 1;
    }
    return -1;
}
