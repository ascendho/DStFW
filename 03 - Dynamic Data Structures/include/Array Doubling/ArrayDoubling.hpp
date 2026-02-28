#pragma once
#include <vector>

// ArrayDouble - Chapter 3: Dynamic Data Structures
// Returns a new array twice the size of old_array, with all original elements
// copied into it.
template<typename T>
std::vector<T> ArrayDouble(const std::vector<T>& old_array);

#include "ArrayDoubling.tpp"
