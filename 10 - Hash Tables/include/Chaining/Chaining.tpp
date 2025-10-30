#pragma once

template<typename K, typename V>
void HashTableInsert(HashTable<K, V> &ht, const K &key, const V &value) {
    size_t hash_value = HashFunction(key, ht.getSize());

    // 如果该位置元素为空，则新建一个链表
    if (ht.getBins()[hash_value] == nullptr) {
        ht.getBins()[hash_value] = new ListNode<K, V>(key, value);
        return;
    }

    // 检查此 key 是否已存在于哈希表中
    auto current = ht.getBins()[hash_value];

    while (current->key != key && current->next != nullptr)
        current = current->next;

    // 找到同 key 的元素，更新对应的 value 值
    if (current->key == key)
        current->value = value;
    else
        // 将该 key-value 对附加到链表后面
        current->next = new ListNode<K, V>(key, value);
}

template<typename K, typename V>
ListNode<K, V> *HashTableLookup(const HashTable<K, V> &ht, const K &key) {
    size_t hash_value = HashFunction(key, ht.getSize());
    if (ht.getBins()[hash_value] == nullptr)
        return nullptr;

    auto current = ht.getBins()[hash_value];
    while (current->key != key && current->next != nullptr)
        current = current->next;

    if (current->key == key)
        return current;

    return nullptr;
}

template<typename K, typename V>
ListNode<K, V> *HashTableRemove(HashTable<K, V> &ht, const K &key) {
    size_t hash_value = HashFunction(key, ht.getSize());
    if (ht.getBins()[hash_value] == nullptr)
        return nullptr;

    // auto 自动推导类型
    auto current = ht.getBins()[hash_value];
    // 声明一个同 current 类型的指针
    decltype(current) last = nullptr;

    while (current->key != key && current->next != nullptr) {
        last = current;
        current = current->next;
    }

    if (current->key == key) {
        if (last != nullptr)
            last->next = current->next;
        else
            ht.getBins()[hash_value] = current->next;

        return current;
    }

    return nullptr;
}

template<typename K>
size_t HashFunction(const K &key, size_t size) noexcept {
    return std::hash<K>{}(key) % size;
}
