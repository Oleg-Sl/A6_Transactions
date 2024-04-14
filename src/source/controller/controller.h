#ifndef TRANSACTIONS_SOURCE_CONTROLLER_CONTROLLER_H_
#define TRANSACTIONS_SOURCE_CONTROLLER_CONTROLLER_H_

// #include "model/bst/self_balancing_binary_search_tree.h"
#include <memory>

#include "model/common/base_class.h"
#include "model/common/data.h"

namespace s21 {

template <typename Key, typename Value>
class Controller {
 public:
  explicit Controller(BaseClass<Key, Value>&& model) : model_(model) {}

  bool Set(Key key, Value value, int validity = 0) {
    return model_.Set(key, value, validity);
  }

  Value Get(Key key) { return model_.Get(key); }

  bool Exists(Key key) { return model_.Exists(key); }

  bool Del(Key key) { return model_.Del(key); }

  bool Update(Key key, Value value) { return model_.Update(key, value); }

  std::vector<Key> Keys() { return model_.Keys(); }

  bool Rename(Key key, Key new_key) { return model_.Rename(key, new_key); }

  int Ttl(Key param) { return model_.Ttl(param); }

  std::vector<Key> Find(Value value) { return model_.Find(value); }

  std::vector<Value> Showall() { return model_.Showall(); }

  std::pair<bool, int> Upload(std::string path) { return model_.Upload(path); }

  std::pair<bool, int> Export(std::string path) { return model_.Export(path); }

 private:
  BaseClass<Key, Value>& model_;
};

}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_CONTROLLER_CONTROLLER_H_