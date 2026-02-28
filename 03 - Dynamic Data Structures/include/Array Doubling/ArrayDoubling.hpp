#pragma once
#include <vector>

// ArrayDouble — 第三章：动态数据结构
// 返回一个大小为原数组两倍的新数组，所有原始元素都被复制到新数组中。
template<typename T>
std::vector<T> ArrayDouble(const std::vector<T>& old_array);

#include "ArrayDoubling.tpp"
