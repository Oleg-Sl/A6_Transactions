#include <vector>
#include <string>
#include <iostream>
#include <algorithm>


template <typename Key, typename Value>
struct Node {
    bool is_leaf{true};
    Node *parent{};
    std::vector<Key> keys{};
    std::vector<Node*> childs{};
    std::vector<Value> values{};
    Node *left{};;
    Node *right{};;
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
            auto it = upper_bound(node->keys.begin(), node->keys.end(), key);
            if (it == node->keys.end()) {
                node = node->childs.back();
            } else if (it == node->keys.begin() && key < node->keys.front()) {
                node = node->childs.front();
            } else {
                size_t index = it - node->keys.begin();
                node = node->childs[index];
            }
            // for (size_t i = 0; i <= node->keys.size(); ++i) {
            //     if (i == node->keys.size() || key < node->keys[i]) {
            //         node = node->childs[i];
            //         break;
            //     }
            // }
        }
        return node;
    }

    void insert(Key key, Value value) {
        node_type* leaf = searchLeaf(key);
        // std::cout << "leaf = " << leaf << std::endl;
        if (leaf == nullptr) {
            root_ = new node_type;
            // printNode(root_);
            root_->is_leaf = true;
            root_->keys.push_back(key);
            root_->values.push_back(value);
            return;
        }
        // std::cout << "2" << std::endl;
        auto middle_it = lower_bound(leaf->keys.begin(), leaf->keys.end(), key);
        size_t insert_index = middle_it - leaf->keys.begin();
        // std::cout << "3" << std::endl;

        if (middle_it == leaf->keys.end()) {
            // std::cout << "4, sz = " << leaf->values.size() << ", sz = " << leaf->keys.size()  << std::endl;
            leaf->values.push_back(value);
            leaf->keys.push_back(key);
        } else if (*middle_it == key) {
            // std::cout << "5" << std::endl;
            leaf->values[insert_index] = value;
        } else {
            // std::cout << "6, sz = " << leaf->values.size() << ", sz = " << leaf->keys.size() << std::endl;
            leaf->values.push_back(value);
            leaf->keys.push_back(key);            
            std::rotate(leaf->keys.begin() + insert_index, leaf->keys.end() - 1, leaf->keys.end());
            std::rotate(leaf->values.begin() + insert_index, leaf->values.end() - 1, leaf->values.end());
        }
        // std::cout << "7" << std::endl;
        if (leaf->keys.size() > 2 * degree) {
            // std::cout << "8" << std::endl;
            split(leaf);
            // std::cout << "9" << std::endl;
        }
    }

    void remove(Key key) {
        node_type* leaf = searchLeaf(key);
        // std::cout << "leaf = " << leaf << ", key = " << key << std::endl;
        if (leaf == nullptr) {
            // std::cout << "leaf = " << leaf << ", key = " << key << std::endl;
            return;
        }
        removeKey(leaf, key);
    }

    size_t degree{200};
    node_type *root_{};
    node_type *begin_leaf{};
