#ifndef TRANSACTIONS_SOURCE_MODEL_COMMON_MANAGERTTL_H_
#define TRANSACTIONS_SOURCE_MODEL_COMMON_MANAGERTTL_H_

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <list>
#include <mutex>
#include <thread>

#include "model/common/basestorage.h"

namespace s21 {

template <typename Key, typename Value>
class ManagerTTL {
 public:
  struct Record {
    Key key;
    std::chrono::steady_clock::time_point die_time;

    Record(Key key, int TTL)
        : key(key),
          die_time(std::chrono::seconds(TTL) +
                   std::chrono::steady_clock::now()) {}
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
    if (record.die_time <= std::chrono::steady_clock::now()) {
      return;
    }

    std::unique_lock lock(records_mtx_);
    auto it = records_.begin();
    while (it != records_.end() && it->die_time < record.die_time) {
      ++it;
    }

    records_.insert(it, record);
  }

  void DeleteExpiredRecords() {
    std::unique_lock lock(storage_mtx_);
    std::unique_lock lock2(records_mtx_);
    auto now = std::chrono::steady_clock::now();

    while (!records_.empty() && records_.begin()->die_time <= now) {
      storage_.Del(records_.begin()->key);
      records_.erase(records_.begin());
    }
  }

  void RenameRecord(const Key& key, const Key& new_key) {
    std::unique_lock lock_2(records_mtx_);

    for (auto it = records_.begin(); it != records_.end(); ++it) {
      if (it->key == key) {
        it->key = new_key;
      }
    }
  }

  void DeleteRecord(const Key& key) {
    std::unique_lock lock_2(records_mtx_);

    for (auto it = records_.begin(); it != records_.end(); ++it) {
      if (it->key == key) {
        records_.erase(it);
        break;
      }
    }
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
  std::list<Record> records_;

  std::atomic<bool> stop_manager_ = false;
  std::condition_variable loop_condition_;
  std::mutex storage_mtx_;
  std::mutex records_mtx_;
  std::thread manager_thread_;
};

}  // namespace s21
#endif  // TRANSACTIONS_SOURCE_MODEL_COMMON_MANAGERTTL_H_