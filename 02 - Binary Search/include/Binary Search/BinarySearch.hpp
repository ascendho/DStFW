#pragma once
#include <vector>
#include <concepts>

template<typename T>
requires std::totally_ordered<T>
int BinarySearch(const std::vector<T>& A, const T& target);

#include "BinarySearch.tpp"
