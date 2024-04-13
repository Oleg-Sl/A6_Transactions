#ifndef TRANSACTIONS_SOURCE_VIEW_MAINMENU_H_
#define TRANSACTIONS_SOURCE_VIEW_MAINMENU_H_

#include <view/baseview.h>

#include "controller/controller.h"
#include "model/parser/parser.h"

namespace s21 {
class MainMenu : public BaseView {
 public:
  const std::map<std::string, MenuAction> kMainMenuCommands = {
      {"1", {[this] { UseHashTable(); }, "use HashTable"}},
      {"2", {[this] { UseSBBST(); }, "use SBBST"}},
      {"3", {[this] { UseBPlusTree(); }, "use BPlusTree"}},
      {"4", {[this] { PopMenu(); }, "exit"}}};

  MainMenu(const Controller& hashtable, const Controller& sbbst,
           const Controller& bplustree)
      : hashtable_(hashtable), sbbst_(sbbst), bplustree_(bplustree) {}

  void Start();

 private:
  Controller hashtable_;
  Controller sbbst_;
  Controller bplustree_;

  void UseHashTable();
  void UseSBBST();
  void UseBPlusTree();
};

}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_VIEW_MAINMENU_H_