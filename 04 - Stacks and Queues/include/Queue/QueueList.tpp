#pragma once

// Enqueue(Queue: q, Type: value):
//     LinkedListNode: node = LinkedListNode(value)
//  1  IF q.back == null:          // queue is empty
//         q.front = node
//         q.back  = node
//     ELSE:
//  2      q.back.next = node
//  3      q.back = node
template<typename T>
void Enqueue(QueueList<T>& q, const T& value) {
    LLNode<T>* node = new LLNode<T>(value);
    if (q.back == nullptr) {  // 1 — empty queue
        q.front = node;
        q.back  = node;
    } else {
        q.back->next = node;  // 2 — link to existing tail
        q.back = node;        // 3 — update tail pointer
    }
}

// Dequeue(Queue: q):
//  1  IF q.front == null:
//         return null
//  2  Type: value = q.front.value
//  3  q.front = q.front.next
//     IF q.front == null:
//         q.back = null
//     return value
template<typename T>
std::optional<T> Dequeue(QueueList<T>& q) {
    if (q.front == nullptr) {  // 1 — empty queue
        return std::nullopt;
    }
    T value = q.front->value;  // 2 — save value
    LLNode<T>* old_front = q.front;
    q.front = q.front->next;   // 3 — advance front pointer
    if (q.front == nullptr) {
        q.back = nullptr;      // last element was removed
    }
    delete old_front;
    return value;
}

template<typename T>
std::optional<T> Front(const QueueList<T>& q) {
    if (q.front == nullptr) {
        return std::nullopt;
    }
    return q.front->value;
}

template<typename T>
bool IsEmpty(const QueueList<T>& q) {
    return q.front == nullptr;
}
