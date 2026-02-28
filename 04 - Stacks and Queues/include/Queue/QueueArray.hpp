#pragma once
#include <vector>
#include <optional>

// Queue implemented as a circular array.
// Maintains front and back indices with wrap-around (modulo array_size).
//
// Pseudocode (array-based, wrap-around):
//   Queue {
//       Integer: array_size
//       Integer: front
//       Integer: back
//       Integer: count      // number of elements currently in queue
//       Array of values: array
//   }
template<typename T>
struct QueueArray {
    std::vector<T> array;
    int array_size;
    int front;   // index of the front element
    int back;    // index of the back element
    int count;   // number of elements in the queue

    explicit QueueArray(int initial_capacity = 4)
        : array(static_cast<std::size_t>(initial_capacity)),
          array_size(initial_capacity),
          front(0),
          back(-1),
          count(0) {}
};

// Enqueue(Queue: q, Type: value)
template<typename T>
void Enqueue(QueueArray<T>& q, const T& value);

// Dequeue(Queue: q) -> returns the front value, or std::nullopt if empty
template<typename T>
std::optional<T> Dequeue(QueueArray<T>& q);

// Peek at the front value without removing it
template<typename T>
std::optional<T> Front(const QueueArray<T>& q);

template<typename T>
bool IsEmpty(const QueueArray<T>& q);

#include "QueueArray.tpp"
