#pragma once

#include <vector>

template<typename K, typename V>
struct ListNode {
    K key;
    V value;
    ListNode *next;


    ListNode(const K &key, const V &value)
        : key(key),
          value(value), next(nullptr) {
    }
};

template<typename K, typename V>
class HashTable {
private:
    size_t size;
    std::vector<ListNode<K, V> *> bins;

public:
    explicit HashTable(size_t table_size) : size(table_size), bins(table_size, nullptr) {
    }

    // 禁用拷贝，防止浅拷贝导致 double free
    HashTable(const HashTable &) = delete;

    HashTable &operator=(const HashTable &) = delete;

    // 析构函数：清理所有链表节点
    ~HashTable() {
        for (auto &head: bins) {
            while (head != nullptr) {
                ListNode<K, V> *temp = head;
                head = head->next;
                delete temp;
            }
        }
    }

    // Getter 方法
    size_t getSize() const {
        return size;
    }

    std::vector<ListNode<K, V> *>& getBins() {
        return bins;
    }

    const std::vector<ListNode<K, V> *>& getBins() const {
        return bins;
    }
};
