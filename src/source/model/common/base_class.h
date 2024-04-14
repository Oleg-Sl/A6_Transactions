#ifndef TRANSACTIONS_SOURCE_MODEL_COMMON_BASE_CLASS_H_
#define TRANSACTIONS_SOURCE_MODEL_COMMON_BASE_CLASS_H_

#include <string>
#include <vector>

#include "data.h"

namespace s21 {
template <typename Key, typename Value>
class BaseClass {
 public:
  virtual ~BaseClass() = default;
  virtual bool Set(const Key& key, const Value& value, int validity = 0) = 0;
  virtual Value Get(const Key& key) const = 0;
  virtual bool Exists(const Key& key) const = 0;
  virtual bool Del(const Key& key) = 0;
  virtual bool Update(const Key& key, const Value& value) = 0;
  virtual std::vector<Key> Keys() const = 0;
  virtual bool Rename(const Key& key, const Key& new_key) = 0;
  virtual int Ttl(const Key& param) const = 0;
  virtual std::vector<Key> Find(const Value& value) const = 0;
  virtual std::vector<Value> Showall() const = 0;
  virtual std::pair<bool, int> Upload(const std::string& path) = 0;
  virtual std::pair<bool, int> Export(const std::string& path) const = 0;
};
}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_MODEL_COMMON_BASE_CLASS_H_