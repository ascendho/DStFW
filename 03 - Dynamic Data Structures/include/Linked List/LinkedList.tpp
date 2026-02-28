#pragma once
#include <stdexcept>

// LinkedListLookUp
//   LinkedListLookUp(LinkedListNode: head, Integer: element_number):
//  1  LinkedListNode: current = head
//     Integer: count = 0
//  2  WHILE count < element_number AND current != null:
//         current = current.next
//         count = count + 1
//     return current
template<typename T>
LinkedListNode<T>* LinkedListLookUp(LinkedListNode<T>* head, int element_number) {
    LinkedListNode<T>* current = head;  // 1
    int count = 0;
    while (count < element_number && current != nullptr) {  // 2
        current = current->next;
        count = count + 1;
    }
    return current;
}

// LinkedListInsertAfter
//   LinkedListInsertAfter(LinkedListNode: previous, LinkedListNode: new_node):
//       new_node.next = previous.next
//       previous.next = new_node
template<typename T>
void LinkedListInsertAfter(LinkedListNode<T>* previous, LinkedListNode<T>* new_node) {
    new_node->next = previous->next;
    previous->next = new_node;
}

// LinkedListInsert
//   LinkedListInsert(LinkedListNode: head, Integer: index, Type: value):
//  1  IF index == 0:
//         LinkedListNode: new_head = LinkedListNode(value)
//         new_head.next = head
//         return new_head
//     LinkedListNode: current = head
//     LinkedListNode: previous = null
//     Integer: count = 0
//  2  WHILE count < index AND current != null:
//         previous = current
//         current = current.next
//         count = count + 1
//  3  IF count < index:
//         Produce an invalid index error.
//  4  LinkedListNode: new_node = LinkedListNode(value)
//     new_node.next = previous.next
//     previous.next = new_node
//  5  return head
template<typename T>
LinkedListNode<T>* LinkedListInsert(LinkedListNode<T>* head, int index, const T& value) {
    // 1 特殊情况：在头部插入
    if (index == 0) {
        LinkedListNode<T>* new_head = new LinkedListNode<T>(value);
        new_head->next = head;
        return new_head;
    }

    LinkedListNode<T>* current = head;
    LinkedListNode<T>* previous = nullptr;
    int count = 0;

    // 2 遍历找到插入位置
    while (count < index && current != nullptr) {
        previous = current;
        current = current->next;
        count = count + 1;
    }

    // 3 检查在到达 index 之前是否已经越过末尾
    if (count < index) {
        throw std::out_of_range("LinkedListInsert: index out of range");
    }

    // 4 拼接新节点
    LinkedListNode<T>* new_node = new LinkedListNode<T>(value);
    new_node->next = previous->next;
    previous->next = new_node;

    // 5 返回（未改变的）链表头
    return head;
}

// LinkedListDelete
//   LinkedListDelete(LinkedListNode: head, Integer: index):
//  1  IF head == null:
//         return null
//  2  IF index == 0:
//         new_head = head.next
//         head.next = null
//         return new_head
//     LinkedListNode: current = head
//     LinkedListNode: previous = null
//     Integer: count = 0
//  3  WHILE count < index AND current != null:
//         previous = current
//         current = current.next
//         count = count + 1
//  4  IF current != null:
//  5      previous.next = current.next
//  6      current.next = null
//     ELSE:
//         Produce an invalid index error.
//  7  return head
template<typename T>
LinkedListNode<T>* LinkedListDelete(LinkedListNode<T>* head, int index) {
    // 1 空链表
    if (head == nullptr) {
        return nullptr;
    }

    // 2 删除头节点
    if (index == 0) {
        LinkedListNode<T>* new_head = head->next;
        head->next = nullptr;
        delete head;
        return new_head;
    }

    LinkedListNode<T>* current = head;
    LinkedListNode<T>* previous = nullptr;
    int count = 0;

    // 3 遍历查找要删除的节点
    while (count < index && current != nullptr) {
        previous = current;
        current = current->next;
        count = count + 1;
    }

    // 4 如果找到，将节点从链表中移除
    if (current != nullptr) {
        previous->next = current->next;  // 5
        current->next = nullptr;          // 6
        delete current;
    } else {
        throw std::out_of_range("LinkedListDelete: index out of range");
    }

    // 7 返回链表头
    return head;
}
