#ifndef TRANSACTIONS_VIEW_VIEW_H_
#define TRANSACTIONS_VIEW_VIEW_H_
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <set>
#include <sstream>
#include <vector>

#include "controller/controller.h"
#include "model/parser/parser.h"

namespace s21 {
class View {
 public:
  using Vector = std::vector<std::string>;

  const std::unordered_map<std::string, std::function<void(std::stringstream&)>>
      kCommands = {
          {"set", [this](std::stringstream& stream) { Set(stream); }},
          {"get", [this](std::stringstream& stream) { Get(stream); }},
          {"exists", [this](std::stringstream& stream) { Exists(stream); }},
          {"del", [this](std::stringstream& stream) { Del(stream); }},
          {"update", [this](std::stringstream& stream) { Update(stream); }},
          {"keys", [this](std::stringstream& stream) { Keys(stream); }},
          {"rename", [this](std::stringstream& stream) { Rename(stream); }},
          {"ttl", [this](std::stringstream& stream) { Ttl(stream); }},
          {"find", [this](std::stringstream& stream) { Find(stream); }},
          {"showall", [this](std::stringstream& stream) { Showall(stream); }},
          {"upload", [this](std::stringstream& stream) { Upload(stream); }},
          {"export", [this](std::stringstream& stream) { Export(stream); }}};

  void Start();

 private:
  std::unique_ptr<Controller> controller_;
  Parser parser_;

  void Set(std::stringstream& stream);
  void Get(std::stringstream& stream);
  void Exists(std::stringstream& stream);
  void Del(std::stringstream& stream);
  void Update(std::stringstream& stream);
  void Keys(std::stringstream& stream);
  void Rename(std::stringstream& stream);
  void Ttl(std::stringstream& stream);
  void Find(std::stringstream& stream);
  void Showall(std::stringstream& stream);
  void Upload(std::stringstream& stream);
  void Export(std::stringstream& stream);

  void InputCommandAndParams();
  void ShowMainMenu();
  void IncorrectParamsMessage();
  std::pair<bool, int> ParseToInt(std::string str);
  std::string ToLower(std::string str);
};

}  // namespace s21
#endif  // TRANSACTIONS_VIEW_VIEW_H_