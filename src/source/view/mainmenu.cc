#include "view/mainmenu.h"
#include "view/storage.h"

namespace s21 {

void MainMenu::Start() {
  PushMenu(kMainMenuCommands);

  while (stack_menu_.size() > 0) {
    DisplayMenu(kMainMenuCommands);
    std::cout << "> ";
    std::string input;
    getline(std::cin, input);
    std::stringstream stream(input);
    std::string command;
    stream >> command;

    ExecuteCommand(command);
  }
}

void MainMenu::UseHashTable() { Storage(hashtable_).Start(); }

void MainMenu::UseSBBST() {}

void MainMenu::UseBPlusTree() {}

}  // namespace s21