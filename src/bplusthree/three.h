#include <vector>
#include <string>
#include <iostream>
#include <algorithm>


template <typename Key, typename Value>
struct Node {
    bool is_leaf;
    Node *parent;
    std::vector<Key> keys;
    std::vector<Node*> childs;
    std::vector<Value> values;
    Node *left;
    Node *right;
};


template <typename Key, typename Value>
class Three {
public:
    using node_type = Node<Key, Value>;
    Three() {}
    ~Three() {}

    node_type* root() const {
        return root_;
    }

    node_type* searchLeaf(Key key) {
        if (root_ == nullptr) {
            return nullptr;
        }
        node_type* node = root_;
        while (!node->is_leaf) {
            // std::cout << "keys size = " << node->keys.size() << std::endl;
            for (size_t i = 0; i <= node->keys.size(); ++i) {
                if (i == node->keys.size() || key < node->keys[i]) {
                    // std::cout << i << std::endl;
                    node = node->childs[i];
                    break;
                }
            }
        }
        return node;
    }

    void insert(Key key, Value value) {
        node_type* leaf = searchLeaf(key);
        if (leaf == nullptr) {
            root_ = new node_type;
            root_->is_leaf = true;
            root_->keys.push_back(key);
            root_->values.push_back(value);
            return;
        }

        auto middle_it = lower_bound(leaf->keys.begin(), leaf->keys.end(), key);
        size_t insert_index = middle_it - leaf->keys.begin();

        if (middle_it == leaf->keys.end()) {
            // std::cout << "Not exist: " << key << std::endl;
            leaf->values.push_back(value);
            leaf->keys.push_back(key);
        } else if (*middle_it == key) {
            // std::cout << "Update key" << std::endl;
            leaf->values[insert_index] = value;
        } else {
            // std::cout << "Insert key: " << *middle_it << " = " << key << std::endl;
            leaf->values.push_back(value);
            leaf->keys.push_back(key);            
            std::rotate(leaf->keys.begin() + insert_index, leaf->keys.end() - 1, leaf->keys.end());
            std::rotate(leaf->values.begin() + insert_index, leaf->values.end() - 1, leaf->values.end());
        }
        if (leaf->keys.size() == 2 * degree) {
            split(leaf);
        }
    }

    void remove(Key key) {
        node_type* leaf = searchLeaf(key);
        if (leaf == nullptr) {
            return;
        }
        removeKeyFromNode(leaf, key);

    }

    size_t degree{2};
    node_type *root_{};
    node_type *begin_leaf{};
private:
    void removeKeyFromNode(node_type *node, Key& key) {
        // удаление ключа из узла
        removeKeyFromLeaf(node, key);

        // если узел является конем
        if (node == root_) {
            // TODO ("Remove root node if it is empty");
            return;
        }
        // если узел заполнен не оптимально
        if (node->keys.size() < degree) {
            rebalance(node);
        }
    }

    void rebalance(node_type* node) {
        std::cout << "============== REBALANCE ==============" << std::endl;
        node_type* left_neighbor = node->left;
        node_type* right_neighbor = node->right;
        if (left_neighbor != nullptr && left_neighbor->keys.size() >= degree) {
            // Заимствование из левого узла
            node->keys.insert(node->keys.begin(), left_neighbor->keys.back());
            node->values.insert(node->values.begin(), left_neighbor->values.back());
            left_neighbor->keys.pop_back();
            left_neighbor->values.pop_back();
        } else if (right_neighbor != nullptr && right_neighbor->keys.size() >= degree) {
            // Заимствование из правого узла
            node->keys.push_back(right_neighbor->keys.front());
            node->values.push_back(right_neighbor->values.front());
            right_neighbor->keys.erase(right_neighbor->keys.begin());
            right_neighbor->values.erase(right_neighbor->values.begin());
        } else if (left_neighbor) {
            // Объедиеение с левым узлом
            std::move(left_neighbor->childs.begin(), left_neighbor->childs.end(), std::back_inserter(node->childs));
            std::move(left_neighbor->keys.begin(), left_neighbor->keys.end(), std::back_inserter(node->keys));
            if (node->right != nullptr) {
                node->right->left = left_neighbor;
            }
            left_neighbor->right = node->right;
            // delete node;
        } else if (right_neighbor) {
            // Объединение с правым узлом
            std::move(node->childs.begin(), node->childs.end(), std::front_inserter(right_neighbor->childs));
            std::move(node->keys.begin(), node->keys.end(), std::front_inserter(right_neighbor->keys));
            if (right_neighbor->right != nullptr) {
                right_neighbor->right->left = node;
            }
            node->right = right_neighbor->right;
            // delete node;
        }
    }

    void mergeNode() {

    }

