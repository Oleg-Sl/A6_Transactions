#ifndef TRANSACTIONS_SOURCE_MODEL_HASHTABLE_HASH_TABLE_H_
#define TRANSACTIONS_SOURCE_MODEL_HASHTABLE_HASH_TABLE_H_

#include <model/common/base_class.h>
#include <model/common/data.h>

#include <array>
#include <chrono>
#include <functional>
#include <stdexcept>
#include <vector>

namespace s21 {

template <typename T, typename Hasher = std::hash<std::string>>
class HashTable : BaseClass {
 public:
  using time_type = std::chrono::steady_clock::time_point;

  static const size_t kDefaultSize = 8;

  struct Node {
    std::string key;
    T value;
    int TTL;
    time_type create_time;
    bool valid = false;

    Node(std::string key, T value, int TTL, time_type create_time)
        : key(key),
          value(value),
          TTL(TTL),
          create_time(create_time),
          valid(true) {}

    Node() {}
  };

  HashTable() {}

  bool Set(std::string key, T value, int TTL = 0) {
    data_.at(GetIndex(key))
        .push_back(Node{key, value, TTL, std::chrono::steady_clock::now()});

    return 0;
  }

  T Get(std::string key) {
    Node node = GetNode(key);
    if (node.valid) {
      return node.value;
    }

    throw std::invalid_argument("Key is not exists");
  }

  bool Exists(std::string key) { return GetNode(key).valid; }

  bool Del(std::string key) {}

  bool Update(std::string key, T value) {}

  std::vector<std::string> Keys() {}

  bool Rename(std::string key, std::string new_key) {}

  std::string Ttl(std::string key) {
    auto response_time = std::chrono::steady_clock::now();
    Node node = GetNode(key);

    if (node.valid) {
      return std::to_string(node.TTL -
                            std::chrono::duration_cast<std::chrono::seconds>(
                                response_time - node.create_time)
                                .count());
    }

    return "(null)";
  }

  std::vector<std::string> Find(T value) {}

  std::vector<T> Showall() {}

  std::pair<bool, int> Upload(std::string path) {}

  std::pair<bool, int> Export(std::string path) {}

 private:
  const Hasher hasher_;
  size_t table_size_ = kDefaultSize;
  std::array<std::vector<Node>, kDefaultSize> data_;

  size_t GetIndex(std::string value) { return hasher_(value) % table_size_; }

  bool CheckTTLExpired(const time_type& response_time, const Node& element) {
    return std::chrono::duration_cast<std::chrono::seconds>(response_time -
                                                            element.create_time)
                       .count() > element.TTL
               ? true
               : false;
  }

  Node GetNode(const std::string& key) {
    auto response_time = std::chrono::steady_clock::now();

    for (auto const& node : data_.at(GetIndex(key))) {
      if (node.key == key && !CheckTTLExpired(response_time, node)) {
        return node;
      }
    }

    return Node();
  }
};
}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_MODEL_HASHTABLE_HASH_TABLE_H_