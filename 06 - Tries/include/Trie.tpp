#pragma once

// TrieSearch - 在 Trie 中搜索字符串
TrieNode* Trie::TrieSearch(const std::string& target) {
    return TrieNodeSearch(root, target, 0);
}

// TrieNodeSearch - 递归搜索实现
TrieNode* Trie::TrieNodeSearch(TrieNode* current, const std::string& target, int index) {
    // ❶ IF index == length(target):
    if (index == static_cast<int>(target.length())) {
        if (current->is_entry) {
            return current;
        } else {
            return nullptr;
        }
    }
    
    // ❷ Character: next_letter = target[index]
    char next_letter = target[index];
    // ❸ Integer: next_index = LetterToIndex(next_letter)
    int next_index = LetterToIndex(next_letter);
    // TrieNode: next_child = current.children[next_index]
    TrieNode* next_child = current->children[next_index];
    
    // ❹ IF next_child == null:
    if (next_child == nullptr) {
        return nullptr;
    } else {
        return TrieNodeSearch(next_child, target, index + 1);
    }
}

// TrieInsert - 向 Trie 中插入字符串
void Trie::TrieInsert(const std::string& new_value) {
    TrieNodeInsert(root, new_value, 0);
}

// TrieNodeInsert - 递归插入实现
void Trie::TrieNodeInsert(TrieNode* current, const std::string& new_value, int index) {
    // ❶ IF index == length(new_value):
    if (index == static_cast<int>(new_value.length())) {
        current->is_entry = true;
    } else {
        // Character: next_letter = new_value[index]
        char next_letter = new_value[index];
        // Integer: next_index = LetterToIndex(next_letter)
        int next_index = LetterToIndex(next_letter);
        // TrieNode: next_child = current.children[next_index]
        TrieNode* next_child = current->children[next_index];
        
        // ❷ IF next_child == null:
        if (next_child == nullptr) {
            current->children[next_index] = new TrieNode();
            // ❸ TrieNodeInsert(current.children[next_index], new_value, index + 1)
            TrieNodeInsert(current->children[next_index], new_value, index + 1);
        } else {
            // ❹ TrieNodeInsert(next_child, new_value, index + 1)
            TrieNodeInsert(next_child, new_value, index + 1);
        }
    }
}

// TrieDelete - 从 Trie 中删除字符串
void Trie::TrieDelete(const std::string& target) {
    TrieNodeDelete(root, target, 0);
}

// TrieNodeDelete - 递归删除实现
bool Trie::TrieNodeDelete(TrieNode* current, const std::string& target, int index) {
    // ❶ IF index == length(target):
    if (index == static_cast<int>(target.length())) {
        if (current->is_entry) {
            current->is_entry = false;
        }
    } else {
        // ❷ Character: next_letter = target[index]
        char next_letter = target[index];
        // Integer: next_index = LetterToIndex(next_letter)
        int next_index = LetterToIndex(next_letter);
        // TrieNode: next_child = current.children[next_index]
        TrieNode* next_child = current->children[next_index];
        
        if (next_child != nullptr) {
            // ❸ IF TrieNodeDelete(next_child, target, index+1):
            if (TrieNodeDelete(next_child, target, index + 1)) {
                current->children[next_index] = nullptr;
            }
        }
    }
    
    // 如果该节点是词条或有子节点，则不删除。
    // ❹ IF current.is_entry:
    if (current->is_entry) {
        return false;
    }
    
    // ❺ FOR EACH ptr IN current.children:
    for (auto ptr : current->children) {
        if (ptr != nullptr) {
            return false;
        }
    }
    
    return true;
}
