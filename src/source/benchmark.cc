#include "model/common/data.h"
#include "model/common/storagebenchmark.h"
#include "model/hashtable/hash_table.h"
#include "view/storagebenchmarkmenu.h"

int main() {
  s21::Controller<std::string, s21::Student> controller_1{
      s21::HashTable<std::string, s21::Student>()};

  s21::StorageBenchmarkMenu<std::string, s21::Student> menu;
  menu.Start();

  return 0;
}