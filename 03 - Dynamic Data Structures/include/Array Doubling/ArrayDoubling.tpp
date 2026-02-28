#pragma once

template<typename T>
std::vector<T> ArrayDouble(const std::vector<T>& old_array) {
    int length = static_cast<int>(old_array.size());
    std::vector<T> new_array(static_cast<std::size_t>(length) * 2);

    int j = 0;
    while (j < length) {
        new_array[j] = old_array[j];
        j = j + 1;
    }
    return new_array;
}
