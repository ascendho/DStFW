#pragma once

// LinkedListNode for use in Stack/Queue linked list implementations
// (local copy for chapter 4 self-containment)
template<typename T>
struct LLNode {
    T value;
    LLNode<T>* next;

    explicit LLNode(const T& val) : value(val), next(nullptr) {}
};
