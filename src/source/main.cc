#include <functional>
#include <iostream>

#include "model/hashtable/hash_table.h"

int main() {
  s21::HashTable<s21::Student> table;

  table.Set("KEY", s21::Student("NAME", "SURNAME", 12, "CITY", 5555));
  table.Set("KEY2", s21::Student("NAME2", "SURNAME2", 12, "CITY", 5555));
  table.Set("KEY3", s21::Student("NAME3", "SURNAME3", 12, "CITY", 5555));
  table.Set("KEY4", s21::Student("NAME4", "SURNAME4", 12, "CITY", 5555));

  std::cout << table.Get("KEY").ToString() << std::endl;
  std::cout << table.Get("KEY2").ToString() << std::endl;
  std::cout << table.Get("KEY3").ToString() << std::endl;
  std::cout << table.Get("KEY4").ToString() << std::endl;
  std::cout << table.Get("KEY5").ToString() << std::endl;
  return 0;
}