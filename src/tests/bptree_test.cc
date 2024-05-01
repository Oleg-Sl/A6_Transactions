#include "common.h"
#include "model/bplustree/b_plus_tree.h"
#include "model/common/student.h"

namespace s21 {

TEST(BPlusTreeSet, OneElement) {
  BPlusTree<std::string, Student> table;

  int status = table.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});

  ASSERT_EQ(status, true);
}

TEST(BPlusTreeSet, ManyElement) {
  BPlusTree<std::string, Student> table;
  size_t count_elements = 100;

  for (size_t i = 0; i < count_elements; ++i) {
    int status = table.Set("KEY" + std::to_string(i),
                           Student{"NAME", "SURNAME", 12, "CITY", 5555});
    ASSERT_EQ(status, true);
  }
}

TEST(BPlusTreeSet, ElementsWithSameKey) {
  BPlusTree<std::string, Student> table;

  int status_1 = table.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});

  int status_2 =
      table.Set("KEY", Student{"NAME2", "SURNAME2", 12, "CITY", 5555});

  ASSERT_EQ(status_1, true);
  ASSERT_EQ(status_2, false);
}

TEST(BPlusTreeGet, Normal) {
  BPlusTree<std::string, Student> table;
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  table.Set("KEY", student);

  ASSERT_EQ(table.Get("KEY"), student);
}

TEST(BPlusTreeGet, ThrowKeyIsNotExists) {
  BPlusTree<std::string, Student> table;

  ASSERT_THROW(table.Get("KEY"), std::invalid_argument);
}

TEST(BPlusTreeExists, Exists) {
  BPlusTree<std::string, Student> table;
  table.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});

  bool status = table.Exists("KEY");

  ASSERT_EQ(status, true);
}

TEST(BPlusTreeExists, NotExistsKey) {
  BPlusTree<std::string, Student> table;

  bool status = table.Exists("KEY");

  ASSERT_EQ(status, false);
}

TEST(BPlusTreeUpdate, Success) {
  BPlusTree<std::string, Student> table;
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  Student new_student{"NAME2", "SURNAME2", 13, "CITY2", 5556};
  table.Set("KEY", student);

  bool status = table.Update("KEY", new_student);

  ASSERT_EQ(table.Get("KEY"), new_student);
  ASSERT_EQ(status, true);
}

TEST(BPlusTreeUpdate, FailKeyIsNotExists) {
  BPlusTree<std::string, Student> table;

  bool status =
      table.Update("KEY", Student{"NAME2", "SURNAME2", 13, "CITY2", 5556});

  ASSERT_EQ(status, false);
}

TEST(BPlusTreeKeys, Normal) {
  BPlusTree<std::string, Student> table;
  std::set<std::string> keys = {"KEY", "KEY2", "KEY3"};

  for (auto key : keys) {
    table.Set(key, Student{"NAME", "SURNAME", 12, "CITY", 5555});
  }

  std::vector<std::string> result = table.Keys();
  std::set<std::string> result_set(result.begin(), result.end());

  ASSERT_EQ(keys.size(), result.size());
  ASSERT_EQ(result_set, keys);
}

TEST(BPlusTreeRename, Normal) {
  BPlusTree<std::string, Student> table;
  table.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});

  bool status = table.Rename("KEY", "KEY2");

  ASSERT_EQ(table.Exists("KEY"), false);
  ASSERT_EQ(table.Exists("KEY2"), true);
  ASSERT_EQ(status, true);
}

TEST(BPlusTreeRename, InvalidKey) {
  BPlusTree<std::string, Student> table;

  bool status = table.Rename("KEY", "KEY2");

  ASSERT_EQ(table.Exists("KEY"), false);
  ASSERT_EQ(table.Exists("KEY2"), false);
  ASSERT_EQ(status, false);
}

TEST(BPlusTreeDel, Normal) {
  BPlusTree<std::string, Student> table;
  table.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});

  bool status = table.Del("KEY");

  ASSERT_EQ(status, true);
  ASSERT_EQ(table.Exists("KEY"), false);
}

TEST(BPlusTreeDel, InvalidKey) {
  BPlusTree<std::string, Student> table;

  bool status = table.Del("KEY");

  ASSERT_EQ(status, false);
}

TEST(BPlusTreeFind, OneElements) {
  BPlusTree<std::string, Student> table;
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  table.Set("KEY", student);

  auto keys = table.Find(student);

  ASSERT_EQ(keys[0], "KEY");
}

TEST(BPlusTreeFind, SomeElements) {
  BPlusTree<std::string, Student> table;
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  std::set<std::string> keys = {"KEY", "KEY2", "KEY3"};

  for (auto key : keys) {
    table.Set(key, student);
  }

  auto keys_result = table.Find(student);
  std::set<std::string> result_set(keys_result.begin(), keys_result.end());

  ASSERT_EQ(keys, result_set);
}

TEST(BPlusTreeFind, NotExistsElement) {
  BPlusTree<std::string, Student> table;

  auto keys = table.Find(Student{"NAME", "SURNAME", 12, "CITY", 5555});

  ASSERT_EQ(keys.empty(), true);
}

TEST(BPlusTreeShowAll, EmptyTable) {
  BPlusTree<std::string, Student> table;

  auto values = table.Showall();

  ASSERT_EQ(values.empty(), true);
}

TEST(BPlusTreeShowAll, OneElement) {
  BPlusTree<std::string, Student> table;

  auto values = table.Showall();

  ASSERT_EQ(values.empty(), true);
}

TEST(BPlusTreeShowAll, SomeElements) {
  BPlusTree<std::string, Student> table;
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  table.Set("KEY", student);
  table.Set("KEY2", student);
  table.Set("KEY3", student);

  auto values = table.Showall();

  ASSERT_EQ(values.size(), 3);
}
}  // namespace s21
