#ifndef TRANSACTIONS_SOURCE_MODEL_COMMON_STORAGEBENCHMARK_H_
#define TRANSACTIONS_SOURCE_MODEL_COMMON_STORAGEBENCHMARK_H_

#include "controller/controller.h"
#include "model/common/data.h"

namespace s21 {

class StorageBenchmark {
 public:
  static constexpr int kKeyLength = 15;

  static void FillStorage(const Controller<std::string, Student>& controller,
                          size_t count);
  static double MeasureGet(const Controller<std::string, Student>& storage,
                           size_t repeats);
  static double MeasureShowAll(const Controller<std::string, Student>& storage,
                               size_t repeats);
  static double MeasureFind(const Controller<std::string, Student>& storage,
                            size_t repeats);
  static double MeasureSet(const Controller<std::string, Student>& storage,
                           size_t repeats);
  static double MeasureDel(const Controller<std::string, Student>& storage,
                           size_t repeats);
};
}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_MODEL_COMMON_STORAGEBENCHMARK_H_