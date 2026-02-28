#pragma once
#include <vector>
#include <optional>

// Stack implemented as a dynamic array (array doubling on overflow).
//
// Pseudocode (array-based):
//   Stack {
//       Integer: array_size
//       Integer: top
//       Array of values: array
//   }
template<typename T>
struct StackArray {
    std::vector<T> array;
    int top;       // index of the top element; -1 means empty
    int array_size;

    explicit StackArray(int initial_capacity = 4)
        : array(static_cast<std::size_t>(initial_capacity)),
          top(-1),
          array_size(initial_capacity) {}
};

// Push(Stack: s, Type: value)
template<typename T>
void Push(StackArray<T>& s, const T& value);

// Pop(Stack: s) -> returns the top value, or std::nullopt if empty
template<typename T>
std::optional<T> Pop(StackArray<T>& s);

// Peek at the top value without removing it
template<typename T>
std::optional<T> Top(const StackArray<T>& s);

template<typename T>
bool IsEmpty(const StackArray<T>& s);

#include "StackArray.tpp"
