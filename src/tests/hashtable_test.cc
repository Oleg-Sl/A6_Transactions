#include <chrono>
#include <thread>

#include "common.h"
#include "model/hashtable/hash_table.h"

static constexpr int kOk =
    static_cast<int>(s21::HashTable<s21::Student>::StatusSet::kOk);
static constexpr int kAlreadyExists = static_cast<int>(
    s21::HashTable<s21::Student>::StatusSet::kElementAlreadyExists);

TEST(HashTableSet, OneElement) {
  s21::HashTable<s21::Student> table;

  int status =
      table.Set("KEY", s21::Student("NAME", "SURNAME", 12, "CITY", 5555));

  ASSERT_EQ(status, kOk);
  ASSERT_EQ(table.GetLoadFactor(), 1);
}

TEST(HashTableSet, ManyElement) {
  s21::HashTable<s21::Student> table;
  size_t count_elements = 100;

  for (size_t i = 0; i < count_elements; ++i) {
    int status = table.Set("KEY" + std::to_string(i),
                           s21::Student("NAME", "SURNAME", 12, "CITY", 5555));
    ASSERT_EQ(status, kOk);
  }

  ASSERT_EQ(table.GetLoadFactor(), count_elements);
}

TEST(HashTableSet, ElementsWithSameKey) {
  s21::HashTable<s21::Student> table;

  int status_1 =
      table.Set("KEY", s21::Student("NAME", "SURNAME", 12, "CITY", 5555));

  int status_2 =
      table.Set("KEY", s21::Student("NAME2", "SURNAME2", 12, "CITY", 5555));

  ASSERT_EQ(status_1, kOk);
  ASSERT_EQ(status_2, kAlreadyExists);
  ASSERT_EQ(table.GetLoadFactor(), 1);
}

TEST(HashTableSet, ElementsWithSameKeyAndTTLExpired) {
  s21::HashTable<s21::Student> table;

  int status_1 =
      table.Set("KEY", s21::Student("NAME", "SURNAME", 12, "CITY", 5555), 1);

  std::this_thread::sleep_for(std::chrono::seconds(2));

  int status_2 =
      table.Set("KEY", s21::Student("NAME2", "SURNAME2", 12, "CITY", 5555), 1);

  ASSERT_EQ(status_1, kOk);
  ASSERT_EQ(status_2, kOk);
  ASSERT_EQ(table.GetLoadFactor(), 2);
}