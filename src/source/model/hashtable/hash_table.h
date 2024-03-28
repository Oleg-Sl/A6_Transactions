#include <model/common/base_class.h>

namespace s21 {
class HashTable : BaseClass {
public:
  bool Set(std::string key, Student student, int validity = 0);
  Student Get(std::string key);
  bool Exists(std::string key);
  bool Del(std::string key);
  bool Update(std::string key, Student student);
  std::vector<std::string> Keys();
  bool Rename(std::string key, std::string new_key);
  std::string Ttl(std::string param);
  std::vector<std::string> Find(Student student);
  std::vector<Student> Showall();
  std::pair<bool, int> Upload(std::string path);
  std::pair<bool, int> Export(std::string path);

private:
};
} // namespace s21