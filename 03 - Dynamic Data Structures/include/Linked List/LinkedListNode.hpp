#pragma once

// LinkedListNode — 第三章：动态数据结构
// 单链表节点，包含一个值和指向下一个节点的指针。
//
// 伪代码：
//   LinkedListNode {
//       Type: value
//       LinkedListNode: next
//   }
template<typename T>
struct LinkedListNode {
    T value;
    LinkedListNode<T>* next;

    explicit LinkedListNode(const T& val) : value(val), next(nullptr) {}
};
