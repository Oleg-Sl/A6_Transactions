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

  struct Position {
    size_t hash;
    size_t index;
    bool is_valid = true;
  };

  struct Node {
    std::string key;
    T value;
    int TTL;
    time_type create_time;
  };

  HashTable() {}

  bool Set(std::string key, T value, int TTL = 0) {
    data_[GetHash(key)].push_back(
        Node{key, value, TTL, std::chrono::steady_clock::now()});

    return 0;
  }

  T Get(std::string key) {
    Position pos = GetNodePosition(key);
    if (pos.is_valid) {
      return (data_[pos.hash][pos.index]).value;
    }

    throw std::invalid_argument("Key is not exists");
  }

  bool Exists(std::string key) { return GetNodePosition(key).is_valid; }

  bool Del(std::string key) {
    Position pos = GetNodePosition(key);
    if (pos.is_valid) {
      data_[pos.hash].erase(data_[pos.hash].begin() + pos.index);
      return true;
    }

    return false;
  }

  bool Update(std::string key, T value) {
    Position pos = GetNodePosition(key);
    if (pos.is_valid) {
      data_[pos.hash][pos.index].value = value;
      return true;
    }

    return false;
  }

  std::vector<std::string> Keys() {}

  bool Rename(std::string key, std::string new_key) {}

  std::string Ttl(std::string key) {
    auto response_time = std::chrono::steady_clock::now();
    Position pos = GetNodePosition(key);

    if (pos.is_valid) {
      Node node = data_[pos.hash][pos.index];
      return std::to_string(node.TTL -
                            std::chrono::duration_cast<std::chrono::seconds>(
                                response_time - node.create_time)
                                .count());
    }

    return "(null)";
  }

  std::vector<std::string> Find(T value) {}

  std::vector<T> Showall() {
    auto response_time = std::chrono::steady_clock::now();
    std::vector<T> result;

    size_t counter_1 = 0;
    for (auto& array : data_) {
      size_t counter_2 = 0;
      for (auto& elem : array) {
        if (!CheckTTLExpired(response_time,
                             Position{counter_1, counter_2, true})) {
          result.push_back(elem.value);
        }
        ++counter_2;
      }
      ++counter_1;
    }

    return result;
  }

  std::pair<bool, int> Upload(std::string path) {}

  std::pair<bool, int> Export(std::string path) {}

 private:
  const Hasher hasher_;
  size_t table_size_ = kDefaultSize;
  std::array<std::vector<Node>, kDefaultSize> data_;

  size_t GetHash(std::string value) { return hasher_(value) % table_size_; }

  bool CheckTTLExpired(const time_type& response_time,
                       const Position& elem_pos) {
    Node element = data_[elem_pos.hash][elem_pos.index];

    return std::chrono::duration_cast<std::chrono::seconds>(response_time -
                                                            element.create_time)
                       .count() > element.TTL
               ? true
               : false;
  }

  Position GetNodePosition(const std::string& key) {
    auto response_time = std::chrono::steady_clock::now();

    size_t hash = GetHash(key);
    for (size_t index = 0; index < data_[hash].size(); ++index) {
      Node node = data_[hash][index];
      if (node.key == key &&
          !CheckTTLExpired(response_time, Position{hash, index, true})) {
        return Position{hash, index, true};
      }
    }
    return Position{0, 0, false};
  }
};
}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_MODEL_HASHTABLE_HASH_TABLE_H_