#ifndef TRANSACTIONS_COMMON_BASE_CLASS_H_
#define TRANSACTIONS_COMMON_BASE_CLASS_H_
#include <string>
#include <vector>

#include "data.h"

namespace s21 {
class BaseClass {
 public:
  virtual bool Set(Data data) = 0;
  virtual Student Get(std::string key) = 0;
  virtual bool Exists(std::string key) = 0;
  virtual bool Del(std::string key) = 0;
  virtual bool Update(std::string key, Student student) = 0;
  virtual std::vector<std::string> Keys() = 0;
  virtual bool Rename(std::string key, std::string new_key) = 0;
  virtual std::string Ttl(std::string param) = 0;  //???
  virtual std::vector<std::string> Find(Student student) = 0;
  virtual std::vector<Student> Showall() = 0;
  virtual std::pair<bool, int> Upload(std::string path) = 0;
  virtual std::pair<bool, int> Export(std::string path) = 0;
};
}  // namespace s21
#endif  // TRANSACTIONS_COMMON_BASE_CLASS_H_
