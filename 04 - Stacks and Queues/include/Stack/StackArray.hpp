#pragma once
#include <vector>
#include <optional>

// 基于动态数组实现的栈（溢出时数组倍增）。
//
// 伪代码（基于数组）：
//   Stack {
//       Integer: array_size
//       Integer: top
//       Array of values: array
//   }
template<typename T>
struct StackArray {
    std::vector<T> array;
    int top;       // 栈顶元素索引；-1 表示空栈
    int array_size;

    explicit StackArray(int initial_capacity = 4)
        : array(static_cast<std::size_t>(initial_capacity)),
          top(-1),
          array_size(initial_capacity) {}
};

// 入栈（Push）
template<typename T>
void Push(StackArray<T>& s, const T& value);

// 出栈（Pop）—— 返回栈顶值，空栈时返回 std::nullopt
template<typename T>
std::optional<T> Pop(StackArray<T>& s);

// 查看栈顶值但不移除
template<typename T>
std::optional<T> Top(const StackArray<T>& s);

template<typename T>
bool IsEmpty(const StackArray<T>& s);

#include "StackArray.tpp"
