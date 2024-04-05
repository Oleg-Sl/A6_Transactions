#include <chrono>
#include <thread>

#include "common.h"
#include "model/hashtable/hash_table.h"

static constexpr bool kOk = true;
static constexpr bool kError = false;

TEST(HashTableSet, OneElement) {
  s21::HashTable<std::string, s21::Student> table;

  int status =
      table.Set("KEY", s21::Student("NAME", "SURNAME", 12, "CITY", 5555));

  ASSERT_EQ(status, kOk);
  ASSERT_EQ(table.GetLoadFactor(), 1);
}

TEST(HashTableSet, ManyElement) {
  s21::HashTable<std::string, s21::Student> table;
  size_t count_elements = 100;

  for (size_t i = 0; i < count_elements; ++i) {
    int status = table.Set("KEY" + std::to_string(i),
                           s21::Student("NAME", "SURNAME", 12, "CITY", 5555));
    ASSERT_EQ(status, kOk);
  }

  ASSERT_EQ(table.GetLoadFactor(), count_elements);
}

TEST(HashTableSet, ElementsWithSameKey) {
  s21::HashTable<std::string, s21::Student> table;

  int status_1 =
      table.Set("KEY", s21::Student("NAME", "SURNAME", 12, "CITY", 5555));

  int status_2 =
      table.Set("KEY", s21::Student("NAME2", "SURNAME2", 12, "CITY", 5555));

  ASSERT_EQ(status_1, kOk);
  ASSERT_EQ(status_2, kError);
  ASSERT_EQ(table.GetLoadFactor(), 1);
}

TEST(HashTableSet, ElementsWithSameKeyAndTTLExpired) {
  s21::HashTable<std::string, s21::Student> table;

  int status_1 =
      table.Set("KEY", s21::Student("NAME", "SURNAME", 12, "CITY", 5555), 1);

  std::this_thread::sleep_for(std::chrono::milliseconds(1001));

  int status_2 =
      table.Set("KEY", s21::Student("NAME2", "SURNAME2", 12, "CITY", 5555));

  ASSERT_EQ(status_1, kOk);
  ASSERT_EQ(status_2, kOk);
  ASSERT_EQ(table.GetLoadFactor(), 2);
}

TEST(HashTableExists, Exists) {
  s21::HashTable<std::string, s21::Student> table;
  table.Set("KEY", s21::Student("NAME", "SURNAME", 12, "CITY", 5555));

  bool status = table.Exists("KEY");

  ASSERT_EQ(status, true);
}

TEST(HashTableExists, NotExistsKey) {
  s21::HashTable<std::string, s21::Student> table;

  bool status = table.Exists("KEY");

  ASSERT_EQ(status, false);
}

TEST(HashTableExists, ExistsWithTTL) {
  s21::HashTable<std::string, s21::Student> table;
  table.Set("KEY", s21::Student("NAME", "SURNAME", 12, "CITY", 5555), 5);

  bool status = table.Exists("KEY");

  ASSERT_EQ(status, true);
}

TEST(HashTableExists, NotExistsTTLExpired) {
  s21::HashTable<std::string, s21::Student> table;
  table.Set("KEY", s21::Student("NAME", "SURNAME", 12, "CITY", 5555), 1);

  std::this_thread::sleep_for(std::chrono::milliseconds(1001));

  bool status = table.Exists("KEY");

  ASSERT_EQ(status, false);
}

TEST(HashTableUpdate, Success) {
  s21::HashTable<std::string, s21::Student> table;
  table.Set("KEY", s21::Student("NAME", "SURNAME", 12, "CITY", 5555));

  bool status =
      table.Update("KEY", s21::Student("NAME2", "SURNAME2", 13, "CITY2", 5556));

  // TODO: CHECK Student
  ASSERT_EQ(status, true);
}

TEST(HashTableUpdate, FailKeyIsNotExists) {
  s21::HashTable<std::string, s21::Student> table;

  bool status =
      table.Update("KEY", s21::Student("NAME2", "SURNAME2", 13, "CITY2", 5556));

  ASSERT_EQ(status, false);
}

TEST(HashTableUpdate, FailTTLExpired) {
  s21::HashTable<std::string, s21::Student> table;
  table.Set("KEY", s21::Student("NAME", "SURNAME", 12, "CITY", 5555), 1);

  std::this_thread::sleep_for(std::chrono::milliseconds(1001));

  bool status =
      table.Update("KEY", s21::Student("NAME2", "SURNAME2", 13, "CITY2", 5556));

  ASSERT_EQ(status, false);
}

TEST(HashTableKeys, Normal) {
  s21::HashTable<std::string, s21::Student> table;
  std::set<std::string> keys = {"KEY", "KEY2", "KEY3"};

  for (auto key : keys) {
    table.Set(key, s21::Student("NAME", "SURNAME", 12, "CITY", 5555));
  }

  std::vector<std::string> result = table.Keys();
  std::set<std::string> result_set(result.begin(), result.end());

  ASSERT_EQ(keys.size(), result.size());
  ASSERT_EQ(result_set, keys);
}

TEST(HashTableRename, Normal) {
  s21::HashTable<std::string, s21::Student> table;
  table.Set("KEY", s21::Student("NAME", "SURNAME", 12, "CITY", 5555));

  bool status = table.Rename("KEY", "KEY2");

  ASSERT_EQ(table.Exists("KEY"), false);
  ASSERT_EQ(table.Exists("KEY2"), true);
  ASSERT_EQ(status, true);
}