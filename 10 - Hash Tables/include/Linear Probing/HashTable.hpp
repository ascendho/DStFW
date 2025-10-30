#pragma once

#include <vector>

template<typename K, typename V>
struct HashTableEntry {
    K key;
    V value;

    HashTableEntry(const K &key, const V &value)
        : key(key), value(value) {
    }
};

template<typename K, typename V>
class HashTable {
private:
    size_t size;
    size_t num_keys;
    std::vector<HashTableEntry<K, V> *> bins;

public:
    // 添加构造函数
    // explicit 关键字防止隐式转换，单参数构造函数几乎总是应该使用它
    // 例如，有了 explicit 关键字，HashTable<int> ht = 10; 这样的代码就会报错，防止从数字意外创建哈希表
    explicit HashTable(size_t initial_size)
        : size(initial_size), num_keys(0), bins(initial_size, nullptr) {
    }

    // 禁用拷贝，防止浅拷贝导致 double free
    HashTable(const HashTable &) = delete;

    HashTable &operator=(const HashTable &) = delete;

    // 析构函数：清理内存
    ~HashTable() {
        for (auto *entry: bins) {
            delete entry;
        }
    }

    // Getter 方法
    size_t getSize() const {
        return size;
    }

    size_t getNumKeys() const {
        return num_keys;
    }

    std::vector<HashTableEntry<K, V> *>& getBins() {
        return bins;
    }

    const std::vector<HashTableEntry<K, V> *>& getBins() const {
        return bins;
    }

    // Setter 方法（用于修改 num_keys）
    void setNumKeys(size_t n) {
        num_keys = n;
    }

    void incrementNumKeys() {
        ++num_keys;
    }

    void decrementNumKeys() {
        --num_keys;
    }
};
