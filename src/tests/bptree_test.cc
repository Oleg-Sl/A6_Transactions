#include <chrono>
#include <filesystem>
#include <thread>

#include "common.h"
#include "model/common/student.h"
#include "model/bplustree/b_plus_tree.h"

namespace s21 {

static constexpr bool kOk = true;
static constexpr bool kError = false;

TEST(BPlusTreeSet, OneElement) {
  BPlusTree<std::string, Student> tree;

  int status = tree.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});

  ASSERT_EQ(status, kOk);
}

TEST(BPlusTreeSet, ManyElement) {
  BPlusTree<std::string, Student> tree;
  size_t count_elements = 1000;

  for (size_t i = 0; i < count_elements; ++i) {
    int status = tree.Set("KEY" + std::to_string(i),
                           Student{"NAME", "SURNAME", 12, "CITY", 5555});
    ASSERT_EQ(status, kOk);
  }
}

TEST(BPlusTreeSet, ElementsWithSameKey) {
  BPlusTree<std::string, Student> tree;

  int status_1 = tree.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});

  int status_2 =
      tree.Set("KEY", Student{"NAME2", "SURNAME2", 12, "CITY", 5555});

  ASSERT_EQ(status_1, kOk);
  ASSERT_EQ(status_2, kError);
}

// TEST(BPlusTreeSet, ElementsWithSameKeyAndTTLExpired) {
//   BPlusTree<std::string, Student> tree;

//   int status_1 =
//       tree.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555}, 1);

//   std::this_thread::sleep_for(std::chrono::milliseconds(1001));

//   int status_2 =
//       tree.Set("KEY", Student{"NAME2", "SURNAME2", 12, "CITY", 5555});

//   ASSERT_EQ(status_1, kOk);
//   ASSERT_EQ(status_2, kOk);
//   ASSERT_EQ(tree.GetLoadFactor(), 2);
// }

TEST(BPlusTreeGet, Normal) {
  BPlusTree<std::string, Student> tree;
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  tree.Set("KEY", student);

  ASSERT_EQ(tree.Get("KEY"), student);
}

TEST(BPlusTreeGet, ThrowKeyIsNotExists) {
  BPlusTree<std::string, Student> tree;

  ASSERT_THROW(tree.Get("KEY"), std::invalid_argument);
}

TEST(BPlusTreeExists, Exists) {
  BPlusTree<std::string, Student> tree;
  tree.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});

  bool status = tree.Exists("KEY");

  ASSERT_EQ(status, true);
}

TEST(BPlusTreeExists, NotExistsKey) {
  BPlusTree<std::string, Student> tree;

  bool status = tree.Exists("KEY");

  ASSERT_EQ(status, false);
}

// TEST(BPlusTreeExists, ExistsWithTTL) {
//   BPlusTree<std::string, Student> tree;
//   tree.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555}, 5);

//   bool status = tree.Exists("KEY");

//   ASSERT_EQ(status, true);
// }

// TEST(BPlusTreeExists, NotExistsTTLExpired) {
//   BPlusTree<std::string, Student> tree;
//   tree.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555}, 1);

//   std::this_thread::sleep_for(std::chrono::milliseconds(1001));

//   bool status = tree.Exists("KEY");

//   ASSERT_EQ(status, false);
// }

TEST(BPlusTreeUpdate, Success) {
  BPlusTree<std::string, Student> tree;
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  Student new_student{"NAME2", "SURNAME2", 13, "CITY2", 5556};
  tree.Set("KEY", student);

  bool status = tree.Update("KEY", new_student);

  ASSERT_EQ(tree.Get("KEY"), new_student);
  ASSERT_EQ(status, true);
}

TEST(BPlusTreeUpdate, FailKeyIsNotExists) {
  BPlusTree<std::string, Student> tree;

  bool status =
      tree.Update("KEY", Student{"NAME2", "SURNAME2", 13, "CITY2", 5556});

  ASSERT_EQ(status, false);
}

