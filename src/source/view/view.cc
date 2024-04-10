#include "model/common/data.h"
#include "model/hashtable/hash_table.h"
#include "model/parser/parser.h"
#include "view/view.h"

namespace s21 {
void View::Start() {
  while (true) {
    ShowMainMenu();
    std::string choice;
    std::cin >> choice;
    if (choice == "1") {
      controller_ = std::make_unique<Controller>(
          std::make_unique<HashTable<std::string, Student>>());
    } else if (choice == "2") {
    } else {
      break;
    }
    InputCommandAndParams();
  }
}

void View::Set(std::stringstream& stream) {
  std::string key = parser_.ParseValue<std::string>(stream, "KEY");
  Student student = parser_.ParseValue<Student>(stream, "STUDENT");

  auto optional_arg = parser_.ParseOptionalArgument<int>(stream, "EX");
  if (optional_arg.first.empty()) {
    controller_->Set(key, student);
  } else {
    controller_->Set(key, student, optional_arg.second);
  }
}

void View::Get(std::stringstream& stream) {
  std::string key = parser_.ParseValue<std::string>(stream, "KEY");
  try {
    Student student = controller_->Get(key);
    std::cout << student << std::endl;
  } catch (std::invalid_argument& ex) {
    std::cout << "(null)" << std::endl;
  }
}

void View::Exists(std::stringstream& stream) {
  std::string key = parser_.ParseValue<std::string>(stream, "KEY");
  std::cout << controller_->Exists(key) << std::endl;
}

void View::Del(std::stringstream& stream) {
  std::string key = parser_.ParseValue<std::string>(stream, "KEY");
  std::cout << controller_->Del(key) << std::endl;
}

void View::Update(std::stringstream& stream) {
  std::string key = parser_.ParseValue<std::string>(stream, "KEY");
  Student student = parser_.ParseValue<Student>(stream, "Student");
  std::cout << controller_->Update(key, student) << std::endl;
}

void View::Keys(std::stringstream& stream) {
  size_t counter = 1;
  for (auto& key : controller_->Keys()) {
    std::cout << counter << ") " << key << std::endl;
    ++counter;
  }
}

void View::Rename(std::stringstream& stream) {
  std::string key_1 = parser_.ParseValue<std::string>(stream, "KEY1");
  std::string key_2 = parser_.ParseValue<std::string>(stream, "KEY2");
  std::cout << controller_->Rename(key_1, key_2) << std::endl;
}

void View::Ttl(std::stringstream& stream) {
  try {
    std::string key = parser_.ParseValue<std::string>(stream, "KEY");
    std::cout << controller_->Ttl(key) << std::endl;
  } catch (std::invalid_argument& ex) {
    std::cout << "(null)" << std::endl;
  }
}

void View::Find(std::stringstream& stream) {
  Student student = parser_.ParseValue<Student>(stream, "STUDENT");
  size_t counter = 1;
  for (auto& key : controller_->Find(student)) {
    std::cout << counter << ") " << key << std::endl;
    ++counter;
  }
}

void View::Showall(std::stringstream& stream) {
  size_t counter = 1;
  for (auto& student : controller_->Showall()) {
    std::cout << counter << ") " << student << std::endl;
    ++counter;
  }
}

void View::Upload(std::stringstream& stream) {
  std::string path = parser_.ParseValue<std::string>(stream, "path");
  controller_->Upload(path);
}

void View::Export(std::stringstream& stream) {
  std::string path = parser_.ParseValue<std::string>(stream, "path");
  controller_->Export(path);
}

void View::InputCommandAndParams() {
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  while (true) {
    std::cout << "Input command and params. For exit input quit" << std::endl;
    std::string input;
    getline(std::cin, input);

    std::stringstream stream(input);
    std::string command = parser_.ParseValue<std::string>(stream, "COMMAND");

    try {
      auto callable = kCommands.find(command);
      if (callable != kCommands.end()) {
        callable->second(stream);
      } else {
        std::cout << "INCORRECT COMMAND " << command << std::endl;
      }
    } catch (std::exception& ex) {
      std::cout << ex.what() << std::endl;
    }
  }
}

void View::ShowMainMenu() {
  std::cout << "Input model (1 - 3). For exit input 4" << std::endl;
  std::cout << "1 (HashTable)" << std::endl;
  std::cout << "2 (SelfBalancingBinarySearchTree)" << std::endl;
  std::cout << "3 (BPlusTree)" << std::endl;
  std::cout << "4 (Exit)" << std::endl;
}

}  // namespace s21
