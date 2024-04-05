#include "view.h"

#include "../parser/parser.h"

namespace s21 {
View::~View() {
  if (controller_) delete controller_;
}
void View::Start() {
  while (true) {
    ShowMainMenu();
    std::string aaa;
    std::cin >> aaa;
    if (aaa == "1") {
      if (controller_) delete controller_;
      controller_ = new Controller("hash");
      InputCommandAndParams();
    } else if (aaa == "2") {
      if (controller_) delete controller_;
      controller_ = new Controller("bst");
      InputCommandAndParams();
    } else if (aaa == "3") {
      if (controller_) delete controller_;
      controller_ = new Controller("btree");
      InputCommandAndParams();
    } else if (aaa == "4") {
      break;
    }
  }
}

View::Vector View::GetParams(std::string str) {
  Parser parser;
  auto params = parser.Parse(str);
  return params;
}

std::string View::GetCommand(Vector params) { return ToLower(params[0]); }

void View::Execute(std::string command, Vector params) {
  std::vector<std::string> commands = {"set",    "get",     "exists", "del",
                                       "update", "keys",    "rename", "ttl",
                                       "find",   "showall", "upload", "export"};
  int ind = -1;
  for (int i = 0; i < commands.size(); ++i) {
    if (command == commands[i]) {
      ind = i;
      break;
    }
  }

  switch (ind) {
    case 0:
      CallSet(params);
      break;
    case 1:
      CallGet(params);
      break;
    case 2:
      CallExists(params);
      break;
    case 3:
      CallDel(params);
      break;
    case 4:
      CallUpdate(params);
      break;
    case 5:
      CallKeys();
      break;
    case 6:
      CallRename(params);
      break;
    case 7:
      CallTtl(params);
      break;
    case 8:
      CallFind(params);
      break;
    case 9:
      CallShowall();
      break;
    case 10:
      CallUpload(params);
      break;
    case 11:
      CallExport(params);
      break;
    default:
      break;
  }
}

void View::CallSet(Vector params) {
  bool res = true;
  if (params.size() != 7 && params.size() != 9) {
    IncorrectParamsMessage();
    return;
  }

  if (params.size() == 7) {
    auto birth = ParseToInt(params[4]);
    if (!birth.first) return;
    auto coins = ParseToInt(params[6]);
    if (!coins.first) return;
    res = controller_->Set(
        params[1],
        Student(params[2], params[3], birth.second, params[5], coins.second));
  }

  if (params.size() == 9) {
    auto birth = ParseToInt(params[4]);
    if (!birth.first) return;
    auto coins = ParseToInt(params[6]);
    if (!coins.first) return;
    if (ToLower(params[7]) != "ex") {
      IncorrectParamsMessage();
      return;
    }
    auto validity = ParseToInt(params[8]);
    if (!validity.first) return;
    res = controller_->Set(
        params[1],
        Student(params[2], params[3], birth.second, params[5], coins.second),
        validity.second);
  }

  if (res) {
    std::cout << "OK" << std::endl;
  } else {
    std::cout << "This key already exists" << std::endl;
  }
}

void View::CallGet(Vector params) {
  if (params.size() != 2) {
    IncorrectParamsMessage();
    return;
  }
  auto res = controller_->Get(params[1]);

  if (res.IsEmpty()) {
    std::cout << "(null)" << std::endl;
  } else {
    std::cout << res.ToString() << std::endl;
  }
}

void View::CallExists(Vector params) {
  if (params.size() != 2) {
    IncorrectParamsMessage();
    return;
  }
  auto res = controller_->Exists(params[1]);
  if (res) {
    std::cout << "true" << std::endl;
  } else {
    std::cout << "false" << std::endl;
  }
}

void View::CallDel(Vector params) {
  if (params.size() != 2) {
    IncorrectParamsMessage();
    return;
  }
  auto res = controller_->Del(params[1]);
  if (res) {
    std::cout << "true" << std::endl;
  } else {
    std::cout << "false" << std::endl;
  }
}

void View::CallUpdate(Vector params) {
  if (params.size() != 7) {
    IncorrectParamsMessage();
    return;
  }

  int birthday;
  if (params[4] == "-") {
    birthday = INT32_MAX;
  } else {
    auto res = ParseToInt(params[4]);
    if (!res.first) return;
    birthday = res.second;
  }

  int coins;
  if (params[6] == "-") {
    coins = INT32_MAX;
  } else {
    auto res = ParseToInt(params[6]);
    if (!res.first) return;
    coins = res.second;
  }

  bool result = controller_->Update(
      params[1], Student(params[2], params[3], birthday, params[5], coins));

  if (result) {
    std::cout << "OK" << std::endl;
  } else {
    std::cout << "Update error" << std::endl;
  }
}

void View::CallKeys() {
  auto keys = controller_->Keys();
  for (size_t i = 0; i < keys.size(); ++i) {
    std::cout << i + 1 << ") " << keys[i] << std::endl;
  }
}

void View::CallRename(Vector params) {
  if (params.size() != 3) {
    IncorrectParamsMessage();
    return;
  }
  bool res = controller_->Rename(params[1], params[2]);
  if (res) {
    std::cout << "OK" << std::endl;
  } else {
    std::cout << "Rename error" << std::endl;
  }
}

void View::CallTtl(Vector params) {}

void View::CallFind(Vector params) {
  if (params.size() != 6) {
    IncorrectParamsMessage();
    return;
  }

  int birthday;
  if (params[3] == "-") {
    birthday = INT32_MAX;
  } else {
    auto res = ParseToInt(params[3]);
    if (!res.first) return;
    birthday = res.second;
  }

  int coins;
  if (params[5] == "-") {
    coins = INT32_MAX;
  } else {
    auto res = ParseToInt(params[5]);
    if (!res.first) return;
    coins = res.second;
  }

  auto keys = controller_->Find(
      Student(params[1], params[2], birthday, params[4], coins));

  for (size_t i = 0; i < keys.size(); ++i) {
    std::cout << i + 1 << ") " << keys[i] << std::endl;
  }
}

void View::CallShowall() {
  auto students = controller_->Showall();
  if (students.size() != 0) {
    std::cout << std::left << std::setw(3) << "#" << std::left << std::setw(15)
              << "Name" << std::left << std::setw(15) << "Surname" << std::left
              << std::setw(10) << "Birthday" << std::left << std::setw(15)
              << "City" << std::left << std::setw(8) << "Coins" << std::endl;
    for (size_t i = 0; i < students.size(); ++i) {
      std::cout << std::setw(3) << i + 1 << students[i].ToString() << std::endl;
    }
  }
}

void View::CallUpload(Vector params) {
  if (params.size() != 2) {
    IncorrectParamsMessage();
    return;
  }
  auto res = controller_->Upload(params[1]);
  if (res.first) {
    std::cout << "OK " << res.second << std::endl;
  } else {
    std::cout << "Upload error" << std::endl;
  }
}

void View::CallExport(Vector params) {
  if (params.size() != 2) {
    IncorrectParamsMessage();
    return;
  }
  auto res = controller_->Export(params[1]);
  if (res.first) {
    std::cout << "OK " << res.second << std::endl;
  } else {
    std::cout << "Export error" << std::endl;
  }
}

void View::InputCommandAndParams() {
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  while (true) {
    std::cout << "Input command and params. For exit input quit" << std::endl;
    std::string input;
    getline(std::cin, input);

    auto params = GetParams(input);
    std::string command = GetCommand(params);
    if (command == "quit")
      break;
    else
      Execute(command, params);
  }
}

void View::ShowMainMenu() {
  std::cout << "Input model (1 - 3). For exit input 4" << std::endl;
  std::cout << "1 (HashTable)" << std::endl;
  std::cout << "2 (SelfBalancingBinarySearchTree)" << std::endl;
  std::cout << "3 (BPlusTree)" << std::endl;
  std::cout << "4 (Exit)" << std::endl;
}

void View::IncorrectParamsMessage() {
  std::cout << "Incorrect params entered. Try again" << std::endl;
}

std::pair<bool, int> View::ParseToInt(std::string str) {
  try {
    int res = stoi(str);
    return std::make_pair(true, res);
  } catch (...) {
    std::cout << "ERROR: unable to cast value " << str << " to type int"
              << std::endl;
    return std::make_pair(false, 0);
  }
}
std::string View::ToLower(std::string str) {
  for (auto& c : str) {
    c = tolower(c);
  }
  return str;
}
}  // namespace s21
