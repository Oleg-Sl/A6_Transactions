#include "view/view.h"

#include <iostream>
#include <limits>

#include "model/common/data.h"
#include "model/hashtable/hash_table.h"
#include "model/parser/parser.h"

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

void View::Start() {
  PushMenu(kMainMenuCommands);

  while (stack_menu_.size() > 0) {
    std::cout << "> ";
    std::string input;
    getline(std::cin, input);
    user_input_ = std::stringstream(input);

    ExecuteCommand();
  }
}

void View::Set() {
  std::string key = parser_.ParseValue<std::string>(user_input_, "key");
  Student student = parser_.ParseValue<Student>(user_input_, "student");

  bool status = false;
  auto optional_arg = parser_.ParseOptionalArgument<int>(user_input_, "ex");
  if (optional_arg.first.empty()) {
    status = controller_->Set(key, student);
  } else {
    status = controller_->Set(key, student, optional_arg.second);
  }

  std::cout << status << std::endl;
}

void View::Get() {
  std::string key = parser_.ParseValue<std::string>(user_input_, "key");
  try {
    Student student = controller_->Get(key);
    std::cout << student << std::endl;
  } catch (std::invalid_argument& ex) {
    std::cout << "(null)" << std::endl;
  }
}

void View::Exists() {
  std::string key = parser_.ParseValue<std::string>(user_input_, "key");
  std::cout << controller_->Exists(key) << std::endl;
}

void View::Del() {
  std::string key = parser_.ParseValue<std::string>(user_input_, "key");
  std::cout << controller_->Del(key) << std::endl;
}

void View::Update() {
  std::string key = parser_.ParseValue<std::string>(user_input_, "key");
  std::string name = parser_.ParseValue<std::string>(user_input_, "name");
  std::string surname = parser_.ParseValue<std::string>(user_input_, "surname");
  std::string birthday =
      parser_.ParseValue<std::string>(user_input_, "birthday");
  std::string city = parser_.ParseValue<std::string>(user_input_, "city");
  std::string coins = parser_.ParseValue<std::string>(user_input_, "coins");

  Student new_student;
  Student old_student = controller_->Get(key);
  new_student.name = name == "-" ? old_student.name : name;
  new_student.surname = surname == "-" ? old_student.surname : name;
  new_student.birthday =
      birthday == "-" ? old_student.birthday : std::stoi(birthday);
  new_student.city = city == "-" ? old_student.city : city;
  new_student.coins = coins == "-" ? old_student.coins : std::stoi(coins);
  std::cout << controller_->Update(key, new_student) << std::endl;
}

void View::Keys() {
  size_t counter = 1;
  for (auto& key : controller_->Keys()) {
    std::cout << counter << ") " << key << std::endl;
    ++counter;
  }
}

void View::Rename() {
  std::string key_1 = parser_.ParseValue<std::string>(user_input_, "key1");
  std::string key_2 = parser_.ParseValue<std::string>(user_input_, "key2");
  std::cout << controller_->Rename(key_1, key_2) << std::endl;
}

void View::Ttl() {
  std::string key = parser_.ParseValue<std::string>(user_input_, "key");
  try {
    std::cout << controller_->Ttl(key) << std::endl;
  } catch (std::invalid_argument& ex) {
    std::cout << "(null)" << std::endl;
  }
}

void View::Find() {
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
  for (auto& key : controller_->Find(student)) {
    std::cout << counter << ") " << key << std::endl;
    ++counter;
  }
}

void View::Showall() {
  size_t counter = 1;
  for (auto& student : controller_->Showall()) {
    std::cout << counter << ") " << student << std::endl;
    ++counter;
  }
}

void View::Upload() {
  std::string path = parser_.ParseValue<std::string>(user_input_, "path");
  auto res = controller_->Upload(path);
  std::cout << res.first << " " << res.second << std::endl;
}

void View::Export() {
  std::string path = parser_.ParseValue<std::string>(user_input_, "path");
  auto res = controller_->Export(path);
  std::cout << res.first << " " << res.second << std::endl;
}

void View::DisplayMenu(const std::map<std::string, MenuAction>& menu) {
  for (auto elem : menu) {
    std::cout << elem.first << " " << elem.second.description << std::endl;
  }
}

void View::ExecuteCommand() {
  try {
    std::string command =
        parser_.ParseValue<std::string>(user_input_, "command");
    auto callable = stack_menu_.top().find(command);
    if (callable != stack_menu_.top().end()) {
      callable->second.function();
    } else {
      std::cout << "incorrect command " << command << std::endl;
    }
  } catch (std::exception& ex) {
    std::cout << ex.what() << std::endl;
  }
}

void View::PushMenu(const std::map<std::string, MenuAction>& menu) {
  DisplayMenu(menu);
  stack_menu_.push(menu);
}

void View::PopMenu() {
  stack_menu_.pop();
  if (!stack_menu_.empty()) {
    DisplayMenu(stack_menu_.top());
  }
}

void View::SetController(std::unique_ptr<Controller> controller) {
  controller_ = std::move(controller);
}

void View::UseHashTable() {
  SetController(std::make_unique<Controller>(
      std::make_unique<HashTable<std::string, Student, StudentComparator>>()));
  PushMenu(kStorageCommands);
}

void View::UseSBBST() {}

void View::UseBPlusTree() {}

}  // namespace s21
