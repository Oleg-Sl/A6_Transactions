#ifndef TRANSACTIONS_SOURCE_MODEL_COMMON_STORAGEBENCHMARK_H_
#define TRANSACTIONS_SOURCE_MODEL_COMMON_STORAGEBENCHMARK_H_

#include "controller/controller.h"
#include "model/common/student.h"

namespace s21 {

class StorageBenchmark {
 public:
  static constexpr int kKeyLength = 15;

  static void FillStorage(Controller<std::string, Student>& controller,
                          size_t count);
  static double MeasureGet(Controller<std::string, Student>& storage,
                           size_t repeats);
  static double MeasureShowAll(Controller<std::string, Student>& storage,
                               size_t repeats);
  static double MeasureFind(Controller<std::string, Student>& storage,
                            size_t repeats);
  static double MeasureSet(Controller<std::string, Student>& storage,
                           size_t repeats);
  static double MeasureDel(Controller<std::string, Student>& storage,
                           size_t repeats);
};
}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_MODEL_COMMON_STORAGEBENCHMARK_H_