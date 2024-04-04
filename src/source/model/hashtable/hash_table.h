#ifndef TRANSACTIONS_SOURCE_MODEL_HASHTABLE_HASH_TABLE_H_
#define TRANSACTIONS_SOURCE_MODEL_HASHTABLE_HASH_TABLE_H_

#include <model/common/base_class.h>
#include <model/common/data.h>

#include <array>
#include <chrono>
#include <functional>
#include <list>
#include <stdexcept>

namespace s21 {

template <typename T, typename Hasher = std::hash<std::string>>
class HashTable : BaseClass {
 public:
  using time_type = std::chrono::steady_clock::time_point;

  static constexpr size_t kDefaultSize = 1;
  static constexpr float kResizeCoeff = 0.75;
  static constexpr size_t kScaleCoeff = 2;

  struct Node {
    std::string key;
    T value;
    int TTL;
    time_type create_time;
    int cached;
  };

  enum class StatusSet { kElementAlreadyExists, kOk };

  HashTable() {
    bucket_pointers_ = allocator.allocate(kDefaultSize);

    for (std::size_t i = 0; i < table_size_; ++i) {
      std::allocator_traits<decltype(allocator)>::construct(
          allocator, &bucket_pointers_[i], data_.end());
    }
  }

  bool Set(std::string key, T value, int TTL = 0) {
    if (data_.size() / table_size_ >= kResizeCoeff) {
      Resize();
    }

    int hash = GetHash(key, table_size_);
    auto pos = GetNodePosition(key, hash);
    if (pos != data_.end()) {
      return static_cast<bool>(StatusSet::kElementAlreadyExists);
    }

    bucket_pointers_[hash] = data_.insert(
        bucket_pointers_[hash],
        Node{key, value, TTL, std::chrono::steady_clock::now(), hash});

    return static_cast<bool>(StatusSet::kOk);
  }

  T Get(std::string key) {
    auto pos = GetNodePosition(key, GetHash(key, table_size_));
    if (pos != data_.end()) {
      return (*pos).value;
    }

    throw std::invalid_argument("Key is not exists");
  }

  bool Exists(std::string key) {
    return GetNodePosition(key, GetHash(key, table_size_)) != data_.end();
  }

  bool Del(std::string key) {
    // Position pos = GetNodePosition(key);
    // if (pos.is_valid) {
    //   data_[pos.hash].erase(data_[pos.hash].begin() + pos.index);
    //   return true;
    // }

    // return false;
  }

  bool Update(std::string key, T value) {
    // Position pos = GetNodePosition(key);
    // if (pos.is_valid) {
    //   data_[pos.hash][pos.index].value = value;
    //   return true;
    // }

    // return false;
  }

  std::vector<std::string> Keys() {
    // auto response_time = std::chrono::steady_clock::now();
    // std::vector<std::string> result;

    // size_t counter_1 = 0;
    // for (auto& array : data_) {
    //   if (!TTLIsExpired(response_time, Position{counter_1, 0, true})) {
    //     result.push_back(array[0].key);
    //   }
    //   ++counter_1;
    // }

    // return result;
  }

  bool Rename(std::string key, std::string new_key) {
    // try {
    //   Set(new_key, Get(key));
    // } catch (const std::invalid_argument& ex) {
    //   return false;
    // }
    // if (!Del(key)) {
    //   return false;
    // }

    // return true;
  }

  std::string Ttl(std::string key) {
    // auto response_time = std::chrono::steady_clock::now();
    // Position pos = GetNodePosition(key);

    // if (pos.is_valid) {
    //   Node node = data_[pos.hash][pos.index];
    //   return std::to_string(node.TTL -
    //                         std::chrono::duration_cast<std::chrono::seconds>(
    //                             response_time - node.create_time)
    //                             .count());
    // }

    // return "(null)";
  }

  std::vector<std::string> Find(T value) {
    // auto response_time = std::chrono::steady_clock::now();
    // std::vector<std::string> result;

    // size_t counter_1 = 0;
    // for (auto& array : data_) {
    //   size_t counter_2 = 0;
    //   for (auto& elem : array) {
    //     // if (!TTLIsExpired(response_time,
    //     //                      Position{counter_1, counter_2, true}) &&
    //     //     elem.value == value) { // TODO: Add operator== to Student
    //     //   result.push_back(elem.value);
    //     // }
    //     ++counter_2;
    //   }
    //   ++counter_1;
    // }

    // return result;
  }

  std::vector<T> Showall() {
    // auto response_time = std::chrono::steady_clock::now();
    // std::vector<T> result;

    // size_t counter_1 = 0;
    // for (auto& array : data_) {
    //   size_t counter_2 = 0;
    //   for (auto& elem : array) {
    //     if (!TTLIsExpired(response_time,
    //                          Position{counter_1, counter_2, true})) {
    //       result.push_back(elem.value);
    //     }
    //     ++counter_2;
    //   }
    //   ++counter_1;
    // }

    // return result;
  }

  std::pair<bool, int> Upload(std::string path) {}

  std::pair<bool, int> Export(std::string path) {}

  std::size_t GetSize() { return table_size_; }

  std::size_t GetLoadFactor() { return data_.size(); }

 private:
  size_t table_size_ = kDefaultSize;
  std::list<Node> data_;
  std::allocator<typename std::list<Node>::iterator> allocator;
  typename std::list<Node>::iterator* bucket_pointers_;
  const Hasher hasher_;

  size_t GetHash(std::string value, size_t size) {
    if (size == 1) return 0;
    return hasher_(value) % (size - 1);
  }

  bool TTLIsExpired(const time_type& response_time,
                    typename std::list<Node>::const_iterator elem_pos) {
    bool is_valid_node = elem_pos != data_.end();
    bool TTL_id_expired = std::chrono::duration_cast<std::chrono::seconds>(
                              response_time - (*elem_pos).create_time)
                              .count() > (*elem_pos).TTL;

    return !is_valid_node || TTL_id_expired;
  }

  typename std::list<Node>::iterator GetNodePosition(std::string key,
                                                     int hash) {
    auto response_time = std::chrono::steady_clock::now();

    for (auto it = bucket_pointers_[hash];
         it != data_.end() && (*it).cached == hash; ++it) {
      Node node = *it;
      if (node.key == key && !TTLIsExpired(response_time, it)) {
        return it;
      }
    }
    return data_.end();
  }

  void Resize() {
    int new_table_size = table_size_ * kScaleCoeff;
    typename std::list<Node>::iterator* new_bucket_pointers =
        allocator.allocate(new_table_size);

    for (std::size_t i = 0; i < new_table_size; ++i) {
      std::allocator_traits<decltype(allocator)>::construct(
          allocator, &new_bucket_pointers[i], data_.end());
    }

    for (auto it = data_.begin(); it != data_.end(); ++it) {
      new_bucket_pointers[GetHash((*it).key, new_table_size)] = it;
    }

    allocator.deallocate(bucket_pointers_, table_size_);
    bucket_pointers_ = new_bucket_pointers;
    table_size_ = new_table_size;
  }
};
}  // namespace s21
#endif  // TRANSACTIONS_SOURCE_MODEL_HASHTABLE_HASH_TABLE_H_
