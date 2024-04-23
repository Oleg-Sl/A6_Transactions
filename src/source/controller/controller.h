#ifndef TRANSACTIONS_SOURCE_CONTROLLER_CONTROLLER_H_
#define TRANSACTIONS_SOURCE_CONTROLLER_CONTROLLER_H_

#include "model/common/base_class.h"
#include "model/common/managerttl.h"

namespace s21 {

template <typename Key, typename Value>
class Controller {
 public:
  explicit Controller(BaseClass<Key, Value>& model) : manager_(model) {}

  bool Set(Key key, Value value, int ttl = 0) {
    if (ttl != 0) {
      manager_.AddRecord(typename ManagerTTL<Key, Value>::Record{
          key, ttl, std::chrono::steady_clock::now()});
    }
    return manager_.Execute(&BaseClass<Key, Value>::Set, key, value, ttl);
  }

  Value Get(Key key) {
    return manager_.Execute(&BaseClass<Key, Value>::Get, key);
  }

  bool Exists(Key key) {
    return manager_.Execute(&BaseClass<Key, Value>::Exists, key);
  }

  bool Del(Key key) {
    return manager_.Execute(&BaseClass<Key, Value>::Del, key);
  }

  bool Update(Key key, Value value) {
    return manager_.Execute(&BaseClass<Key, Value>::Update, key, value);
  }

  std::vector<Key> Keys() {
    return manager_.Execute(&BaseClass<Key, Value>::Keys);
  }

  bool Rename(Key key, Key new_key) {
    return manager_.Execute(&BaseClass<Key, Value>::Rename, key, new_key);
  }

  int Ttl(Key param) {
    manager_.DeleteExpiredRecords();
    return manager_.Execute(&BaseClass<Key, Value>::Ttl, param);
  }

  std::vector<Key> Find(Value value) {
    return manager_.Execute(&BaseClass<Key, Value>::Find, value);
  }

  std::vector<Value> Showall() {
    return manager_.Execute(&BaseClass<Key, Value>::Showall);
  }

  std::pair<bool, int> Upload(std::string path) {
    return manager_.Execute(&BaseClass<Key, Value>::Upload, path);
  }

  std::pair<bool, int> Export(std::string path) {
    return manager_.Execute(&BaseClass<Key, Value>::Export, path);
  }

 private:
  ManagerTTL<Key, Value> manager_;
};

}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_CONTROLLER_CONTROLLER_H_