    void split(node_type* node) {
        std::cout << "split node = " << node << std::endl;

        // Создание нового узла и копирование в него половиы данных
        node_type* new_node = new node_type;
        new_node->left = node;
        new_node->right = node->right;
        if (node->right) {
            node->right->left = new_node;
        }
        node->right = new_node;
        
        Key mid_key = node->keys[degree];
        // std::cout << "mid_key = " << mid_key << std::endl;
        // printNode(node);
        // std::cout << "node is root = " << (node == root_) << std::endl;

        // Перемещение части данных в новый узел
        new_node->is_leaf = node->is_leaf;
        if (node->is_leaf) {
            moveDataToNewNode(node, new_node, degree);
        } else {
            // std::cout << "node is not leaf" << std::endl;
            moveDataToNewNode(node, new_node, degree + 1);
            // std::cout << "node is not leaf" << std::endl;

            node->keys.pop_back();
        }
        // printNode(node);
        // printNode(new_node);

        new_node->parent = node->parent;
        // std::cout << "node is root = " << (node == root_) << std::endl;

        if (node == root_) {
            // std::cout << "node is root" << std::endl;
            node_type *new_root = new node_type;
            new_root->is_leaf = false;
            new_root->keys.push_back(mid_key);
            new_root->childs.push_back(node);
            new_root->childs.push_back(new_node);
            
            root_ = new_root;
            node->parent = new_root;
            new_node->parent = new_root;
            return;
        }

        node_type* parent = node->parent;
        // if (parent) {
        // std::cout << "parent = " << parent << std::endl;

        auto mid_it = lower_bound(parent->keys.begin(), parent->keys.end(), *new_node->keys.begin());
        size_t mid_index = mid_it - parent->keys.begin();
        parent->keys.push_back(mid_key);
        parent->childs.push_back(new_node);
        std::rotate(parent->keys.begin() + mid_index, parent->keys.end() - 1, parent->keys.end());
        std::rotate(parent->childs.begin() + mid_index + 1, parent->childs.end() - 1, parent->childs.end());
        printNode(parent);
        // }
        // if (parent) {
        //     auto middle_it = upper_bound(parent->keys.begin(), parent->keys.end(), *new_node->keys.begin());
        //     size_t insert_index = middle_it - parent->keys.begin();
        //     if (middle_it == parent->keys.end()) {
        //         parent->keys.push_back(*new_node->keys.begin());
        //         parent->childs.push_back(new_node);
        //     } else {
        //         parent->keys.push_back(*new_node->keys.begin());
        //         parent->childs.push_back(new_node);
        //         std::rotate(parent->keys.begin() + insert_index, parent->keys.end() - 1, parent->keys.end());
        //         std::rotate(parent->childs.begin() + insert_index, parent->childs.end() - 1, parent->childs.end());
        //     }
        //     // for (size_t i = 0; i <= parent->keys.size(); ++i) {
        //     //     if (i == parent->keys.size()) {
        //     //         parent->keys.push_back(*new_node->keys.begin());
        //     //         parent->childs.push_back(new_node);
        //     //         break;
        //     //     }
        //     //     if (*new_node->keys.begin() < parent->keys[i]) {
        //     //         parent->keys.push_back(*new_node->keys.begin());
        //     //         parent->childs.push_back(new_node);
        //     //         std::rotate(parent->keys.begin() + i, parent->keys.end() - 1, parent->keys.end());
        //     //         std::rotate(parent->values.begin() + i, parent->values.end() - 1, parent->values.end());
        //     //         break;
        //     //     }
        //     //     // std::rotate(new_node->keys.begin(), new_node->keys.begin() + i, new_node->keys.end());
        //     //     // std::rotate(new_node->childs.begin(), new_node->childs.begin() + i, new_node->childs.end());
        //     // }
        // } else {
        //     parent = new node_type;
        //     parent->is_leaf = false;
        //     parent->keys.push_back(*new_node->keys.begin());
        //     parent->childs.push_back(node);
        //     parent->childs.push_back(new_node);
        //     node->parent = parent;
        //     new_node->parent = parent;
        // }
        // // if (parent->parent == nullptr) {
        // //     root_ = parent;
        // // }

        if (parent->keys.size() == 2 * degree) {
            split(parent);
        }
    }
    
    void moveDataToNewNode(node_type *src, node_type *dst, size_t border) {
        std::move(src->keys.begin() + border, src->keys.end(), std::back_inserter(dst->keys));
        src->keys.erase(src->keys.begin() + border, src->keys.end());
        if (src->is_leaf) {
            std::move(src->values.begin() + border, src->values.end(), std::back_inserter(dst->values));
            src->values.erase(src->values.begin() + border, src->values.end());
        } else {
            std::move(src->childs.begin() + border, src->childs.end(), std::back_inserter(dst->childs));
            src->childs.erase(src->childs.begin() + border, src->childs.end());
        }
    }

    void removeKeyFromLeaf(node_type *node, Key *key) {
        auto it = lower_bound(node->keys.begin(), node->keys.end(), key);
        size_t index = it - node->keys.begin();
        if (it == node->keys.end()) {
            return;
        }
        node->keys.erase(it);
        node->values.erase(node->values.begin() + index);
    }

    void updateKeys(node_type *left, node_type *right) {
        node_type *parent_left = left->parent;
        node_type *parent_right = right->parent;

        while (parent_left != parent_right) {
            parent_left = parent_left->parent;
            parent_right = parent_right->parent;
        }
        
        if (parent_left == nullptr) {
            return;
        }
    }

 public:   
    void printNode(node_type *node) {
        std::cout << "Node: " << node << std::endl;
        for (size_t i = 0; i < node->keys.size(); ++i) {
            std::cout << "key = " << node->keys[i] << std::endl;
        }
    }
};


