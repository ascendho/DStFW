#pragma once
#include <stdexcept>
#include "LinkedListNode.hpp"

// LinkedListLookUp — 第三章：动态数据结构
// 返回指向给定 element_number（从 0 开始索引）处节点的指针。
// 如果索引超出范围，返回 nullptr。
template<typename T>
LinkedListNode<T>* LinkedListLookUp(LinkedListNode<T>* head, int element_number);

// LinkedListInsertAfter — 第三章：动态数据结构
// 在链表中将 new_node 插入到 previous 之后。
template<typename T>
void LinkedListInsertAfter(LinkedListNode<T>* previous, LinkedListNode<T>* new_node);

// LinkedListInsert — 第三章：动态数据结构
// 在位置 index（从 0 开始）处插入一个包含给定值的新节点。
// 返回（可能更新过的）链表头。
// 如果 index 超过末尾一个以上位置，抛出 std::out_of_range。
template<typename T>
LinkedListNode<T>* LinkedListInsert(LinkedListNode<T>* head, int index, const T& value);

// LinkedListDelete — 第三章：动态数据结构
// 删除位置 index（从 0 开始）处的节点。
// 返回（可能更新过的）链表头，如果链表为空则返回 nullptr。
// 如果 index 超出范围，抛出 std::out_of_range。
template<typename T>
LinkedListNode<T>* LinkedListDelete(LinkedListNode<T>* head, int index);

#include "LinkedList.tpp"
