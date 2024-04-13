#ifndef TRANSACTIONS_SOURCE_MODEL_COMMON_BASE_CLASS_H_
#define TRANSACTIONS_SOURCE_MODEL_COMMON_BASE_CLASS_H_

#include <string>
#include <vector>

#include "data.h"

namespace s21 {
class BaseClass {
 public:
  virtual ~BaseClass() = default;
  virtual bool Set(const std::string& key, const Student& student,
                   int validity = 0) = 0;
  virtual Student Get(const std::string& key) const = 0;
  virtual bool Exists(const std::string& key) const = 0;
  virtual bool Del(const std::string& key) = 0;
  virtual bool Update(const std::string& key, const Student& student) = 0;
  virtual std::vector<std::string> Keys() const = 0;
  virtual bool Rename(const std::string& key, const std::string& new_key) = 0;
  virtual int Ttl(const std::string& param) const = 0;
  virtual std::vector<std::string> Find(const Student& student) const = 0;
  virtual std::vector<Student> Showall() const = 0;
  virtual std::pair<bool, int> Upload(const std::string& path) = 0;
  virtual std::pair<bool, int> Export(const std::string& path) const = 0;
};
}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_MODEL_COMMON_BASE_CLASS_H_