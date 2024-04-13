#include <model/bst/self_balancing_binary_search_tree.h>
#include <model/common/data.h>
#include <model/hashtable/hash_table.h>
#include <view/mainmenu.h>

#include <iostream>
#include <string>

namespace s21 {
struct StudentComparator {
  std::string str_ignored = "-";
  int int_ignored = std::numeric_limits<int>::min();

  bool operator()(Student student_1, Student student_2) const {
    bool name_equal =
        (student_1.name == student_2.name || student_2.name == str_ignored);
    bool surname_equal = (student_1.surname == student_2.surname ||
                          student_2.surname == str_ignored);
    bool birhday_equal = (student_1.birthday == student_2.birthday ||
                          student_2.birthday == int_ignored);
    bool city_equal =
        (student_1.city == student_2.city || student_2.city == str_ignored);
    bool coins_equal =
        (student_1.coins == student_2.coins || student_2.coins == int_ignored);

    return name_equal && surname_equal && birhday_equal && city_equal &&
           coins_equal;
  }
};
}  // namespace s21

int main() {
  s21::HashTable<std::string, s21::Student, s21::StudentComparator> hashtable;
  s21::Controller controller_1(std::move(hashtable));

  s21::MainMenu mainmenu(controller_1, controller_1, controller_1);

  mainmenu.Start();

  return 0;
}
