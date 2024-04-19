#ifndef TRANSACTIONS_SOURCE_MODEL_COMMON_MANAGERTTL_H_
#define TRANSACTIONS_SOURCE_MODEL_COMMON_MANAGERTTL_H_

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

#include "model/common/base_class.h"

namespace s21 {

template <typename Key, typename Value>
class ManagerTTL {
 public:
  struct Record {
    Key key;
    int TTL;
    std::chrono::steady_clock::time_point create_time;

    bool operator<(const Record& other) const {
      return create_time + std::chrono::seconds(TTL) >
             other.create_time + std::chrono::seconds(other.TTL);
    }
  };

  ManagerTTL(BaseClass<Key, Value>& storage) : storage_(storage) {}

  void AddRecord(const Record& record) {
    UpdateRecordsPriority();
    pq_.push(record);
  }

  void DeleteExpiredRecords() {
    std::chrono::steady_clock::time_point now =
        std::chrono::steady_clock::now();
    while (!pq_.empty() &&
           pq_.top().create_time + std::chrono::seconds(pq_.top().TTL) <= now) {
      storage_.Del(pq_.top().key);
      pq_.pop();
    }
  }

  void UpdateRecordsPriority() {
    std::priority_queue<Record> temp_pq;
    auto time = std::chrono::steady_clock::now();

    while (!pq_.empty()) {
      Record curr = pq_.top();
      pq_.pop();

      temp_pq.push(Record{curr.key, curr.TTL, time});
    }

    pq_ = std::move(temp_pq);
  }

  void StartManager(std::chrono::seconds sleep_time, std::mutex& mtx,
                    std::condition_variable& cv, bool& status) {
    while (!stop_manager) {
      {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait_for(lock, sleep_time);
        cv.wait(lock, [this, &status] { return status; });
        status = false;
        DeleteExpiredRecords();
        status = true;
        cv.notify_all();
      }
    }
  }

  void StopManager() { stop_manager = true; }

 private:
  bool stop_manager = false;
  BaseClass<Key, Value>& storage_;
  std::priority_queue<Record> pq_;
};

}  // namespace s21
#endif  // TRANSACTIONS_SOURCE_MODEL_COMMON_MANAGERTTL_H_