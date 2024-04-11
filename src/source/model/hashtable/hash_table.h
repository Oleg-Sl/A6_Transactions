#ifndef TRANSACTIONS_SOURCE_MODEL_HASHTABLE_HASH_TABLE_H_
#define TRANSACTIONS_SOURCE_MODEL_HASHTABLE_HASH_TABLE_H_

#include <model/common/base_class.h>
#include <model/common/data.h>

#include <chrono>
#include <fstream>
#include <list>
#include <stdexcept>

namespace s21 {

template <typename Key, typename Value,
          typename ValueEqual = std::equal_to<Value>,
          typename Hasher = std::hash<Key>>
class HashTable : public BaseClass {
 public:
  using time_type = std::chrono::steady_clock::time_point;

  static constexpr size_t kDefaultSize = 1;
  static constexpr float kResizeCoeff = 0.75;
  static constexpr size_t kScaleCoeff = 2;

  struct Node {
    Key key;
    Value value;
    int TTL;
    time_type create_time;
    int cached;
  };

  HashTable() {
    bucket_pointers_ = allocator_.allocate(kDefaultSize);

    for (std::size_t i = 0; i < table_size_; ++i) {
      std::allocator_traits<decltype(allocator_)>::construct(
          allocator_, &bucket_pointers_[i], data_.end());
    }
  }

  ~HashTable() override {
    allocator_.deallocate(bucket_pointers_, table_size_);
  }

  bool Set(const Key& key, const Value& value, int TTL = 0) override {
    auto response_time = std::chrono::steady_clock::now();

    if (data_.size() / table_size_ >= kResizeCoeff) {
      Resize();
    }

    int hash = GetHash(key, table_size_);
    auto pos = GetNodePosition(key, hash);
    if (pos != data_.end()) {
      return false;
    }

    if (bucket_pointers_[hash] != data_.end()) {
      bucket_pointers_[hash] = data_.insert(
          bucket_pointers_[hash], Node{key, value, TTL, response_time, hash});
    } else {
      data_.push_back(Node{key, value, TTL, response_time, hash});
      bucket_pointers_[hash] = std::prev(data_.end());
    }

    return true;
  }

  Value Get(const Key& key) const override {
    auto pos = GetNodePosition(key, GetHash(key, table_size_));
    if (pos != data_.end()) {
      return pos->value;
    }

    throw std::invalid_argument("Key is not exists");
  }

  bool Exists(const Key& key) const override {
    return GetNodePosition(key, GetHash(key, table_size_)) != data_.end();
  }

  bool Del(const Key& key) override {
    auto pos = GetNodePosition(key, GetHash(key, table_size_));
    if (pos != data_.end()) {
      bucket_pointers_[pos->cached] =
          std::next(pos)->cached == std::next(pos)->cached ? (std::next(pos))
                                                           : data_.end();
      data_.erase(pos);
      return true;
    }

    return false;
  }

  bool Update(const Key& key, const Value& value) override {
    auto pos = GetNodePosition(key, GetHash(key, table_size_));
    if (pos != data_.end()) {
      pos->value = value;
      return true;
    }

    return false;
  }

  std::vector<Key> Keys() const override {
    auto response_time = std::chrono::steady_clock::now();
    std::vector<Key> result;

    for (auto it = data_.begin(); it != data_.end(); ++it) {
      if (!TTLIsExpired(response_time, it)) {
        result.push_back(it->key);
      }
    }

    return result;
  }

  bool Rename(const Key& key, const Key& new_key) override {
    auto pos = GetNodePosition(key, GetHash(key, table_size_));
    auto pos2 = GetNodePosition(new_key, GetHash(new_key, table_size_));
    if (pos != data_.end() && pos2 == data_.end()) {
      int new_key_hash = GetHash(new_key, table_size_);
      Node new_node = *pos;
      new_node.cached = new_key_hash;
      new_node.key = new_key;
      if (bucket_pointers_[new_key_hash] == data_.end()) {
        data_.push_back(new_node);
        bucket_pointers_[new_key_hash] = std::prev(data_.end());
      } else {
        bucket_pointers_[new_key_hash] =
            data_.insert(bucket_pointers_[new_key_hash], new_node);
      }
      Del(key);

      return true;
    }

    return false;
  }

  int Ttl(const Key& key) const override {
    auto response_time = std::chrono::steady_clock::now();
    auto pos = GetNodePosition(key, GetHash(key, table_size_));

    if (pos != data_.end()) {
      return pos->TTL - std::chrono::duration_cast<std::chrono::seconds>(
                            response_time - pos->create_time)
                            .count();
    }

    throw std::invalid_argument("Key is not exists");
  }

