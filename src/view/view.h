#ifndef TRANSACTIONS_VIEW_VIEW_H_
#define TRANSACTIONS_VIEW_VIEW_H_
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include "../controller/controller.h"

namespace s21 {
class View {
 public:
  using Vector = std::vector<std::string>;
  View() = default;
  void Start();

 private:
  Controller controller_;
  Vector GetParams(std::string str);
  std::string GetCommand(Vector params);
  void Execute(std::string command, Vector params);
  void CallSet(Vector params);
  void CallGet(Vector params);
  void CallExists(Vector params);
  void CallDel(Vector params);
  void CallUpdate(Vector params);
  void CallKeys();
  void CallRename(Vector params);
  void CallTtl(Vector params);
  void CallFind(Vector params);
  void CallShowall();
  void CallUpload(Vector params);
  void CallExport(Vector params);
  void IncorrectParamsMessage();
  std::pair<bool, int> ParseToInt(std::string str);
  std::string ToLower(std::string str);
};

}  // namespace s21
#endif  // TRANSACTIONS_VIEW_VIEW_H_