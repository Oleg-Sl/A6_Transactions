#ifndef TRANSACTIONS_SOURCE_MODEL_BST_SELF_BALANCING_BINARY_SEARCH_TREE_H_
#define TRANSACTIONS_SOURCE_MODEL_BST_SELF_BALANCING_BINARY_SEARCH_TREE_H_

#include <fstream>

#include "model/common/base_class.h"
#include "model/common/data.h"

namespace s21 {

enum Color {
  Black,
  Red,
};

class BSTNode {
 public:
  BSTNode* parent = nullptr;
  BSTNode* link[2] = {nullptr, nullptr};
  Data data;
  bool color = Color::Black;

  BSTNode() = default;
  explicit BSTNode(const Data& value) : data(value) { color = Color::Red; };
};

class SelfBalancingBinarySearchTree : public BaseClass<std::string, Student> {
 public:
  class BSTIterator;
  using Node = BSTNode;
  using Pointer = Node*;
  using Iterator = BSTIterator;
  using ValueType = Data;
  Pointer leaf_;

  SelfBalancingBinarySearchTree();
  ~SelfBalancingBinarySearchTree();

  // internal class BSTIterator
  class BSTIterator {
    friend class SelfBalancingBinarySearchTree;

   public:
    BSTIterator(Pointer node, Pointer leaf) : current_(node), leaf_(leaf) {}
    BSTIterator() : current_(nullptr), leaf_(nullptr) {}
    BSTIterator(const BSTIterator& obj) {
      current_ = obj.current_;
      leaf_ = obj.leaf_;
    }

    BSTIterator& operator=(const BSTIterator& obj) noexcept {
      current_ = obj.current_;
      leaf_ = obj.leaf_;
      return *this;
    }

    BSTIterator& operator--() noexcept {
      if (current_->link[0] != leaf_) {
        current_ = current_->link[0];
        while (current_->link[1] != leaf_) {
          current_ = current_->link[1];
        }
      } else {
        Pointer tmp = current_->parent;
        while (tmp && tmp->data.GetKey() > current_->data.GetKey()) {
          current_ = tmp;
          tmp = tmp->parent;
        }
        current_ = tmp;
      }
      return *this;
    }

    BSTIterator& operator++() noexcept {
      if (current_->link[1] != leaf_) {
        current_ = current_->link[1];
        while (current_->link[0] != leaf_) {
          current_ = current_->link[0];
        }
      } else {
        Pointer tmp = current_->parent;
        while (tmp && tmp->data.GetKey() < current_->data.GetKey()) {
          current_ = tmp;
          tmp = tmp->parent;
        }
        current_ = tmp;
      }
      return *this;
    }

    bool operator!=(const BSTIterator& other) const noexcept {
      return (current_ != other.current_);
    }

    bool IsValid() const { return current_ != nullptr; }

    ValueType* operator->() noexcept { return &(current_->data); }

   private:
    Pointer current_;
    Pointer leaf_;
  };

  bool Set(const std::string& key, const Student& student, int validity = 0);
  Student Get(const std::string& key) const ;
  bool Exists(const std::string& key) const;
  bool Del(const std::string& key);
  bool Update(const std::string& key, const Student& student);
  std::vector<std::string> Keys() const;
  bool Rename(const std::string& key, const std::string& new_key);
  int Ttl(const std::string& param) const;
  std::vector<std::string> Find(const Student& student) const;
  std::vector<Student> Showall() const;
  std::pair<bool, int> Upload(const std::string& path);
  std::pair<bool, int> Export(const std::string& path) const;

 private:
  Pointer root_;
  void Swap(Pointer a, Pointer b);
  void LeftRotation(Pointer node);
  void RightRotation(Pointer node);
  void BalanceTree(Pointer node);
  Iterator Begin();
  Iterator End();
  void Clear();
  int CountChildren(Pointer node) const;
  Pointer GetChild(Pointer node) const;
  Pointer GetParent(Pointer node) const;
  Pointer Search(const std::string& key) const;
  bool RemoveNode(Pointer node);
  void RemoveNodeWithoutChildren(Pointer node);
  void RemoveNodeWithOneChild(Pointer node);
  void RemoveNodeWithTwoChild(Pointer node);
  void RebalanceTree(Pointer node);
  bool NodeIsLeftChild(Pointer node);
  Pointer MinValueFromRight(const Pointer node) const;
  void ExcludeNode(Pointer a, Pointer b);
  bool CompareValue(Student student, Student search_template);
  bool ParseLine(std::string line);
};

}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_MODEL_BST_SELF_BALANCING_BINARY_SEARCH_TREE_H_