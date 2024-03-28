#include <model/hashtable/hash_table.h>

namespace s21 {

bool HashTable::Set(std::string key, Student student, int validity) {}
Student HashTable::Get(std::string key) {}
bool HashTable::Exists(std::string key) {}
bool HashTable::Del(std::string key) {}
bool HashTable::Update(std::string key, Student student) {}
std::vector<std::string> HashTable::Keys() {}
bool HashTable::Rename(std::string key, std::string new_key) {}
std::string HashTable::Ttl(std::string param) {}
std::vector<std::string> HashTable::Find(Student student) {}
std::vector<Student> HashTable::Showall() {}
std::pair<bool, int> HashTable::Upload(std::string path) {}
std::pair<bool, int> HashTable::Export(std::string path) {}

} // namespace s21