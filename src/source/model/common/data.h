#ifndef TRANSACTIONS_SOURCE_MODEL_COMMON_DATA_H_
#define TRANSACTIONS_SOURCE_MODEL_COMMON_DATA_H_

#include <istream>
#include <limits>
#include <ostream>
#include <string>

namespace s21 {
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
    return in >> student.name >> student.surname >> student.birthday >>
           student.city >> student.coins;
  }
};

struct StudentComparator {
  const std::string kStrIgnored = "-";
  const int kIntIgnored = std::numeric_limits<int>::min();

  bool operator()(const Student& student_1, const Student& student_2) const {
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

}  // namespace s21

#endif  // TRANSACTIONS_SOURCE_MODEL_COMMON_DATA_H_