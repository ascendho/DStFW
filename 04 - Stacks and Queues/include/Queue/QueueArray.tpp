#pragma once

// 入队：将值添加到循环队列的尾部。
// 如果队列已满，将数组倍增并重新连续排列元素。
template<typename T>
void Enqueue(QueueArray<T>& q, const T& value) {
    // 满时数组倍增
    if (q.count == q.array_size) {
        int new_size = q.array_size * 2;
        std::vector<T> new_array(static_cast<std::size_t>(new_size));
        // 按逻辑顺序复制元素（从 front 到 back，环绕复制）
        for (int i = 0; i < q.count; ++i) {
            new_array[static_cast<std::size_t>(i)] =
                q.array[static_cast<std::size_t>((q.front + i) % q.array_size)];
        }
        q.array = std::move(new_array);
        q.front = 0;
        q.back = q.count - 1;
        q.array_size = new_size;
    }
    // back 指针环绕前进
    q.back = (q.back + 1) % q.array_size;
    q.array[static_cast<std::size_t>(q.back)] = value;
    q.count = q.count + 1;
}

// 出队：移除并返回队首元素。
template<typename T>
std::optional<T> Dequeue(QueueArray<T>& q) {
    if (q.count == 0) {
        return std::nullopt;
    }
    T value = q.array[static_cast<std::size_t>(q.front)];
    // front 指针环绕前进
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
