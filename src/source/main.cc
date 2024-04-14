// #include <model/bst/self_balancing_binary_search_tree.h>
#include <model/common/data.h>
#include <model/hashtable/hash_table.h>
#include <view/mainmenu.h>
#include <view/storage.h>

#include <iostream>
#include <string>

namespace s21 {
struct StudentComparator {
  const std::string kStrIgnored = "-";
  const int kIntIgnored = std::numeric_limits<int>::min();

  bool operator()(Student student_1, Student student_2) const {
    bool name_equal =
        (student_1.name == student_2.name || student_2.name == kStrIgnored);
    bool surname_equal = (student_1.surname == student_2.surname ||
                          student_2.surname == kStrIgnored);
    bool birhday_equal = (student_1.birthday == student_2.birthday ||
                          student_2.birthday == kIntIgnored);
    bool city_equal =
        (student_1.city == student_2.city || student_2.city == kStrIgnored);
    bool coins_equal =
        (student_1.coins == student_2.coins || student_2.coins == kIntIgnored);

    return name_equal && surname_equal && birhday_equal && city_equal &&
           coins_equal;
  }
};

template <>
void Storage<std::string, Student>::Update() {
  std::string key = parser_.ParseValue<std::string>(user_input_, "key");
  std::string name = parser_.ParseValue<std::string>(user_input_, "name");
  std::string surname = parser_.ParseValue<std::string>(user_input_, "surname");
  std::string birthday =
      parser_.ParseValue<std::string>(user_input_, "birthday");
  std::string city = parser_.ParseValue<std::string>(user_input_, "city");
  std::string coins = parser_.ParseValue<std::string>(user_input_, "coins");

  Student new_student;
  Student old_student = controller_.Get(key);
  new_student.name = name == "-" ? old_student.name : name;
  new_student.surname = surname == "-" ? old_student.surname : surname;
  new_student.birthday =
      birthday == "-" ? old_student.birthday : std::stoi(birthday);
  new_student.city = city == "-" ? old_student.city : city;
  new_student.coins = coins == "-" ? old_student.coins : std::stoi(coins);
  std::cout << controller_.Update(key, new_student) << std::endl;
}

template <>
void Storage<std::string, Student>::Find() {
  Student student;
  student.name = parser_.ParseValue<std::string>(user_input_, "name");
  student.surname = parser_.ParseValue<std::string>(user_input_, "surname");
  std::string birthday =
      parser_.ParseValue<std::string>(user_input_, "birthday");
  student.city = parser_.ParseValue<std::string>(user_input_, "city");
  std::string coins = parser_.ParseValue<std::string>(user_input_, "coins");

  birthday == "-" ? student.birthday = std::numeric_limits<int>::min()
                  : student.birthday = std::stoi(birthday);
  coins == "-" ? student.coins = std::numeric_limits<int>::min()
               : student.coins = std::stoi(coins);

  size_t counter = 1;
  for (auto& key : controller_.Find(student)) {
    std::cout << counter << ") " << key << std::endl;
    ++counter;
  }
}
}  // namespace s21

int main() {
  s21::HashTable<std::string, s21::Student> hashtable;
  s21::Controller<std::string, s21::Student> controller_1(std::move(hashtable));

  s21::MainMenu<std::string, s21::Student> mainmenu(controller_1, controller_1,
                                                    controller_1);

  mainmenu.Start();

  return 0;
}
