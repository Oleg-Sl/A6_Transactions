#ifndef TRANSACTIONS_SOURCE_VIEW_STORAGEBENCHMARK_H_
#define TRANSACTIONS_SOURCE_VIEW_STORAGEBENCHMARK_H_

#include <chrono>
#include <random>

#include "controller/controller.h"
#include "model/common/data.h"
#include "view/baseview.h"

namespace s21 {

class StorageBenchmark {
 public:
  static constexpr int kKeyLength = 15;

  void FillStorage(Controller<std::string, Student> controller, size_t count);

  double MeasureGet(Controller<std::string, Student> storage, size_t repeats);
  double MeasureShowAll(Controller<std::string, Student> storage,
                        size_t repeats);
  double MeasureFind(Controller<std::string, Student> storage, size_t repeats);
  double MeasureSet(Controller<std::string, Student> storage, size_t repeats);
  double MeasureDel(Controller<std::string, Student> storage, size_t repeats);

 private:
  std::random_device rd_;
  std::mt19937 gen_{rd_()};
};
}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_VIEW_STORAGEBENCHMARK_H_