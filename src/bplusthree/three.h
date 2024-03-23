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
            std::cout << "Not exist" << std::endl;
            leaf->values.push_back(value);
            leaf->keys.push_back(key);
        } else if (*middle_it == key) {
            std::cout << "Update key" << std::endl;
            leaf->values[insert_index] = value;
        } else {
            std::cout << "Insert key: " << *middle_it << " = " << key << std::endl;
            leaf->values.push_back(value);
            leaf->keys.push_back(key);            
            std::rotate(leaf->keys.begin() + insert_index, leaf->keys.end() - 1, leaf->keys.end());
            std::rotate(leaf->values.begin() + insert_index, leaf->values.end() - 1, leaf->values.end());
        }

        // for (size_t i = 0; i <= leaf->keys.size(); ++i) {
        //     if (i == leaf->keys.size()) {
        //         leaf->keys.push_back(key);
        //         leaf->values.push_back(value);
        //         break;
        //     }
        //     if (key < leaf->keys[i]) {
        //         leaf->keys.push_back(key);
        //         leaf->values.push_back(value);
        //         std::rotate(leaf->keys.begin() + i, leaf->keys.end() - 1, leaf->keys.end());
        //         std::rotate(leaf->values.begin() + i, leaf->values.end() - 1, leaf->values.end());
        //         break;
        //     }
        // }
        if (leaf->keys.size() == 2 * degree) {
            split(leaf);
        }
    }

    void remove(Key key) {
        node_type* leaf = searchLeaf(key);
        auto it = lower_bound(leaf->keys.begin(), leaf->keys.end(), key);
        size_t index = it - leaf->keys.begin();
        if (it == leaf->keys.end()) {
            return;
        }
        leaf->keys.erase(it);
        leaf->values.erase(leaf->values.begin() + index);
        if (leaf->keys < degree) {
            rebalance(leaf);
        }
    }

    size_t degree{2};
    node_type *root_{};
private:
    void rebalance(node_type* node) {

    }

    void split(node_type* node) {
        // std::cout << "split node = " << node->parent << std::endl;
        node_type* new_node = new node_type;
        new_node->is_leaf = node->is_leaf;
        new_node->parent = node->parent;
        new_node->left = node;
        new_node->right = node->right;
        if (node->right) {
            node->right->left = new_node;
        }

        node->right = new_node;
        std::move(node->keys.begin() + degree, node->keys.end(), std::back_inserter(new_node->keys));
        node->keys.erase(node->keys.begin() + degree, node->keys.end());
        if (node->is_leaf) {
            std::move(node->values.begin() + degree, node->values.end(), std::back_inserter(new_node->values));
            node->values.erase(node->values.begin() + degree, node->values.end());
        } else {
            std::move(node->childs.begin() + degree, node->childs.end(), std::back_inserter(new_node->childs));
            node->childs.erase(node->childs.begin() + degree, node->childs.end());
        }

        node_type* parent = node->parent;
        if (parent) {
            auto middle_it = upper_bound(parent->keys.begin(), parent->keys.end(), *new_node->keys.begin());
            size_t insert_index = middle_it - parent->keys.begin();
            if (middle_it == parent->keys.end()) {
                parent->keys.push_back(*new_node->keys.begin());
                parent->childs.push_back(new_node);
            } else {
                parent->keys.push_back(*new_node->keys.begin());
                parent->childs.push_back(new_node);
                std::rotate(parent->keys.begin() + insert_index, parent->keys.end() - 1, parent->keys.end());
                std::rotate(parent->childs.begin() + insert_index, parent->childs.end() - 1, parent->childs.end());
            }
            // for (size_t i = 0; i <= parent->keys.size(); ++i) {
            //     if (i == parent->keys.size()) {
            //         parent->keys.push_back(*new_node->keys.begin());
            //         parent->childs.push_back(new_node);
            //         break;
            //     }
            //     if (*new_node->keys.begin() < parent->keys[i]) {
            //         parent->keys.push_back(*new_node->keys.begin());
            //         parent->childs.push_back(new_node);
            //         std::rotate(parent->keys.begin() + i, parent->keys.end() - 1, parent->keys.end());
            //         std::rotate(parent->values.begin() + i, parent->values.end() - 1, parent->values.end());
            //         break;
            //     }
            //     // std::rotate(new_node->keys.begin(), new_node->keys.begin() + i, new_node->keys.end());
            //     // std::rotate(new_node->childs.begin(), new_node->childs.begin() + i, new_node->childs.end());
            // }
        } else {
            parent = new node_type;
            parent->is_leaf = false;
            parent->keys.push_back(*new_node->keys.begin());
            parent->childs.push_back(node);
            parent->childs.push_back(new_node);
            node->parent = parent;
            new_node->parent = parent;
        }
        if (parent->parent == nullptr) {
            root_ = parent;
        }

        // if (parent->keys.size() == 2 * degree) {
        //     split(parent);
        // }
    }

};


