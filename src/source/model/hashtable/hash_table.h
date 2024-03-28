#ifndef TRANSACTIONS_SOURCE_MODEL_HASHTABLE_HASH_TABLE_H_
#define TRANSACTIONS_SOURCE_MODEL_HASHTABLE_HASH_TABLE_H_

#include <model/common/base_class.h>
#include <model/common/data.h>

#include <array>
#include <chrono>
#include <functional>
#include <vector>

namespace s21 {
const size_t kDefaultSize = 8;

template <typename T, typename Hasher = std::hash<std::string>>
class HashTable : BaseClass {
  struct Node {
    std::string key;
    T value;
    std::chrono::steady_clock::time_point create_time;
  };

 public:
  HashTable() {}

  bool Set(std::string key, T value, int validity = 0) {
    data_.at(GetIndex(key))
        .push_back(Node{key, value, std::chrono::steady_clock::now()});

    return 0;
  }

  T Get(std::string key) { return data_.at(GetIndex(key))[0].value; }

  bool Exists(std::string key) {}
  bool Del(std::string key) {}
  bool Update(std::string key, T value) {}
  std::vector<std::string> Keys() {}
  bool Rename(std::string key, std::string new_key) {}
  std::string Ttl(std::string param) {}
  std::vector<std::string> Find(T value) {}
  std::vector<T> Showall() {}
  std::pair<bool, int> Upload(std::string path) {}
  std::pair<bool, int> Export(std::string path) {}

 private:
  const Hasher hasher_;
  size_t table_size_ = kDefaultSize;
  std::array<std::vector<Node>, kDefaultSize> data_;

  size_t GetIndex(std::string value) { return hasher_(value) % table_size_; }
};
}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_MODEL_HASHTABLE_HASH_TABLE_H_