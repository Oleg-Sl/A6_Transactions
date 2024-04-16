// #include <model/bst/self_balancing_binary_search_tree.h>
#include <model/common/data.h>
#include <model/hashtable/hash_table.h>
#include <view/mainmenu.h>
#include <view/storagemenu.h>

#include <iostream>
#include <string>

namespace s21 {

template <>
void StorageMenu<std::string, Student>::Update() {
  std::stringstream user_input = ReadInputAsStringStream();
  std::string key = parser_.ParseValue<std::string>(user_input, "key");
  std::string name = parser_.ParseValue<std::string>(user_input, "name");
  std::string surname = parser_.ParseValue<std::string>(user_input, "surname");
  std::string birthday =
      parser_.ParseValue<std::string>(user_input, "birthday");
  std::string city = parser_.ParseValue<std::string>(user_input, "city");
  std::string coins = parser_.ParseValue<std::string>(user_input, "coins");

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
void StorageMenu<std::string, Student>::Find() {
  std::stringstream user_input = ReadInputAsStringStream();
  Student student;
  student.name = parser_.ParseValue<std::string>(user_input, "name");
  student.surname = parser_.ParseValue<std::string>(user_input, "surname");
  std::string birthday =
      parser_.ParseValue<std::string>(user_input, "birthday");
  student.city = parser_.ParseValue<std::string>(user_input, "city");
  std::string coins = parser_.ParseValue<std::string>(user_input, "coins");

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
  using key = std::string;
  using value = s21::Student;

  s21::HashTable<key, value> hashtable;
  s21::Controller<key, value> controller_1(std::move(hashtable));

  s21::MainMenu<key, value> mainmenu(controller_1, controller_1, controller_1);

  mainmenu.Start();

  return 0;
}
