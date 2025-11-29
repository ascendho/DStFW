#pragma once
#include <vector>

template<typename T>
requires std::equality_comparable<T>
int LinearScan(const std::vector<T>& A, const T& target);

#include "LinearScan.tpp"
