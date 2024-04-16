#ifndef TRANSACTIONS_SOURCE_MODEL_COMMON_DATA_H_
#define TRANSACTIONS_SOURCE_MODEL_COMMON_DATA_H_

#include <iomanip>
#include <istream>
#include <sstream>
#include <string>

namespace s21 {
struct StudentComparator {
  const std::string kStrIgnored = "-";
  const int kIntIgnored = std::numeric_limits<int>::min();

  bool operator()(Student student_1, Student student_2) const {
    bool name_equal =
        (student_1.name == student_2.name || student_2.name == kStrIgnored);
    bool surname_equal = (student_1.surname == student_2.surname ||
                          student_2.surname == kStrIgnored);
    bool birhday_equal = (student_1.birthday == student_2.birthday ||
                          student_2.birthday == kIntIgnored);
    bool city_equal =
        (student_1.city == student_2.city || student_2.city == kStrIgnored);
    bool coins_equal =
        (student_1.coins == student_2.coins || student_2.coins == kIntIgnored);

    return name_equal && surname_equal && birhday_equal && city_equal &&
           coins_equal;
  }
};

struct Student {
  std::string name;
  std::string surname;
  int birthday{0};
  std::string city;
  int coins{0};

  Student() = default;

  Student(const std::string& name, const std::string& surname, int birthday,
          const std::string& city, int coins)
      : name(name),
        surname(surname),
        birthday(birthday),
        city(city),
        coins(coins) {}

  std::string ToString() {
    std::stringstream ss;
    ss << std::left << std::setw(15) << name << std::left << std::setw(15)
       << surname << std::left << std::setw(10) << std::to_string(birthday)
       << std::left << std::setw(15) << city << std::left << std::setw(8)
       << std::to_string(coins);
    return ss.str();
  }

  bool operator==(const Student& other) const {
    return name == other.name && surname == other.surname &&
           birthday == other.birthday && city == other.city &&
           coins == other.coins;
  }

  friend std::ostream& operator<<(std::ostream& os, const Student& student) {
    return os << student.name << " " << student.surname << " "
              << student.birthday << " " << student.city << " "
              << student.coins;
  }

  friend std::istream& operator>>(std::istream& in, Student& student) {
    in >> student.name >> student.surname >> student.birthday >> student.city >>
        student.coins;

    return in;
  }

  bool IsEmpty() const {
    if (name == "" && surname == "" && birthday == 0 && city == "" &&
        coins == 0)
      return true;
    return false;
  }
};

class Data {
 public:
  Data() = default;
  Data(std::string key, Student value, int validity = 0)
      : key_(key), value_(value), validity_(validity) {}

  std::string GetKey() { return key_; }
  Student GetValue() { return value_; }
  int GetValidity() { return validity_; }
  void SetKey(std::string key) { key_ = key; }
  void SetValue(Student value) { value_ = value; }
  void SetValidity(int validity) { validity_ = validity; }

 private:
  std::string key_;
  Student value_;
  int validity_{0};
};
}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_MODEL_COMMON_DATA_H_