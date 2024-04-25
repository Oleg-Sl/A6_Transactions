#ifndef TRANSACTIONS_SOURCE_MODEL_COMMON_MANAGERTTL_H_
#define TRANSACTIONS_SOURCE_MODEL_COMMON_MANAGERTTL_H_

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

#include "model/common/basestorage.h"

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

  ManagerTTL(BaseStorage<Key, Value>& storage,
             std::chrono::seconds clean_interaval)
      : storage_(storage), manager_thread_([this, &clean_interaval] {
          StartManagerLoop(clean_interaval);
        }) {}

  ~ManagerTTL() {
    StopManager();
    loop_condition_.notify_all();
    manager_thread_.join();
  }

  void AddRecord(const Record& record) {
    if (record.TTL == 0) {
      return;
    }

    std::unique_lock lock(pq_mtx_);
    pq_.push(record);
  }

  void DeleteExpiredRecords() {
    std::unique_lock lock(storage_mtx_);
    std::unique_lock lock2(pq_mtx_);
    std::chrono::steady_clock::time_point now =
        std::chrono::steady_clock::now();
    while (!pq_.empty() &&
           pq_.top().create_time + std::chrono::seconds(pq_.top().TTL) <= now) {
      storage_.Del(pq_.top().key);
      pq_.pop();
    }
  }

  void RenameRecord(const Key& key, const Key& new_key) {
    std::unique_lock lock_2(pq_mtx_);
    std::priority_queue<Record> temp;

    while (!pq_.empty()) {
      if (pq_.top().key == key) {
        Record new_record = pq_.top();
        new_record.key = new_key;
        temp.push(new_record);
      } else {
        temp.push(pq_.top());
      }
      pq_.pop();
    }

    std::swap(pq_, temp);
  }

  void DeleteRecord(const Key& key) {
    std::unique_lock lock_2(pq_mtx_);
    std::priority_queue<Record> temp;

    while (!pq_.empty()) {
      if (pq_.top().key != key) {
        temp.push(pq_.top());
      }
      pq_.pop();
    }

    std::swap(pq_, temp);
  }

  void StartManagerLoop(std::chrono::seconds sleep_time) {
    while (!stop_manager_.load()) {
      DeleteExpiredRecords();

      std::mutex temp;
      std::unique_lock lock(temp);
      loop_condition_.wait_for(lock, sleep_time);
    }
  }

  void StopManager() { stop_manager_.store(true); }

  template <typename Func, typename... Args>
  auto ExecuteStorageOperation(Func func, Args... args) {
    DeleteExpiredRecords();
    std::unique_lock lock(storage_mtx_);
    auto result = std::invoke(func, storage_, std::forward<Args>(args)...);

    return result;
  }

 private:
  BaseStorage<Key, Value>& storage_;
  std::priority_queue<Record> pq_;

  std::atomic<bool> stop_manager_ = false;
  std::condition_variable loop_condition_;
  std::mutex storage_mtx_;
  std::mutex pq_mtx_;
  std::thread manager_thread_;
};

}  // namespace s21
#endif  // TRANSACTIONS_SOURCE_MODEL_COMMON_MANAGERTTL_H_