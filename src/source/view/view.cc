#include <iostream>

#include "model/common/data.h"
#include "model/hashtable/hash_table.h"
#include "model/parser/parser.h"
#include "view/view.h"

namespace s21 {
void View::Start() {
  AddMenuOnStack(kMainMenuCommands);

  while (stack_menu_.size() > 0) {
    CallMenuAction(stack_menu_.top());
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
  Student student = parser_.ParseValue<Student>(user_input_, "student");
  std::cout << controller_->Update(key, student) << std::endl;
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
  Student student = parser_.ParseValue<Student>(user_input_, "student");
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
  controller_->Upload(path);
}

void View::Export() {
  std::string path = parser_.ParseValue<std::string>(user_input_, "path");
  controller_->Export(path);
}

void View::ShowMenu(const std::map<std::string, MenuAction>& menu) {
  for (auto elem : menu) {
    std::cout << elem.first << " " << elem.second.description << std::endl;
  }
}

void View::CallMenuAction(const std::map<std::string, MenuAction>& menu) {
  std::cout << "> ";
  std::string input;
  getline(std::cin, input);
  user_input_ = std::stringstream(input);
  std::string command = parser_.ParseValue<std::string>(user_input_, "command");

  try {
    auto callable = menu.find(command);
    if (callable != menu.end()) {
      callable->second.function();
    } else {
      std::cout << "incorrect command " << command << std::endl;
    }
  } catch (std::exception& ex) {
    std::cout << ex.what() << std::endl;
  }
}

void View::AddMenuOnStack(const std::map<std::string, MenuAction>& menu) {
  ShowMenu(menu);
  stack_menu_.push(menu);
}

void View::PopMenuFromStack() {
  stack_menu_.pop();
  if (!stack_menu_.empty()) {
    ShowMenu(stack_menu_.top());
  }
}

void View::SetController(std::unique_ptr<Controller> controller) {
  controller_ = std::move(controller);
}

void View::UseHashTable() {
  SetController(std::make_unique<Controller>(
      std::make_unique<HashTable<std::string, Student>>()));
  AddMenuOnStack(kStorageCommands);
}

void View::UseSBBST() {}

void View::UseBPlusTree() {}

}  // namespace s21
