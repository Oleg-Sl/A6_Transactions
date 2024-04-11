#ifndef TRANSACTIONS_VIEW_VIEW_H_
#define TRANSACTIONS_VIEW_VIEW_H_
#include <functional>
#include <map>
#include <sstream>
#include <stack>
#include <string>

#include "controller/controller.h"
#include "model/hashtable/hash_table.h"
#include "model/parser/parser.h"

namespace s21 {
class View {
  struct MenuAction {
    std::function<void()> function;
    std::string description;
  };

 public:
  const std::map<std::string, MenuAction> kStorageCommands = {
      {"set", {[this] { Set(); }, "<key> <struct> [ex <time>]"}},
      {"get", {[this] { Get(); }, "<key>"}},
      {"exists", {[this] { Exists(); }, "<key>"}},
      {"del", {[this] { Del(); }, "<key>"}},
      {"update", {[this] { Update(); }, "<key> <struct>"}},
      {"keys", {[this] { Keys(); }, ""}},
      {"rename", {[this] { Rename(); }, "<key1> <key2>"}},
      {"ttl", {[this] { Ttl(); }, "<key>"}},
      {"find", {[this] { Find(); }, "<struct>"}},
      {"showall", {[this] { Showall(); }, ""}},
      {"upload", {[this] { Upload(); }, "<path>"}},
      {"export", {[this] { Export(); }, "<path>"}},
      {"help", {[this] { DisplayMenu(kStorageCommands); }}},
      {"exit", {[this] { PopMenu(); }}}};

  const std::map<std::string, MenuAction> kMainMenuCommands = {
      {"1", {[this] { UseHashTable(); }, "use HashTable"}},
      {"2", {[this] { UseSBBST(); }, "use SBBST"}},
      {"3", {[this] { UseBPlusTree(); }, "use BPlusTree"}},
      {"4", {[this] { PopMenu(); }, "exit"}}};

  void Start();

 private:
  Parser parser_;
  std::unique_ptr<Controller> controller_;
  std::stack<std::map<std::string, MenuAction>> stack_menu_;
  std::stringstream user_input_;

  void Set();
  void Get();
  void Exists();
  void Del();
  void Update();
  void Keys();
  void Rename();
  void Ttl();
  void Find();
  void Showall();
  void Upload();
  void Export();

  void UseHashTable();
  void UseSBBST();
  void UseBPlusTree();

  void SetController(std::unique_ptr<Controller> controller);

  void DisplayMenu(const std::map<std::string, MenuAction>& menu);
  void PushMenu(const std::map<std::string, MenuAction>& menu);
  void PopMenu();
  void ExecuteCommand();
};

}  // namespace s21
#endif  // TRANSACTIONS_VIEW_VIEW_H_