// TEST(BPlusTreeUpdate, FailTTLExpired) {
//   BPlusTree<std::string, Student> tree;
//   tree.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555}, 1);

//   std::this_thread::sleep_for(std::chrono::milliseconds(1001));

//   bool status =
//       tree.Update("KEY", Student{"NAME2", "SURNAME2", 13, "CITY2", 5556});

//   ASSERT_EQ(status, false);
// }

TEST(BPlusTreeKeys, Normal) {
  BPlusTree<std::string, Student> tree;
  std::set<std::string> keys = {"KEY", "KEY2", "KEY3"};

  for (auto key : keys) {
    tree.Set(key, Student{"NAME", "SURNAME", 12, "CITY", 5555});
  }

  std::vector<std::string> result = tree.Keys();
  std::set<std::string> result_set(result.begin(), result.end());

  ASSERT_EQ(keys.size(), result.size());
  ASSERT_EQ(result_set, keys);
}

// TEST(BPlusTreeKeys, KeyWithTTLExpired) {
//   BPlusTree<std::string, Student> tree;
//   std::set<std::string> keys = {"KEY", "KEY2", "KEY3"};

//   for (auto key : keys) {
//     tree.Set(key, Student{"NAME", "SURNAME", 12, "CITY", 5555});
//   }
//   tree.Set("KEY4", Student{"NAME", "SURNAME", 12, "CITY", 5555}, 1);
//   std::this_thread::sleep_for(std::chrono::milliseconds(1001));

//   std::vector<std::string> result = tree.Keys();
//   std::set<std::string> result_set(result.begin(), result.end());

//   ASSERT_EQ(keys.size(), result.size());
//   ASSERT_EQ(result_set, keys);
// }

TEST(BPlusTreeRename, Normal) {
  BPlusTree<std::string, Student> tree;
  tree.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});

  bool status = tree.Rename("KEY", "KEY2");

  ASSERT_EQ(tree.Exists("KEY"), false);
  ASSERT_EQ(tree.Exists("KEY2"), true);
  ASSERT_EQ(status, true);
}

TEST(BPlusTreeRename, InvalidKey) {
  BPlusTree<std::string, Student> tree;

  bool status = tree.Rename("KEY", "KEY2");

  ASSERT_EQ(tree.Exists("KEY"), false);
  ASSERT_EQ(tree.Exists("KEY2"), false);
  ASSERT_EQ(status, false);
}

// TEST(BPlusTreeRename, KeyWithTTLExpired) {
//   BPlusTree<std::string, Student> tree;
//   tree.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555}, 1);
//   std::this_thread::sleep_for(std::chrono::milliseconds(1001));

//   bool status = tree.Rename("KEY", "KEY2");

//   ASSERT_EQ(tree.Exists("KEY"), false);
//   ASSERT_EQ(tree.Exists("KEY2"), false);
//   ASSERT_EQ(status, false);
// }

TEST(BPlusTreeDel, Normal) {
  BPlusTree<std::string, Student> tree;
  tree.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});

  bool status = tree.Del("KEY");

  ASSERT_EQ(status, true);
  ASSERT_EQ(tree.Exists("KEY"), false);
}

TEST(BPlusTreeDel, InvalidKey) {
  BPlusTree<std::string, Student> tree;

  bool status = tree.Del("KEY");

  ASSERT_EQ(status, false);
}

// TEST(BPlusTreeDel, DelWithTTLExpired) {
//   BPlusTree<std::string, Student> tree;
//   tree.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555}, 1);

//   std::this_thread::sleep_for(std::chrono::milliseconds(1001));
//   bool status = tree.Del("KEY");

//   ASSERT_EQ(status, false);
//   ASSERT_EQ(tree.Exists("KEY"), false);
// }

// TEST(BPlusTreeTTL, Normal) {
//   BPlusTree<std::string, Student> tree;
//   tree.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555}, 1);

//   int res = tree.Ttl("KEY");

