#ifndef TRANSACTIONS_CONTROLLER_CONTROLLER_H_
#define TRANSACTIONS_CONTROLLER_CONTROLLER_H_

#include "../bst/self_balancing_binary_search_tree.h"

namespace s21 {
class Controller {
 public:
  bool Set(Data data);
  Student Get(std::string key);
  bool Exists(std::string key);
  bool Del(std::string key);
  bool Update(std::string key, Student student);
  std::vector<std::string> Keys();
  bool Rename(std::string key, std::string new_key);
  std::string Ttl(std::string param);
  std::vector<std::string> Find(Student student);
  std::vector<Student> Showall();
  std::pair<bool, int> Upload(std::string path);
  std::pair<bool, int> Export(std::string path);

 private:
  SelfBalancingBinarySearchTree bst_;
};

}  // namespace s21
#endif  // TRANSACTIONS_CONTROLLER_CONTROLLER_H_