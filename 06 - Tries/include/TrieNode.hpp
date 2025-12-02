#pragma once
#include <array>
#include <memory>

// 假设只支持小写字母 a-z
constexpr size_t ALPHABET_SIZE = 26;

struct TrieNode {
    std::array<TrieNode*, ALPHABET_SIZE> children;
    bool is_entry;
    
    TrieNode() : is_entry(false) {
        children.fill(nullptr);
    }
    
    ~TrieNode() {
        for (auto child : children) {
            delete child;
        }
    }
};

// 将字符转换为索引 (a->0, b->1, ..., z->25)
inline int LetterToIndex(char letter) {
    return static_cast<int>(letter - 'a');
}

// 将索引转换为字符
inline char IndexToLetter(int index) {
    return static_cast<char>('a' + index);
}
