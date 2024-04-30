#ifndef TRANSACTIONS_SOURCE_MODEL_BPLUSTREE_B_PLUS_TREE_H_
#define TRANSACTIONS_SOURCE_MODEL_BPLUSTREE_B_PLUS_TREE_H_

#include <fstream>
#include <stdexcept>

#include "model/bplustree/tree.h"
#include "model/common/basestorage.h"

namespace s21 {

template <typename Key, typename Value,
          typename ValueEqual = std::equal_to<Value>>
class BPlusTree : public BaseStorage<Key, Value> {
 public:
  template <typename T>
  struct NewValue {
    int ttl;
    T value;
  };
  using new_value = NewValue<Value>;

  ~BPlusTree() = default;

  bool Set(const Key& key, const Value& value) {
    return tree.Insert(key, new_value{0, value});
  };

  Value Get(const Key& key) const {
    const new_value& value = tree.Search(key);
    return value.value;
  };

  bool Exists(const Key& key) const { return tree.Exists(key); };

  bool Del(const Key& key) { return tree.Remove(key); };

  bool Update(const Key& key, const Value& value) {
    try {
      const new_value& old = tree.Search(key);
      return tree.Update(key, new_value{old.ttl, value});
    } catch (const std::invalid_argument& err) {
      return false;
    }
  };

  std::vector<Key> Keys() const {
    std::vector<Key> keys;
    for (auto it = tree.Begin(); it != tree.End(); ++it) {
      keys.push_back((*it).first);
    }
    return keys;
  };

  bool Rename(const Key& key, const Key& new_key) {
    if (tree.Exists(new_key) || !tree.Exists(key)) {
      return false;
    }
    const new_value value = tree.Search(key);
    bool res = tree.Remove(key);
    if (!res) {
      return false;
    }
    return tree.Insert(new_key, value);
  };

  int Ttl(const Key& key) const {
    const new_value& value = tree.Search(key);
    return value.ttl;
  };

  std::vector<Key> Find(const Value& value) const {
    std::vector<Key> result;
    ValueEqual equal;

    for (auto it = tree.Begin(); it != tree.End(); ++it) {
      if (equal((*it).second.value, value)) {
        result.push_back((*it).first);
      }
    }

    return result;
  };

  std::vector<Value> Showall() const {
    std::vector<Value> values;
    for (auto it = tree.Begin(); it != tree.End(); ++it) {
      values.push_back((*it).second.value);
    }
    return values;
  };

  std::pair<bool, int> Upload(const std::string& path) {
    std::ifstream file(path);

    if (!file.is_open()) {
      return std::pair<bool, int>(false, 0);
    }

    Key key;
    Value value;
    int counter = 0;
    while (file >> key && file >> value) {
      if (Set(std::move(key), std::move(value))) {
        ++counter;
      }
    }

    return std::pair<bool, int>(true, counter);
  };

  std::pair<bool, int> Export(const std::string& path) const {
    std::ofstream file(path);

    if (!file.is_open()) {
      return std::pair<bool, int>(false, 0);
    }

    int counter = 0;
    for (auto it = tree.Begin(); it != tree.End(); ++it) {
      file << (*it).first << " " << (*it).second.value << std::endl;
      ++counter;
    }

    return std::pair<bool, int>(true, counter);
  };

  void Print() { tree.printTree(); }

 private:
  Tree<Key, new_value> tree;
};
}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_MODEL_BPLUSTREE_B_PLUS_TREE_H_