  std::vector<Key> Find(const Value& value) const override {
    auto response_time = std::chrono::steady_clock::now();
    std::vector<Key> result;
    ValueEqual equal;

    for (auto it = data_.begin(); it != data_.end(); ++it) {
      if (!TTLIsExpired(response_time, it) && equal(value, it->value)) {
        result.push_back(it->key);
      }
    }

    return result;
  }

  std::vector<Value> Showall() const override {
    auto response_time = std::chrono::steady_clock::now();
    std::vector<Value> result;

    for (auto it = data_.begin(); it != data_.end(); ++it) {
      if (!TTLIsExpired(response_time, it)) {
        result.push_back(it->value);
      }
    }

    return result;
  }

  std::pair<bool, int> Upload(const std::string& path) override {
    std::ifstream file(path);
    Key key;
    Value value;

    if (!file.is_open()) {
      return std::pair<bool, int>(false, 0);
    }

    int counter = 0;
    while (file >> key && file >> value) {
      if (Set(key, value)) {
        ++counter;
      }
    }

    return std::pair<bool, int>(true, counter);
  }

  std::pair<bool, int> Export(const std::string& path) const override {
    auto response_time = std::chrono::steady_clock::now();
    std::ofstream file(path);

    if (!file.is_open()) {
      return std::pair<bool, int>(false, 0);
    }

    int counter = 0;
    for (auto it = data_.begin(); it != data_.end(); ++it) {
      if (!TTLIsExpired(response_time, it)) {
        file << it->key << " " << it->value << std::endl;
        ++counter;
      }
    }

    return std::pair<bool, int>(true, counter);
  }

  std::size_t GetSize() const { return table_size_; }

  std::size_t GetLoadFactor() const { return data_.size(); }

 private:
  size_t table_size_ = kDefaultSize;
  std::list<Node> data_;
  std::allocator<typename std::list<Node>::iterator> allocator_;
  typename std::list<Node>::iterator* bucket_pointers_;
  const Hasher hasher_{};

  size_t GetHash(const Key& value, size_t size) const {
    return size == 1 ? 0 : hasher_(value) % (size - 1);
  }

  bool TTLIsExpired(const time_type& response_time,
                    typename std::list<Node>::const_iterator elem_pos) const {
    bool is_valid_node = elem_pos != data_.end();
    bool TTL_is_expired = std::chrono::duration_cast<std::chrono::milliseconds>(
                              response_time - elem_pos->create_time)
                                  .count() > 1000 * elem_pos->TTL &&
                          elem_pos->TTL != 0;

    return !is_valid_node || TTL_is_expired;
  }

  typename std::list<Node>::const_iterator GetNodePosition(const Key& key,
                                                           int hash) const {
    auto response_time = std::chrono::steady_clock::now();

    for (auto it = bucket_pointers_[hash];
         it != data_.end() && it->cached == hash; ++it) {
      Node node = *it;
      if (node.key == key && !TTLIsExpired(response_time, it)) {
        return it;
      }
    }

    return data_.end();
  }

  typename std::list<Node>::iterator GetNodePosition(const Key& key, int hash) {
    auto response_time = std::chrono::steady_clock::now();

    for (auto it = bucket_pointers_[hash];
         it != data_.end() && it->cached == hash; ++it) {
      Node node = *it;
      if (node.key == key && !TTLIsExpired(response_time, it)) {
        return it;
      }
    }

    return data_.end();
  }

  void Resize() {
    size_t new_table_size = table_size_ * kScaleCoeff;
    typename std::list<Node>::iterator* new_bucket_pointers =
        allocator_.allocate(new_table_size);

    for (std::size_t i = 0; i < new_table_size; ++i) {
      std::allocator_traits<decltype(allocator_)>::construct(
          allocator_, &new_bucket_pointers[i], data_.end());
    }

    for (auto it = data_.begin(); it != data_.end(); ++it) {
      new_bucket_pointers[GetHash(it->key, new_table_size)] = it;
    }

    allocator_.deallocate(bucket_pointers_, table_size_);
    bucket_pointers_ = new_bucket_pointers;
    table_size_ = new_table_size;
  }
};
}  // namespace s21
#endif  // TRANSACTIONS_SOURCE_MODEL_HASHTABLE_HASH_TABLE_H_
