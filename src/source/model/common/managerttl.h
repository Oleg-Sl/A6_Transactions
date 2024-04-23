#ifndef TRANSACTIONS_SOURCE_MODEL_COMMON_MANAGERTTL_H_
#define TRANSACTIONS_SOURCE_MODEL_COMMON_MANAGERTTL_H_

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <functional>

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

  ManagerTTL(BaseClass<Key, Value>& storage)
      : storage_(storage),
        thread_([this] { StartManager(std::chrono::seconds(5)); }) {}

  ~ManagerTTL() {
    StopManager();
    
    thread_.join();
  }

  void AddRecord(const Record& record) { pq_.push(record); }

  void DeleteExpiredRecords() {
    std::chrono::steady_clock::time_point now =
        std::chrono::steady_clock::now();
    while (!pq_.empty() &&
           pq_.top().create_time + std::chrono::seconds(pq_.top().TTL) <= now) {
      storage_.Del(pq_.top().key);
      pq_.pop();
    }
  }

  void StartManager(std::chrono::seconds sleep_time) {
    while (!stop_manager_) {
      {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait_for(lock, sleep_time, [this] { return storage_access_; });
        cv_.wait(lock, [this] { return storage_access_; });
        storage_access_ = false;
        DeleteExpiredRecords();
        storage_access_ = true;
        cv_.notify_all();
      }
    }
  }

  void StopManager() { stop_manager_ = true; }

  template <typename Func, typename... Args>
  auto Execute(Func func, Args... args) {
    {  // Delete expired records
      std::unique_lock<std::mutex> lock(mtx_);
      cv_.wait(lock, [this] { return storage_access_; });
    }
    cv_.notify_all();

    std::unique_lock<std::mutex> lock(mtx_);
    cv_.wait(lock, [this] { return storage_access_; });
    storage_access_ = false;
    auto result = std::invoke(func, storage_, std::forward<Args>(args)...);
    storage_access_ = true;
    cv_.notify_all();
    return result;
  }

 private:
  BaseClass<Key, Value>& storage_;
  
  std::mutex mtx_;
  std::condition_variable cv_;
  bool storage_access_ = true;
  bool stop_manager_ = false;
  std::priority_queue<Record> pq_;
  std::thread thread_;
};

}  // namespace s21
#endif  // TRANSACTIONS_SOURCE_MODEL_COMMON_MANAGERTTL_H_