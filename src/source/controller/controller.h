#ifndef TRANSACTIONS_SOURCE_CONTROLLER_CONTROLLER_H_
#define TRANSACTIONS_SOURCE_CONTROLLER_CONTROLLER_H_

// #include "model/bst/self_balancing_binary_search_tree.h"
#include <memory>

#include "model/common/base_class.h"
#include "model/common/data.h"

namespace s21 {
class Controller {
 public:
  explicit Controller(BaseClass&& model);
  bool Set(std::string key, Student student, int validity = 0);
  Student Get(std::string key);
  bool Exists(std::string key);
  bool Del(std::string key);
  bool Update(std::string key, Student student);
  std::vector<std::string> Keys();
  bool Rename(std::string key, std::string new_key);
  int Ttl(std::string param);
  std::vector<std::string> Find(Student student);
  std::vector<Student> Showall();
  std::pair<bool, int> Upload(std::string path);
  std::pair<bool, int> Export(std::string path);

 private:
  BaseClass& model_;
};

}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_CONTROLLER_CONTROLLER_H_