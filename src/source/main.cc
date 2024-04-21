#include <model/bst/self_balancing_binary_search_tree.h>
#include <model/common/data.h>
#include <model/hashtable/hash_table.h>
#include <view/mainmenu.h>

int main() {
  s21::HashTable<std::string, s21::Student, s21::StudentComparator> hashtable;
  s21::SelfBalancingBinarySearchTree<std::string, s21::Student,
                                     s21::StudentComparator>
      sbbst;

  s21::Controller<std::string, s21::Student> controller_1(std::move(hashtable));
  s21::Controller<std::string, s21::Student> controller_2(std::move(sbbst));

  s21::MainMenu<std::string, s21::Student> mainmenu(controller_1, controller_2,
                                                    controller_1);

  mainmenu.Start();

  return 0;
}
