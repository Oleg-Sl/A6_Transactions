#ifndef TRANSACTIONS_SOURCE_MODEL_BPLUSTREE_TREE_H_
#define TRANSACTIONS_SOURCE_MODEL_BPLUSTREE_TREE_H_

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace s21 {

template <typename Key, typename Value>
class Three {
 public:
  struct Node {
    bool is_leaf{true};
    Node* parent{};
    std::vector<Key> keys{};
    std::vector<Node*> childs{};
    std::vector<Value> values{};
    Node* left{};
    Node* right{};
  };

  class Iterator {
   public:
    Iterator(Node* node, size_t ind = 0) : node_(node), ind_(ind){};

    Iterator& operator++() noexcept {
      if (node_ == nullptr) {
        return *this;
      }
      ind_ += 1;
      if (ind_ >= node_->keys.size()) {
        node_ = node_->right;
        ind_ = 0;
      }
      return *this;
    }

    bool operator!=(const Iterator& other) {
      return node_ != other.node_ || ind_ != other.ind_;
    }

    bool operator==(const Iterator& other) {
      return node_ == other.node_ && ind_ == other.ind_;
    }

    std::pair<Key, Value> operator*() {
      if (node_ == nullptr) {
        return std::pair<Key, Value>{};
      }
      return {node_->keys[ind_], node_->values[ind_]};
    }

    std::pair<Key, Value>* operator->() {
      if (node_ == nullptr) {
        return std::pair<Key, Value>{};
      }
      return {node_->keys[ind_], node_->values[ind_]};
    }

   private:
    Node* node_;
    size_t ind_;
  };

  Three() {}
  ~Three() { Clear(root_); }

  Node* root() const { return root_; }

  bool Exists(const Key& key) const {
    Node* leaf = searchLeaf(key);
    if (leaf == nullptr) {
      return false;
    }
    auto it = lower_bound(leaf->keys.begin(), leaf->keys.end(), key);
    return it != leaf->keys.end() && *it == key;
  }

  bool Update(const Key& key, const Value& value) {
    Node* leaf = searchLeaf(key);
    if (leaf == nullptr) {
      return false;
    }
    auto it = lower_bound(leaf->keys.begin(), leaf->keys.end(), key);
    if (it == leaf->keys.end() || *it != key) {
      return false;
    }
    size_t index = std::distance(leaf->keys.begin(), it);
    leaf->values[index] = value;
    return true;
  }

  const Value& search(const Key& key) const {
    Node* leaf = searchLeaf(key);
    if (leaf == nullptr) {
      throw std::invalid_argument("Key not found");
    }
    auto it = lower_bound(leaf->keys.begin(), leaf->keys.end(), key);
    if (it == leaf->keys.end() || *it != key) {
      throw std::invalid_argument("Key not found");
    }
    size_t index = std::distance(leaf->keys.begin(), it);
    return leaf->values[index];
  }

  Node* searchLeaf(const Key& key) const {
    if (root_ == nullptr) {
      return nullptr;
    }
    Node* node = root_;
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
    }
    return node;
  }

  bool insert(Key key, Value value) {
    Node* leaf = searchLeaf(key);
    if (leaf == nullptr) {
      root_ = new Node;
      root_->is_leaf = true;
      root_->keys.push_back(key);
      root_->values.push_back(value);
      begin_ = root_;
      return true;
    }
    auto middle_it = lower_bound(leaf->keys.begin(), leaf->keys.end(), key);
    if (*middle_it == key) {
      return false;
    };
    size_t insert_index = middle_it - leaf->keys.begin();
    if (middle_it == leaf->keys.end()) {
      leaf->values.push_back(value);
      leaf->keys.push_back(key);
    } else if (*middle_it == key) {
      leaf->values[insert_index] = value;
    } else {
      leaf->values.push_back(value);
      leaf->keys.push_back(key);
      std::rotate(leaf->keys.begin() + insert_index, leaf->keys.end() - 1,
                  leaf->keys.end());
      std::rotate(leaf->values.begin() + insert_index, leaf->values.end() - 1,
                  leaf->values.end());
    }
    if (leaf->keys.size() > 2 * degree) {
      split(leaf);
    }
    return true;
  }

  bool remove(const Key& key) {
    Node* leaf = searchLeaf(key);
    if (leaf == nullptr) {
      return false;
    }
    return removeKey(leaf, key);
  }

  Iterator Begin() const { return Iterator(begin_); }

  Iterator End() const { return Iterator(nullptr); }

 private:
  size_t degree{20};
  Node* root_{};
  Node* begin_{};

  bool removeKey(Node* node, const Key& key) {
    bool is_removed = removeKeyFromNode(node, key);

    if (node == root_) {
      updateRoot(node);
      return is_removed;
    }
    if (node->keys.size() < degree) {
      rebalance(node);
    } else {
      recursiveUpdateKeys(node->parent);
    }
    return is_removed;
  }

  void rebalance(Node* node) {
    Node* left_neighbor = node->left;
    Node* right_neighbor = node->right;

    if (left_neighbor != nullptr && left_neighbor->keys.size() > degree) {
      borrowFromLeftNeighbor(left_neighbor, node);
    } else if (right_neighbor != nullptr &&
               right_neighbor->keys.size() > degree) {
      borrowFromRightNeighbor(node, right_neighbor);
    } else if (left_neighbor && left_neighbor->parent == node->parent) {
      mergeNodes(left_neighbor, node);
    } else if (right_neighbor && right_neighbor->parent == node->parent) {
      mergeNodes(node, right_neighbor);
    }
  }

  void mergeNodes(Node* left, Node* right) {
    if (left->is_leaf) {
      std::move(right->keys.begin(), right->keys.end(),
                std::back_inserter(left->keys));
      std::move(right->values.begin(), right->values.end(),
                std::back_inserter(left->values));
    } else {
      for (auto& item : right->childs) {
        left->keys.push_back(item->keys.front());
      }
      std::move(right->childs.begin(), right->childs.end(),
                std::back_inserter(left->childs));
      for (auto& item : left->childs) {
        item->parent = left;
      }
    }
    if (right->right != nullptr) {
      right->right->left = left;
    }
    left->right = right->right;
    for (size_t i = 0; i < right->parent->childs.size(); ++i) {
      Node* item = right->parent->childs[i];
      if (item == right) {
        removeKey(right->parent, right->parent->keys[i - 1]);
        break;
      }
    }
    updateKeys(left->parent);
    delete right;
  }

  void borrowFromLeftNeighbor(Node* left, Node* right) {
    right->keys.insert(right->keys.begin(), left->keys.back());
    left->keys.pop_back();

    if (right->is_leaf) {
      right->values.insert(right->values.begin(), left->values.back());
      left->values.pop_back();
    } else {
      Node* borrowed = left->childs.back();
      right->childs.insert(right->childs.begin(), borrowed);
      borrowed->parent = right;
      left->childs.pop_back();
    }
    recursiveUpdateKeys(right->parent);
  }

  void borrowFromRightNeighbor(Node* left, Node* right) {
    left->keys.push_back(right->keys.front());
    right->keys.erase(right->keys.begin());

    if (left->is_leaf) {
      left->values.push_back(right->values.front());
      right->values.erase(right->values.begin());
    } else {
      Node* borrowed = right->childs.front();
      left->childs.push_back(borrowed);
      borrowed->parent = left;
      right->childs.erase(right->childs.begin());
    }
    recursiveUpdateKeys(right->parent);
  }

  void updateRoot(Node* node) {
    if (node->keys.size() > 0) {
      return;
    }
    if (!node->is_leaf) {
      root_ = node->childs[0];
      root_->parent = nullptr;
    } else {
      root_ = nullptr;
    }
    delete node;
  }

  void recursiveUpdateKeys(Node* node) {
    while (node) {
      updateKeys(node);
      node = node->parent;
    }
  }

  void updateKeys(Node* node) {
    if (node == nullptr) {
      return;
    }
    for (size_t i = 0; i < node->keys.size(); ++i) {
      Key key = searchMinKey(node->childs[i + 1]);
      node->keys[i] = key;
    }
  }

  Key searchMinKey(Node* node) {
    while (!node->is_leaf) {
      node = node->childs[0];
    }
    return node->keys[0];
  }

  void split(Node* node) {
    Node* new_node = new Node;

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
      Node* new_root = new Node;
      new_root->is_leaf = false;
      new_root->keys.push_back(mid_key);
      new_root->childs.push_back(node);
      new_root->childs.push_back(new_node);
      root_ = new_root;
      node->parent = new_root;
      new_node->parent = new_root;
      return;
    }

    Node* parent = node->parent;

    auto mid_it = lower_bound(parent->keys.begin(), parent->keys.end(),
                              new_node->keys.front());
    size_t mid_index = mid_it - parent->keys.begin();
    parent->keys.push_back(mid_key);
    parent->childs.push_back(new_node);
    std::rotate(parent->keys.begin() + mid_index, parent->keys.end() - 1,
                parent->keys.end());
    std::rotate(parent->childs.begin() + mid_index + 1,
                parent->childs.end() - 1, parent->childs.end());

    if (parent->keys.size() > 2 * degree) {
      split(parent);
    }
  }

  void moveDataToNewNode(Node* src, Node* dst, size_t border) {
    if (src->is_leaf) {
      std::move(src->keys.begin() + border, src->keys.end(),
                std::back_inserter(dst->keys));
      std::move(src->values.begin() + border, src->values.end(),
                std::back_inserter(dst->values));
      src->keys.erase(src->keys.begin() + border, src->keys.end());
      src->values.erase(src->values.begin() + border, src->values.end());
    } else {
      std::move(src->keys.begin() + border + 1, src->keys.end(),
                std::back_inserter(dst->keys));
      src->keys.erase(src->keys.begin() + border, src->keys.end());
      std::move(src->childs.begin() + border + 1, src->childs.end(),
                std::back_inserter(dst->childs));
      for (auto& item : dst->childs) {
        item->parent = dst;
      }
      src->childs.erase(src->childs.begin() + border + 1, src->childs.end());
    }
  }

  bool removeKeyFromNode(Node* node, const Key& key) {
    auto it = lower_bound(node->keys.begin(), node->keys.end(), key);
    size_t index = it - node->keys.begin();
    if (it == node->keys.end() || node->keys.size() == 0) {
      return false;
    }

    node->keys.erase(it);
    if (node->is_leaf) {
      node->values.erase(node->values.begin() + index);
    } else {
      node->childs.erase(node->childs.begin() + index + 1);
    }
    return true;
  }

  void Clear(Node* node) {
    for (auto child : node->childs) {
      Clear(child);
    }
    delete node;
  }

 public:
  void printNode(Node* node) {
    std::cout << "========================================" << std::endl;
    std::cout << "Node: " << node << "(is leaf = " << node->is_leaf << ")"
              << std::endl;
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
    std::cout << "Left = " << node->left << ", Right = " << node->right
              << std::endl;
  }

  void printTree() { _printTree(root_); }

  void _printTree(Node* node, int depth = 0) {
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

}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_MODEL_BPLUSTREE_TREE_H_
