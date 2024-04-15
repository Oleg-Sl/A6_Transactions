#ifndef TRANSACTIONS_SOURCE_VIEW_MAINMENU_H_
#define TRANSACTIONS_SOURCE_VIEW_MAINMENU_H_

#include <view/baseview.h>

#include "controller/controller.h"
#include "model/parser/parser.h"
#include "view/storagemenu.h"

namespace s21 {
template <typename Key, typename Value>
class MainMenu : public BaseView {
 public:
  const std::map<std::string, MenuAction> kMainMenuCommands = {
      {"1", {[this] { UseHashTable(); }, "use HashTable"}},
      {"2", {[this] { UseSBBST(); }, "use SBBST"}},
      {"3", {[this] { UseBPlusTree(); }, "use BPlusTree"}},
      {"4", {[this] { PopMenu(); }, "exit"}}};

  MainMenu(const Controller<Key, Value>& hashtable,
           const Controller<Key, Value>& sbbst,
           const Controller<Key, Value>& bplustree)
      : hashtable_(hashtable), sbbst_(sbbst), bplustree_(bplustree) {}

  void Start() override {
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

 private:
  Controller<Key, Value> hashtable_;
  Controller<Key, Value> sbbst_;
  Controller<Key, Value> bplustree_;

  void UseHashTable() const { StorageMenu<Key, Value>(hashtable_).Start(); }

  void UseSBBST() const { StorageMenu<Key, Value>(sbbst_).Start(); }

  void UseBPlusTree() const { StorageMenu<Key, Value>(bplustree_).Start(); }
};

}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_VIEW_MAINMENU_H_