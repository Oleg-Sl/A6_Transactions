#include <iostream>
#include <limits>

#include "model/common/data.h"
#include "model/hashtable/hash_table.h"
#include "model/parser/parser.h"
#include "view/storage.h"

namespace s21 {
void Storage::Start() {
  PushMenu(kStorageCommands);
  DisplayMenu(kStorageCommands);

  while (stack_menu_.size() > 0) {
    std::cout << "> ";
    std::string input;
    getline(std::cin, input);
    std::stringstream stream(input);
    std::string command;
    stream >> command;
    user_input_ = std::move(stream);

    ExecuteCommand(command);
  }
}

void Storage::Set() {
  std::string key = parser_.ParseValue<std::string>(user_input_, "key");
  Student student = parser_.ParseValue<Student>(user_input_, "student");

  bool status = false;
  auto optional_arg = parser_.ParseOptionalArgument<int>(user_input_, "ex");
  if (optional_arg.first.empty()) {
    status = controller_.Set(key, student);
  } else {
    status = controller_.Set(key, student, optional_arg.second);
  }

  std::cout << status << std::endl;
}

void Storage::Get() {
  std::string key = parser_.ParseValue<std::string>(user_input_, "key");
  try {
    Student student = controller_.Get(key);
    std::cout << student << std::endl;
  } catch (std::invalid_argument& ex) {
    std::cout << "(null)" << std::endl;
  }
}

void Storage::Exists() {
  std::string key = parser_.ParseValue<std::string>(user_input_, "key");
  std::cout << controller_.Exists(key) << std::endl;
}

void Storage::Del() {
  std::string key = parser_.ParseValue<std::string>(user_input_, "key");
  std::cout << controller_.Del(key) << std::endl;
}

void Storage::Update() {
  std::string key = parser_.ParseValue<std::string>(user_input_, "key");
  Student student = parser_.ParseValue<Student>(user_input_, "student");

  std::cout << controller_.Update(key, student) << std::endl;
}

void Storage::Keys() {
  size_t counter = 1;
  for (auto& key : controller_.Keys()) {
    std::cout << counter << ") " << key << std::endl;
    ++counter;
  }
}

void Storage::Rename() {
  std::string key_1 = parser_.ParseValue<std::string>(user_input_, "key1");
  std::string key_2 = parser_.ParseValue<std::string>(user_input_, "key2");
  std::cout << controller_.Rename(key_1, key_2) << std::endl;
}

void Storage::Ttl() {
  std::string key = parser_.ParseValue<std::string>(user_input_, "key");
  try {
    std::cout << controller_.Ttl(key) << std::endl;
  } catch (std::invalid_argument& ex) {
    std::cout << "(null)" << std::endl;
  }
}

void Storage::Find() {
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

void Storage::Showall() {
  size_t counter = 1;
  for (auto& student : controller_.Showall()) {
    std::cout << counter << ") " << student << std::endl;
    ++counter;
  }
}

void Storage::Upload() {
  std::string path = parser_.ParseValue<std::string>(user_input_, "path");
  auto res = controller_.Upload(path);
  std::cout << res.first << " " << res.second << std::endl;
}

void Storage::Export() {
  std::string path = parser_.ParseValue<std::string>(user_input_, "path");
  auto res = controller_.Export(path);
  std::cout << res.first << " " << res.second << std::endl;
}

}  // namespace s21
