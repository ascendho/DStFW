#pragma once

// Enqueue: add value to the back of the circular queue.
// If full, double the array and re-layout elements contiguously.
template<typename T>
void Enqueue(QueueArray<T>& q, const T& value) {
    // Array doubling when full
    if (q.count == q.array_size) {
        int new_size = q.array_size * 2;
        std::vector<T> new_array(static_cast<std::size_t>(new_size));
        // Copy elements in logical order (front … back, wrapping around)
        for (int i = 0; i < q.count; ++i) {
            new_array[static_cast<std::size_t>(i)] =
                q.array[static_cast<std::size_t>((q.front + i) % q.array_size)];
        }
        q.array = std::move(new_array);
        q.front = 0;
        q.back = q.count - 1;
        q.array_size = new_size;
    }
    // Advance back with wrap-around
    q.back = (q.back + 1) % q.array_size;
    q.array[static_cast<std::size_t>(q.back)] = value;
    q.count = q.count + 1;
}

// Dequeue: remove and return the front element.
template<typename T>
std::optional<T> Dequeue(QueueArray<T>& q) {
    if (q.count == 0) {
        return std::nullopt;
    }
    T value = q.array[static_cast<std::size_t>(q.front)];
    // Advance front with wrap-around
    q.front = (q.front + 1) % q.array_size;
    q.count = q.count - 1;
    return value;
}

template<typename T>
std::optional<T> Front(const QueueArray<T>& q) {
    if (q.count == 0) {
        return std::nullopt;
    }
    return q.array[static_cast<std::size_t>(q.front)];
}

template<typename T>
bool IsEmpty(const QueueArray<T>& q) {
    return q.count == 0;
}
