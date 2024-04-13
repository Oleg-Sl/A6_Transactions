#ifndef TRANSACTIONS_SOURCE_VIEW_STORAGE_H_
#define TRANSACTIONS_SOURCE_VIEW_STORAGE_H_

#include <view/baseview.h>

#include <functional>
#include <map>
#include <sstream>
#include <stack>
#include <string>

#include "controller/controller.h"
#include "model/parser/parser.h"

namespace s21 {
class Storage : BaseView {
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

  Storage(const Controller& controller) : controller_(controller) {}

  void Start();

 private:
  Controller controller_;
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
};

}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_VIEW_STORAGE_H_