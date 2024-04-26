#ifndef TRANSACTIONS_SOURCE_CONTROLLER_CONTROLLER_H_
#define TRANSACTIONS_SOURCE_CONTROLLER_CONTROLLER_H_

#include "model/common/basestorage.h"
#include "model/common/managerttl.h"

namespace s21 {

template <typename Key, typename Value>
class Controller {
 public:
  explicit Controller(BaseStorage<Key, Value>& model) : manager_(model) {}

  bool Set(Key key, Value value, int ttl = 0) {
    manager_.AddRecord(typename ManagerTTL<Key, Value>::Record{key, ttl});
    return manager_.ExecuteStorageOperation(&BaseStorage<Key, Value>::Set, key,
                                            value, ttl);
  }

  Value Get(Key key) {
    return manager_.ExecuteStorageOperation(&BaseStorage<Key, Value>::Get, key);
  }

  bool Exists(Key key) {
    return manager_.ExecuteStorageOperation(&BaseStorage<Key, Value>::Exists,
                                            key);
  }

  bool Del(Key key) {
    manager_.DeleteRecord(key);
    return manager_.ExecuteStorageOperation(&BaseStorage<Key, Value>::Del, key);
  }

  bool Update(Key key, Value value) {
    return manager_.ExecuteStorageOperation(&BaseStorage<Key, Value>::Update,
                                            key, value);
  }

  std::vector<Key> Keys() {
    return manager_.ExecuteStorageOperation(&BaseStorage<Key, Value>::Keys);
  }

  bool Rename(Key key, Key new_key) {
    manager_.RenameRecord(key, new_key);
    return manager_.ExecuteStorageOperation(&BaseStorage<Key, Value>::Rename,
                                            key, new_key);
  }

  int Ttl(Key param) {
    return manager_.ExecuteStorageOperation(&BaseStorage<Key, Value>::Ttl,
                                            param);
  }

  std::vector<Key> Find(Value value) {
    return manager_.ExecuteStorageOperation(&BaseStorage<Key, Value>::Find,
                                            value);
  }

  std::vector<Value> Showall() {
    return manager_.ExecuteStorageOperation(&BaseStorage<Key, Value>::Showall);
  }

  std::pair<bool, int> Upload(std::string path) {
    return manager_.ExecuteStorageOperation(&BaseStorage<Key, Value>::Upload,
                                            path);
  }

  std::pair<bool, int> Export(std::string path) {
    return manager_.ExecuteStorageOperation(&BaseStorage<Key, Value>::Export,
                                            path);
  }

  void StartManagerLoop(std::chrono::seconds sleep_time) {
    manager_.StartManagerLoop(sleep_time);
  }

  void StopManagerLoop() { manager_.StopManagerLoop(); }

 private:
  ManagerTTL<Key, Value> manager_;
};

}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_CONTROLLER_CONTROLLER_H_