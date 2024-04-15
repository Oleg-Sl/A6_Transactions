#include "model/common/storagebenchmark.h"

namespace s21 {
template <typename Func, typename... Args>
double MeasureTime(Func func, Args... args) {
  auto start = std::chrono::steady_clock::now();
  std::invoke(func, std::forward<Args>(args)...);
  auto end = std::chrono::steady_clock::now();
  return std::chrono::duration<double, std::milli>(end - start).count();
}

Student GenerateStudent() {
  std::vector<std::string> names = {"Alice", "Bob",   "Charlie", "David",
                                    "Emma",  "Frank", "Grace",   "Henry",
                                    "Ivy",   "Jack"};
  std::vector<std::string> surnames = {"Smith", "Johnson", "Williams", "Jones",
                                       "Brown", "Davis",   "Miller",   "Wilson",
                                       "Moore", "Taylor"};
  std::vector<std::string> cities = {
      "New_York",     "Los_Angeles", "Chicago",   "Houston", "Phoenix",
      "Philadelphia", "San_Antonio", "San_Diego", "Dallas",  "San_Jose"};

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> name_dist(0, names.size() - 1);
  std::uniform_int_distribution<int> surname_dist(0, surnames.size() - 1);
  std::uniform_int_distribution<int> city_dist(0, cities.size() - 1);
  std::uniform_int_distribution<int> year_dist(1990, 2024);
  std::uniform_int_distribution<int> coins_dist(0, 999);

  std::string name = names[name_dist(gen)];
  std::string surname = surnames[surname_dist(gen)];
  int birthday_year = year_dist(gen);
  std::string city = cities[city_dist(gen)];
  int coins = coins_dist(gen);

  return Student(name, surname, birthday_year, city, coins);
}

std::string GenerateKey(size_t length) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<char> distribution('a', 'z');

  std::string key;

  for (size_t i = 0; i < length; ++i) {
    key.push_back(distribution(gen));
  }

  return key;
}

void StorageBenchmark::FillStorage(Controller<std::string, Student> controller,
                                   size_t count) {
  for (size_t i = 0; i < count; ++i) {
    controller.Set(GenerateKey(kKeyLength), GenerateStudent());
  }
}

double StorageBenchmark::MeasureGet(Controller<std::string, Student> storage,
                                    size_t repeats) {
  std::vector<std::string> keys = storage.Keys();
  std::uniform_int_distribution<int> temp(0, storage.Keys().size() - 1);
  double time = 0;

  for (size_t i = 0; i < repeats; ++i) {
    std::string key = keys[temp(gen_)];
    time += MeasureTime(
        [&storage](const std::string& k) { return storage.Get(k); }, key);
  }

  return time;
}

double StorageBenchmark::MeasureShowAll(
    Controller<std::string, Student> storage, size_t repeats) {
  double time = 0;

  for (size_t i = 0; i < repeats; ++i) {
    time += MeasureTime([&storage] { return storage.Showall(); });
  }

  return time;
}

double StorageBenchmark::MeasureFind(Controller<std::string, Student> storage,
                                     size_t repeats) {
  double time = 0;
  std::vector<Student> students = storage.Showall();
  std::uniform_int_distribution<int> temp(0, students.size() - 1);

  for (size_t i = 0; i < repeats; ++i) {
    Student student = students[temp(gen_)];
    time += MeasureTime(
        [&storage](const Student& s) { return storage.Find(s); }, student);
  }

  return time;
}

double StorageBenchmark::MeasureSet(Controller<std::string, Student> storage,
                                    size_t repeats) {
  double time = 0;

  for (size_t i = 0; i < repeats; ++i) {
    Student student = GenerateStudent();
    std::string key = GenerateKey(kKeyLength);
    time +=
        MeasureTime([&storage](const std::string& k,
                               const Student& s) { return storage.Set(k, s); },
                    key, student);
    storage.Del(key);
  }

  return time;
}

double StorageBenchmark::MeasureDel(Controller<std::string, Student> storage,
                                    size_t repeats) {
  double time = 0;
  std::vector<std::string> keys = storage.Keys();
  std::uniform_int_distribution<int> temp(0, storage.Keys().size() - 1);

  for (size_t i = 0; i < repeats; ++i) {
    std::string key = keys[temp(gen_)];
    time += MeasureTime(
        [&storage](const std::string& k) { return storage.Del(k); }, key);
    storage.Set(key, GenerateStudent());
  }

  return time;
}

}  // namespace s21