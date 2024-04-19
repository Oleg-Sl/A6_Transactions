#include <model/bst/self_balancing_binary_search_tree.h>
#include <model/common/data.h>
#include <model/hashtable/hash_table.h>
#include <view/mainmenu.h>

#include <mutex>
#include <thread>

#include "model/common/managerttl.h"

int main() {
  s21::HashTable<std::string, s21::Student, s21::StudentComparator> hashtable;
  s21::SelfBalancingBinarySearchTree sbbst;

  s21::Controller controller_1(hashtable);
  s21::Controller controller_2(sbbst);

  // s21::MainMenu mainmenu(controller_1, controller_2, controller_1);
  s21::StorageMenu storage(controller_1);
  s21::ManagerTTL manager(hashtable);

  storage.Start();

  return 0;
}
