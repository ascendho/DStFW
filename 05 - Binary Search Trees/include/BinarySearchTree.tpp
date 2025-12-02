#pragma once

// FindValue - 递归搜索实现
template<typename T>
requires std::totally_ordered<T>
TreeNode<T>* BinarySearchTree<T>::FindValue(TreeNode<T>* current, const T& target) {
    // ❶ IF current == null:
    if (current == nullptr) {
        return nullptr;
    }
    
    // ❷ IF current.value == target:
    if (current->value == target) {
        return current;
    }
    
    // ❸ IF target < current.value AND current.left != null:
    if (target < current->value && current->left != nullptr) {
        return FindValue(current->left, target);
    }
    
    // ❹ IF target > current.value AND current.right != null:
    if (target > current->value && current->right != nullptr) {
        return FindValue(current->right, target);
    }
    
    // ❺ return null
    return nullptr;
}

// FindValueItr - 迭代搜索实现
template<typename T>
requires std::totally_ordered<T>
TreeNode<T>* BinarySearchTree<T>::FindValueItr(TreeNode<T>* root_node, const T& target) {
    // ❶ TreeNode: current = root
    TreeNode<T>* current = root_node;
    
    // ❷ WHILE current != null AND current.value != target:
    while (current != nullptr && current->value != target) {
        // ❸ IF target < current.value:
        if (target < current->value) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    // ❹ return current
    return current;
}

// FindTreeNode - 搜索的包装函数
template<typename T>
requires std::totally_ordered<T>
TreeNode<T>* BinarySearchTree<T>::FindTreeNode(const T& target) {
    // IF tree.root == null:
    if (root == nullptr) {
        return nullptr;
    }
    return FindValue(root, target);
}

// InsertNode - 递归插入辅助函数
template<typename T>
requires std::totally_ordered<T>
void BinarySearchTree<T>::InsertNode(TreeNode<T>* current, const T& new_value) {
    // ❶ IF new_value == current.value:
    if (new_value == current->value) {
        // 根据需要更新节点
        return;
    }
    
    // ❷ IF new_value < current.value:
    if (new_value < current->value) {
        // ❸ IF current.left != null:
        if (current->left != nullptr) {
            InsertNode(current->left, new_value);
        } else {
            current->left = new TreeNode<T>(new_value);
            current->left->parent = current;
        }
    } else {
        // ❹ IF current.right != null:
        if (current->right != nullptr) {
            InsertNode(current->right, new_value);
        } else {
            current->right = new TreeNode<T>(new_value);
            current->right->parent = current;
        }
    }
}

// InsertTreeNode - 插入的包装函数
template<typename T>
requires std::totally_ordered<T>
void BinarySearchTree<T>::InsertTreeNode(const T& new_value) {
    // IF tree.root == null:
    if (root == nullptr) {
        root = new TreeNode<T>(new_value);
    } else {
        InsertNode(root, new_value);
    }
}

// RemoveTreeNode - 从树中删除一个节点
template<typename T>
requires std::totally_ordered<T>
void BinarySearchTree<T>::RemoveTreeNode(TreeNode<T>* node) {
    // ❶ IF tree.root == null OR node == null:
    if (root == nullptr || node == nullptr) {
        return;
    }
    
    // 情况 A: 删除叶子节点
    // ❷ IF node.left == null AND node.right == null:
    if (node->left == nullptr && node->right == nullptr) {
        if (node->parent == nullptr) {
            root = nullptr;
        } else if (node->parent->left == node) {
            node->parent->left = nullptr;
        } else {
            node->parent->right = nullptr;
        }
        delete node;
        return;
    }
    
    // 情况 B: 删除只有一个子节点的节点
    // ❸ IF node.left == null OR node.right == null:
    if (node->left == nullptr || node->right == nullptr) {
        // ❹ TreeNode: child = node.left
        TreeNode<T>* child = node->left;
        if (node->left == nullptr) {
            child = node->right;
        }
        
        // ❺ child.parent = node.parent
        child->parent = node->parent;
        
        if (node->parent == nullptr) {
            root = child;
        } else if (node->parent->left == node) {
            node->parent->left = child;
        } else {
            node->parent->right = child;
        }
        
        delete node;
        return;
    }
    
    // 情况 C: 删除有两个子节点的节点
    // 找到后继节点并将其从树中移除
    // ❻ TreeNode: successor = node.right
    TreeNode<T>* successor = node->right;
    while (successor->left != nullptr) {
        successor = successor->left;
    }
    
    // 在删除前保存后继节点的值
    T successor_value = successor->value;
    
    // 删除后继节点
    RemoveTreeNode(successor);
    
    // 用后继节点的值替换当前节点的值
    node->value = successor_value;
}
