#pragma once

// Push(Stack: s, Type: value):
//     IF s.top == s.array_size – 1:
//         Expand the size of the array   (array doubling)
//     s.top = s.top + 1
//     s.array[s.top] = value
template<typename T>
void Push(StackArray<T>& s, const T& value) {
    if (s.top == s.array_size - 1) {
        // Array doubling: allocate a new array of twice the size
        int new_size = s.array_size * 2;
        s.array.resize(static_cast<std::size_t>(new_size));
        s.array_size = new_size;
    }
    s.top = s.top + 1;
    s.array[static_cast<std::size_t>(s.top)] = value;
}

// Pop(Stack: s):
//     Type: value = null
//     IF s.top > -1:
//         value = s.array[s.top]
//         s.top = s.top – 1
//     return value
template<typename T>
std::optional<T> Pop(StackArray<T>& s) {
    std::optional<T> value = std::nullopt;
    if (s.top > -1) {
        value = s.array[static_cast<std::size_t>(s.top)];
        s.top = s.top - 1;
    }
    return value;
}

template<typename T>
std::optional<T> Top(const StackArray<T>& s) {
    if (s.top > -1) {
        return s.array[static_cast<std::size_t>(s.top)];
    }
    return std::nullopt;
}

template<typename T>
bool IsEmpty(const StackArray<T>& s) {
    return s.top == -1;
}
