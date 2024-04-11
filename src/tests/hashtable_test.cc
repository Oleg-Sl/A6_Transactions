#include <chrono>
#include <filesystem>
#include <thread>

#include "common.h"
#include "model/hashtable/hash_table.h"

namespace s21 {

static constexpr bool kOk = true;
static constexpr bool kError = false;

TEST(HashTableSet, OneElement) {
  HashTable<std::string, Student> table;

  int status = table.Set("KEY", Student("NAME", "SURNAME", 12, "CITY", 5555));

  ASSERT_EQ(status, kOk);
  ASSERT_EQ(table.GetLoadFactor(), 1);
}

TEST(HashTableSet, ManyElement) {
  HashTable<std::string, Student> table;
  size_t count_elements = 100;

  for (size_t i = 0; i < count_elements; ++i) {
    int status = table.Set("KEY" + std::to_string(i),
                           Student("NAME", "SURNAME", 12, "CITY", 5555));
    ASSERT_EQ(status, kOk);
  }

  ASSERT_EQ(table.GetLoadFactor(), count_elements);
}

TEST(HashTableSet, ElementsWithSameKey) {
  HashTable<std::string, Student> table;

  int status_1 = table.Set("KEY", Student("NAME", "SURNAME", 12, "CITY", 5555));

  int status_2 =
      table.Set("KEY", Student("NAME2", "SURNAME2", 12, "CITY", 5555));

  ASSERT_EQ(status_1, kOk);
  ASSERT_EQ(status_2, kError);
  ASSERT_EQ(table.GetLoadFactor(), 1);
}

TEST(HashTableSet, ElementsWithSameKeyAndTTLExpired) {
  HashTable<std::string, Student> table;

  int status_1 =
      table.Set("KEY", Student("NAME", "SURNAME", 12, "CITY", 5555), 1);

  std::this_thread::sleep_for(std::chrono::milliseconds(1001));

  int status_2 =
      table.Set("KEY", Student("NAME2", "SURNAME2", 12, "CITY", 5555));

  ASSERT_EQ(status_1, kOk);
  ASSERT_EQ(status_2, kOk);
  ASSERT_EQ(table.GetLoadFactor(), 2);
}

TEST(HashTableGet, Normal) {
  HashTable<std::string, Student> table;
  Student student("NAME", "SURNAME", 12, "CITY", 5555);
  table.Set("KEY", student);

  ASSERT_EQ(table.Get("KEY"), student);
}

TEST(HashTableGet, ThrowKeyIsNotExists) {
  HashTable<std::string, Student> table;

  ASSERT_THROW(table.Get("KEY"), std::invalid_argument);
}

TEST(HashTableExists, Exists) {
  HashTable<std::string, Student> table;
  table.Set("KEY", Student("NAME", "SURNAME", 12, "CITY", 5555));

  bool status = table.Exists("KEY");

  ASSERT_EQ(status, true);
}

TEST(HashTableExists, NotExistsKey) {
  HashTable<std::string, Student> table;

  bool status = table.Exists("KEY");

  ASSERT_EQ(status, false);
}

TEST(HashTableExists, ExistsWithTTL) {
  HashTable<std::string, Student> table;
  table.Set("KEY", Student("NAME", "SURNAME", 12, "CITY", 5555), 5);

  bool status = table.Exists("KEY");

  ASSERT_EQ(status, true);
}

TEST(HashTableExists, NotExistsTTLExpired) {
  HashTable<std::string, Student> table;
  table.Set("KEY", Student("NAME", "SURNAME", 12, "CITY", 5555), 1);

  std::this_thread::sleep_for(std::chrono::milliseconds(1001));

  bool status = table.Exists("KEY");

  ASSERT_EQ(status, false);
}

TEST(HashTableUpdate, Success) {
  HashTable<std::string, Student> table;
  Student student("NAME", "SURNAME", 12, "CITY", 5555);
  Student new_student("NAME2", "SURNAME2", 13, "CITY2", 5556);
  table.Set("KEY", student);

  bool status = table.Update("KEY", new_student);

  ASSERT_EQ(table.Get("KEY"), new_student);
  ASSERT_EQ(status, true);
}

TEST(HashTableUpdate, FailKeyIsNotExists) {
  HashTable<std::string, Student> table;

  bool status =
      table.Update("KEY", Student("NAME2", "SURNAME2", 13, "CITY2", 5556));

  ASSERT_EQ(status, false);
}

TEST(HashTableUpdate, FailTTLExpired) {
  HashTable<std::string, Student> table;
  table.Set("KEY", Student("NAME", "SURNAME", 12, "CITY", 5555), 1);

  std::this_thread::sleep_for(std::chrono::milliseconds(1001));

  bool status =
      table.Update("KEY", Student("NAME2", "SURNAME2", 13, "CITY2", 5556));

  ASSERT_EQ(status, false);
}

TEST(HashTableKeys, Normal) {
  HashTable<std::string, Student> table;
  std::set<std::string> keys = {"KEY", "KEY2", "KEY3"};

  for (auto key : keys) {
    table.Set(key, Student("NAME", "SURNAME", 12, "CITY", 5555));
  }

  std::vector<std::string> result = table.Keys();
  std::set<std::string> result_set(result.begin(), result.end());

  ASSERT_EQ(keys.size(), result.size());
  ASSERT_EQ(result_set, keys);
}

TEST(HashTableKeys, KeyWithTTLExpired) {
  HashTable<std::string, Student> table;
  std::set<std::string> keys = {"KEY", "KEY2", "KEY3"};

  for (auto key : keys) {
    table.Set(key, Student("NAME", "SURNAME", 12, "CITY", 5555));
  }
  table.Set("KEY4", Student("NAME", "SURNAME", 12, "CITY", 5555), 1);
  std::this_thread::sleep_for(std::chrono::milliseconds(1001));

  std::vector<std::string> result = table.Keys();
  std::set<std::string> result_set(result.begin(), result.end());

  ASSERT_EQ(keys.size(), result.size());
  ASSERT_EQ(result_set, keys);
}

TEST(HashTableRename, Normal) {
  HashTable<std::string, Student> table;
  table.Set("KEY", Student("NAME", "SURNAME", 12, "CITY", 5555));

  bool status = table.Rename("KEY", "KEY2");

  ASSERT_EQ(table.Exists("KEY"), false);
  ASSERT_EQ(table.Exists("KEY2"), true);
  ASSERT_EQ(status, true);
}

TEST(HashTableRename, InvalidKey) {
  HashTable<std::string, Student> table;

  bool status = table.Rename("KEY", "KEY2");

  ASSERT_EQ(table.Exists("KEY"), false);
  ASSERT_EQ(table.Exists("KEY2"), false);
  ASSERT_EQ(status, false);
}

TEST(HashTableRename, KeyWithTTLExpired) {
  HashTable<std::string, Student> table;
  table.Set("KEY", Student("NAME", "SURNAME", 12, "CITY", 5555), 1);
  std::this_thread::sleep_for(std::chrono::milliseconds(1001));

  bool status = table.Rename("KEY", "KEY2");

  ASSERT_EQ(table.Exists("KEY"), false);
  ASSERT_EQ(table.Exists("KEY2"), false);
  ASSERT_EQ(status, false);
}

TEST(HashTableDel, Normal) {
  HashTable<std::string, Student> table;
  table.Set("KEY", Student("NAME", "SURNAME", 12, "CITY", 5555));

  bool status = table.Del("KEY");

  ASSERT_EQ(status, true);
  ASSERT_EQ(table.Exists("KEY"), false);
}

TEST(HashTableDel, InvalidKey) {
  HashTable<std::string, Student> table;

  bool status = table.Del("KEY");

  ASSERT_EQ(status, false);
}

TEST(HashTableDel, DelWithTTLExpired) {
  HashTable<std::string, Student> table;
  table.Set("KEY", Student("NAME", "SURNAME", 12, "CITY", 5555), 1);

  std::this_thread::sleep_for(std::chrono::milliseconds(1001));
  bool status = table.Del("KEY");

  ASSERT_EQ(status, false);
  ASSERT_EQ(table.Exists("KEY"), false);
}

TEST(HashTableTTL, Normal) {
  HashTable<std::string, Student> table;
  table.Set("KEY", Student("NAME", "SURNAME", 12, "CITY", 5555), 1);

  int res = table.Ttl("KEY");

  ASSERT_EQ(res, 1);
}

TEST(HashTableTTL, ThrowKeyWithTTLExpired) {
  HashTable<std::string, Student> table;
  table.Set("KEY", Student("NAME", "SURNAME", 12, "CITY", 5555), 1);

  std::this_thread::sleep_for(std::chrono::milliseconds(1001));

  ASSERT_THROW(table.Ttl("KEY"), std::invalid_argument);
}

TEST(HashTableTTL, ThrowInvalidKey) {
  HashTable<std::string, Student> table;

  ASSERT_THROW(table.Ttl("KEY"), std::invalid_argument);
}

TEST(HashTableExport, Normal) {
  HashTable<std::string, Student> table;
  table.Set("KEY", Student("NAME", "SURNAME", 12, "CITY", 5555));
  table.Set("KEY2", Student("NAME", "SURNAME", 12, "CITY", 5555));
  table.Set("KEY3", Student("NAME", "SURNAME", 12, "CITY", 5555));
  std::string temp_file =
      (std::filesystem::temp_directory_path() / "temp.dat").string();

  table.Export(temp_file);

  ASSERT_EQ(std::fstream(temp_file).is_open(), true);
  std::remove(temp_file.c_str());
  ASSERT_EQ(std::fstream(temp_file).is_open(), false);
}

TEST(HashTableUpload, Normal) {
  HashTable<std::string, Student> table;

  auto result = table.Upload(kSamplesDir + "student.dat");

  ASSERT_EQ(result.first, true);
  ASSERT_EQ(result.second, 2);
  ASSERT_EQ(table.GetLoadFactor(), 2);
}

TEST(HashTableUpload, FileNotExists) {
  HashTable<std::string, Student> table;

  auto result = table.Upload(kSamplesDir + "notexists.dat");

  ASSERT_EQ(result.first, false);
  ASSERT_EQ(result.second, 0);
  ASSERT_EQ(table.GetLoadFactor(), 0);
}

TEST(HashTableUpload, FileIncorrectStucture) {
  HashTable<std::string, Student> table;

  auto result = table.Upload(kSamplesDir + "student_incorrect.dat");

  ASSERT_EQ(result.first, true);
  ASSERT_EQ(result.second, 0);
  ASSERT_EQ(table.GetLoadFactor(), 0);
}

TEST(HashTableFind, OneElements) {
  HashTable<std::string, Student> table;
  Student student("NAME", "SURNAME", 12, "CITY", 5555);
  table.Set("KEY", student);

  auto keys = table.Find(student);

  ASSERT_EQ(keys[0], "KEY");
}

TEST(HashTableFind, SomeElements) {
  HashTable<std::string, Student> table;
  Student student("NAME", "SURNAME", 12, "CITY", 5555);
  std::set<std::string> keys = {"KEY", "KEY2", "KEY3"};

  for (auto key : keys) {
    table.Set(key, student);
  }

  auto keys_result = table.Find(student);
  std::set<std::string> result_set(keys_result.begin(), keys_result.end());

  ASSERT_EQ(keys, result_set);
}

TEST(HashTableFind, NotExistsElement) {
  HashTable<std::string, Student> table;

  auto keys = table.Find(Student("NAME", "SURNAME", 12, "CITY", 5555));

  ASSERT_EQ(keys.empty(), true);
}

TEST(HashTableShowAll, EmptyTable) {
  HashTable<std::string, Student> table;

  auto values = table.Showall();

  ASSERT_EQ(values.empty(), true);
}

TEST(HashTableShowAll, OneElement) {
  HashTable<std::string, Student> table;

  auto values = table.Showall();

  ASSERT_EQ(values.empty(), true);
}

TEST(HashTableShowAll, SomeElements) {
  HashTable<std::string, Student> table;
  Student student("NAME", "SURNAME", 12, "CITY", 5555);
  table.Set("KEY", student);
  table.Set("KEY2", student);
  table.Set("KEY3", student);

  auto values = table.Showall();

  ASSERT_EQ(values.size(), 3);
  ASSERT_EQ(table.GetLoadFactor(), 3);
}
}  // namespace s21
