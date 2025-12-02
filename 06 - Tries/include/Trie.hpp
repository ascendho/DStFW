#pragma once
#include <string>
#include "TrieNode.hpp"

class Trie {
public:
    TrieNode* root;
    
    Trie() {
        root = new TrieNode();
    }
    
    ~Trie() {
        delete root;
    }
    
    // 搜索函数
    TrieNode* TrieSearch(const std::string& target);
    TrieNode* TrieNodeSearch(TrieNode* current, const std::string& target, int index);
    
    // 插入函数
    void TrieInsert(const std::string& new_value);
    void TrieNodeInsert(TrieNode* current, const std::string& new_value, int index);
    
    // 删除函数
    void TrieDelete(const std::string& target);
    bool TrieNodeDelete(TrieNode* current, const std::string& target, int index);
};

#include "Trie.tpp"
