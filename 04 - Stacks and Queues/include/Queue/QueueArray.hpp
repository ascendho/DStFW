#pragma once
#include <vector>
#include <optional>

// 基于循环数组实现的队列。
// 通过取模实现 front 和 back 索引的环绕。
//
// 伪代码（基于数组，环绕）：
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
    int front;   // 队首元素索引
    int back;    // 队尾元素索引
    int count;   // 队列中的元素数量

    explicit QueueArray(int initial_capacity = 4)
        : array(static_cast<std::size_t>(initial_capacity)),
          array_size(initial_capacity),
          front(0),
          back(-1),
          count(0) {}
};

// 入队（Enqueue）
template<typename T>
void Enqueue(QueueArray<T>& q, const T& value);

// 出队（Dequeue）—— 返回队首值，空队列时返回 std::nullopt
template<typename T>
std::optional<T> Dequeue(QueueArray<T>& q);

// 查看队首值但不移除
template<typename T>
std::optional<T> Front(const QueueArray<T>& q);

template<typename T>
bool IsEmpty(const QueueArray<T>& q);

#include "QueueArray.tpp"
