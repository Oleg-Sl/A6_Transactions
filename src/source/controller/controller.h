#ifndef TRANSACTIONS_SOURCE_CONTROLLER_CONTROLLER_H_
#define TRANSACTIONS_SOURCE_CONTROLLER_CONTROLLER_H_

#include "model/common/base_class.h"
#include "model/common/data.h"

namespace s21 {

template <typename Key, typename Value>
class Controller {
 public:
  explicit Controller(BaseClass<Key, Value>&& model) : model_(model) {}

  bool Set(Key key, Value value, int validity = 0) const {
    return model_.Set(key, value, validity);
  }

  Value Get(Key key) const { return model_.Get(key); }

  bool Exists(Key key) const { return model_.Exists(key); }

  bool Del(Key key) const { return model_.Del(key); }

  bool Update(Key key, Value value) const { return model_.Update(key, value); }

  std::vector<Key> Keys() const { return model_.Keys(); }

  bool Rename(Key key, Key new_key) const {
    return model_.Rename(key, new_key);
  }

  int Ttl(Key param) const { return model_.Ttl(param); }

  std::vector<Key> Find(Value value) const { return model_.Find(value); }

  std::vector<Value> Showall() const { return model_.Showall(); }

  std::pair<bool, int> Upload(std::string path) { return model_.Upload(path); }

  std::pair<bool, int> Export(std::string path) const {
    return model_.Export(path);
  }

 private:
  BaseClass<Key, Value>& model_;
};

}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_CONTROLLER_CONTROLLER_H_