private:
    void removeKey(node_type *node, Key& key) {
        // std::cout << "remove key = " << key << " from node = " << node->keys[0] << std::endl;
        // for (auto& el : node->keys) {
        //     std::cout << el << ", ";
        // }
        // std::cout << std::endl;
        removeKeyFromNode(node, key);
        // for (auto& el : node->keys) {
        //     std::cout << el << ", ";
        // }
        // std::cout << std::endl;
        
        if (node == root_) {
            updateRoot(node);
            return;
        }
        if (node->keys.size() < degree) {
            rebalance(node);
        } else {
            recursiveUpdateKeys(node->parent);
        }
    }

    void rebalance(node_type* node) {
        node_type* left_neighbor = node->left;
        node_type* right_neighbor = node->right;

        if (left_neighbor != nullptr && left_neighbor->keys.size() > degree) {
            // std::cout << "Заимствование из левого узла" << std::endl;
            borrowFromLeftNeighbor(left_neighbor, node);
        } else if (right_neighbor != nullptr && right_neighbor->keys.size() > degree) {
            // std::cout << "Заимствование из правого узла = " << node << right_neighbor->keys.size() << std::endl;
            borrowFromRightNeighbor(node, right_neighbor);
        } else if (left_neighbor && left_neighbor->parent == node->parent) {
            // std::cout << "Объединение с левым узлом" << std::endl;
            // std::cout << "left_neighbor = " << left_neighbor->keys[0] << ", node = " << node->keys[0] << std::endl;
            // std::cout << "parent left = " << left_neighbor->parent << ", parent node = " << node->parent << std::endl;
            mergeNodes(left_neighbor, node);
        } else if (right_neighbor && right_neighbor->parent == node->parent) {
            // std::cout << "Объединение с правым узлом" << std::endl;
            // std::cout << "right_neighbor = " << right_neighbor->keys[0] << ", node = " << node->keys[0] << std::endl;
            // std::cout << "parent right = " << right_neighbor->parent << ", parent node = " << node->parent << std::endl;
            mergeNodes(node, right_neighbor);
        }
    }

    void mergeNodes(node_type* left, node_type* right) {
            // std::cout << "Merge: " << "left = " << left->keys[0] << ", right = " << right->keys[0] << std::endl;
            if (left->is_leaf) {
                std::move(right->keys.begin(), right->keys.end(), std::back_inserter(left->keys));
                std::move(right->values.begin(), right->values.end(), std::back_inserter(left->values));
            } else {
                // std::transform(right->childs.begin(), right->childs.end(), std::back_inserter(left->keys), [](auto item) { return item->keys.front(); });
                for (auto& item : right->childs) {
                    left->keys.push_back(item->keys.front());
                }
                std::move(right->childs.begin(), right->childs.end(), std::back_inserter(left->childs));
                for (auto& item : left->childs) {
                    item->parent = left;
                }
            }

            if (right->right != nullptr) {
                right->right->left = left;
            }

            left->right = right->right;
            // printNode(root_);

            // auto it = upper_bound(right->parent->keys.begin(), right->parent->keys.end(), left->keys.front());
            // auto ind = 0;
            for (size_t i = 0; i < right->parent->childs.size(); ++i) {
                node_type* item = right->parent->childs[i];
                if (item == right) {
                    // ind = i;
                    removeKey(right->parent, right->parent->keys[i - 1]);
                    break;
                }
            }
            updateKeys(left->parent);
            // printTree();
            delete right;
    }

    void borrowFromLeftNeighbor(node_type* left, node_type* right) {
        right->keys.insert(right->keys.begin(), left->keys.back());
        left->keys.pop_back();

        if (right->is_leaf) {
            right->values.insert(right->values.begin(), left->values.back());
            left->values.pop_back();
        } else {
            node_type* borrowed = left->childs.back();
            right->childs.insert(right->childs.begin(), borrowed);
            borrowed->parent = right;
            left->childs.pop_back();
        }

        // recursiveUpdateKeys(left->parent);
        // recursiveUpdateKeys(right->parent);
        // if (right->is_leaf) {
            recursiveUpdateKeys(right->parent);
            // recursiveUpdateKeys(left->parent);
        // } else {
        //     recursiveUpdateKeys(right);
        //     recursiveUpdateKeys(left);

        // }
    }

    void borrowFromRightNeighbor(node_type* left, node_type* right) {
        left->keys.push_back(right->keys.front());
        right->keys.erase(right->keys.begin());

        if (left->is_leaf) {
            left->values.push_back(right->values.front());
            right->values.erase(right->values.begin());
        } else {
            node_type* borrowed = right->childs.front();
            left->childs.push_back(borrowed);
            borrowed->parent = left;
            right->childs.erase(right->childs.begin());
        }
        // recursiveUpdateKeys(left->parent);
        // recursiveUpdateKeys(right->parent);
        // recursiveUpdateKeys(right);
        // if (right->is_leaf) {
            recursiveUpdateKeys(right->parent);
            // recursiveUpdateKeys(left->parent);
        // } else {
        //     recursiveUpdateKeys(right);
        //     recursiveUpdateKeys(left);
        // }
    }

    void updateRoot(node_type *node) {
        
        // std::cout << "update root1: " << node->keys[0] << ", sz = " << node->keys.size() << std::endl;
        // printNode(node);
        if (node->keys.size() > 0) {
            return;
        }
        // std::cout << "update root2: " << node->keys[0] << std::endl;

        if (!node->is_leaf) {
            root_ = node->childs[0];
            root_->parent = nullptr;
        } else {
            root_ = nullptr;
        }
        delete node;
    }

    void recursiveUpdateKeys(node_type* node) {
        while (node) {
            updateKeys(node);
            node = node->parent;
        }
    }

    void updateKeys(node_type* node) {
        if (node == nullptr) {
            return;
        }
        for (size_t i = 0; i < node->keys.size(); ++i) {
            Key key = searchMinKey(node->childs[i + 1]);
            node->keys[i] = key;
        }
    }

    Key searchMinKey(node_type* node) {
        while (!node->is_leaf) {
            node = node->childs[0];
        }
        return node->keys[0];
    }

    void split(node_type* node) {
        node_type* new_node = new node_type;

        // ссылки на соседей
        new_node->left = node;
        new_node->right = node->right;
        if (node->right) {
            node->right->left = new_node;
        }
        node->right = new_node;

        // 
        Key mid_key = node->keys[degree];

        new_node->is_leaf = node->is_leaf;
        new_node->parent = node->parent;
        moveDataToNewNode(node, new_node, degree);

        if (node == root_) {
            node_type *new_root = new node_type;
            new_root->is_leaf = false;
            new_root->keys.push_back(mid_key);
            new_root->childs.push_back(node);
            new_root->childs.push_back(new_node);
            root_ = new_root;
            // node->is_leaf = true;
            // new_node->is_leaf = true;
            node->parent = new_root;
            new_node->parent = new_root;
            return;
        }

        node_type* parent = node->parent;

        auto mid_it = lower_bound(parent->keys.begin(), parent->keys.end(), new_node->keys.front());
        size_t mid_index = mid_it - parent->keys.begin();
        parent->keys.push_back(mid_key);
        parent->childs.push_back(new_node);
        std::rotate(parent->keys.begin() + mid_index, parent->keys.end() - 1, parent->keys.end());
        std::rotate(parent->childs.begin() + mid_index + 1, parent->childs.end() - 1, parent->childs.end());

        if (parent->keys.size() > 2 * degree) {
            split(parent);
        }
    }

    void moveDataToNewNode(node_type *src, node_type *dst, size_t border) {
        if (src->is_leaf) {
            // printNode(src);
            // printNode(dst);
            std::move(src->keys.begin() + border, src->keys.end(), std::back_inserter(dst->keys));
            std::move(src->values.begin() + border, src->values.end(), std::back_inserter(dst->values));
            src->keys.erase(src->keys.begin() + border, src->keys.end());
            src->values.erase(src->values.begin() + border, src->values.end());
            // printNode(src);
            // printNode(dst);
        } else {
            std::move(src->keys.begin() + border + 1, src->keys.end(), std::back_inserter(dst->keys));
            src->keys.erase(src->keys.begin() + border, src->keys.end());
            // std::transform(src->childs.begin() + border, src->childs.end(), src->childs.begin() + border, [dst](auto item) { return item->parent = dst; });
            // auto tmp = dst->childs.end() - 1;
            std::move(src->childs.begin() + border + 1, src->childs.end(), std::back_inserter(dst->childs));
            for (auto& item : dst->childs) {
                item->parent = dst;
            }
            // std::transform(dst->childs.begin(), dst->childs.end(), dst->childs.begin(), [dst](auto item) { return item->parent = dst; });
            // std::transform(tmp, dst->childs.end(), dst, [dst](auto item) { return item->parent = dst; });
            // std::transform(src->childs.begin(), src->childs.end(), src->childs.begin(), [dst](auto item) { return item->parent = dst; });
            src->childs.erase(src->childs.begin() + border + 1, src->childs.end());
        }
    }

    void removeKeyFromNode(node_type* node, Key& key) {
        auto it = lower_bound(node->keys.begin(), node->keys.end(), key);
        size_t index = it - node->keys.begin();
        if (it == node->keys.end() || node->keys.size() == 0) {
            return;
        }

        node->keys.erase(it);
        if (node->is_leaf) {
            node->values.erase(node->values.begin() + index);
        } else {
            node->childs.erase(node->childs.begin() + index + 1);
        }
    }

    // void updateKeys(node_type *left, node_type *right) {
    //     node_type *parent_left = left->parent;
    //     node_type *parent_right = right->parent;

    //     while (parent_left != parent_right) {
    //         parent_left = parent_left->parent;
    //         parent_right = parent_right->parent;
    //     }
        
    //     if (parent_left == nullptr) {
    //         return;
    //     }
    // }

 public:   
    void printNode(node_type *node) {
        std::cout << "========================================" << std::endl;
        std::cout << "Node: " << node << "(is leaf = " << node->is_leaf << ")" << std::endl;
        std::cout << "Parent: " << node->parent << std::endl;
        std::cout << "keys = ";
        for (size_t i = 0; i < node->keys.size(); ++i) {
            std::cout << " " << node->keys[i] << ",";
        }
        std::cout << std::endl;
        std::cout << "childs = ";
        for (size_t i = 0; i < node->childs.size(); ++i) {
            std::cout << " " << node->childs[i] << ",";
        }
        std::cout << std::endl;
        std::cout << "Left = " << node->left << ", Right = " << node->right << std::endl;
    }

    void printTree() {
        _printTree(root_);
    }

    void _printTree(node_type* node, int depth = 0) {
        for (int i = 0; i < depth; ++i) {
            std::cout << "|  ";
        }
        if (node == nullptr) {
            return;
        }
        std::cout << "|__";

        for (const auto& key : node->keys) {
            std::cout << key << " ";
        }
        std::cout << std::endl;

        if (!node->is_leaf) {
            for (const auto& child : node->childs) {
                _printTree(child, depth + 1);
            }
        }
    }
};


