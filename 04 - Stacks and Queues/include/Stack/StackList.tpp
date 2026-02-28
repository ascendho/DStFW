#pragma once

// Push(Stack: s, Type: value):
//     LinkedListNode: node = LinkedListNode(value)
//     node.next = s.head
//     s.head = node
template<typename T>
void Push(StackList<T>& s, const T& value) {
    LLNode<T>* node = new LLNode<T>(value);
    node->next = s.head;
    s.head = node;
}

// Pop(Stack: s):
//     Type: value = null
//     IF s.head != null:
//         value = s.head.value
//         s.head = s.head.next
//     return value
template<typename T>
std::optional<T> Pop(StackList<T>& s) {
    std::optional<T> value = std::nullopt;
    if (s.head != nullptr) {
        value = s.head->value;
        LLNode<T>* old_head = s.head;
        s.head = s.head->next;
        delete old_head;
    }
    return value;
}

template<typename T>
std::optional<T> Top(const StackList<T>& s) {
    if (s.head != nullptr) {
        return s.head->value;
    }
    return std::nullopt;
}

template<typename T>
bool IsEmpty(const StackList<T>& s) {
    return s.head == nullptr;
}