//   ASSERT_EQ(res, 1);
// }

// TEST(BPlusTreeTTL, ThrowKeyWithTTLExpired) {
//   BPlusTree<std::string, Student> tree;
//   tree.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555}, 1);

//   std::this_thread::sleep_for(std::chrono::milliseconds(1001));

//   ASSERT_THROW(tree.Ttl("KEY"), std::invalid_argument);
// }

// TEST(BPlusTreeTTL, ThrowInvalidKey) {
//   BPlusTree<std::string, Student> tree;

//   ASSERT_THROW(tree.Ttl("KEY"), std::invalid_argument);
// }

TEST(BPlusTreeExport, Normal) {
  BPlusTree<std::string, Student> tree;
  tree.Set("KEY", Student{"NAME", "SURNAME", 12, "CITY", 5555});
  tree.Set("KEY2", Student{"NAME", "SURNAME", 12, "CITY", 5555});
  tree.Set("KEY3", Student{"NAME", "SURNAME", 12, "CITY", 5555});
  std::string temp_file =
      (std::filesystem::temp_directory_path() / "temp.dat").string();

  tree.Export(temp_file);

  ASSERT_EQ(std::fstream(temp_file).is_open(), true);
  std::remove(temp_file.c_str());
  ASSERT_EQ(std::fstream(temp_file).is_open(), false);
}

TEST(BPlusTreeUpload, Normal) {
  BPlusTree<std::string, Student> tree;

  auto result = tree.Upload(kSamplesDir + "student.dat");

  ASSERT_EQ(result.first, true);
  ASSERT_EQ(result.second, 20);
}

TEST(BPlusTreeUpload, FileNotExists) {
  BPlusTree<std::string, Student> tree;

  auto result = tree.Upload(kSamplesDir + "notexists.dat");

  ASSERT_EQ(result.first, false);
  ASSERT_EQ(result.second, 0);
}

TEST(BPlusTreeUpload, FileIncorrectStucture) {
  BPlusTree<std::string, Student> tree;

  auto result = tree.Upload(kSamplesDir + "student_incorrect.dat");

  ASSERT_EQ(result.first, true);
  ASSERT_EQ(result.second, 0);
}

TEST(BPlusTreeFind, OneElements) {
  BPlusTree<std::string, Student> tree;
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  tree.Set("KEY", student);

  auto keys = tree.Find(student);

  ASSERT_EQ(keys[0], "KEY");
}

TEST(BPlusTreeFind, SomeElements) {
  BPlusTree<std::string, Student> tree;
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  std::set<std::string> keys = {"KEY", "KEY2", "KEY3"};

  for (auto key : keys) {
    tree.Set(key, student);
  }

  auto keys_result = tree.Find(student);
  std::set<std::string> result_set(keys_result.begin(), keys_result.end());

  ASSERT_EQ(keys, result_set);
}

TEST(BPlusTreeFind, NotExistsElement) {
  BPlusTree<std::string, Student> tree;

  auto keys = tree.Find(Student{"NAME", "SURNAME", 12, "CITY", 5555});

  ASSERT_EQ(keys.empty(), true);
}

TEST(BPlusTreeShowAll, Emptytree) {
  BPlusTree<std::string, Student> tree;

  auto values = tree.Showall();

  ASSERT_EQ(values.empty(), true);
}

TEST(BPlusTreeShowAll, OneElement) {
  BPlusTree<std::string, Student> tree;

  auto values = tree.Showall();

  ASSERT_EQ(values.empty(), true);
}

TEST(BPlusTreeShowAll, SomeElements) {
  BPlusTree<std::string, Student> tree;
  Student student{"NAME", "SURNAME", 12, "CITY", 5555};
  tree.Set("KEY", student);
  tree.Set("KEY2", student);
  tree.Set("KEY3", student);

  auto values = tree.Showall();

  ASSERT_EQ(values.size(), 3);
}
}  // namespace s21
