#ifndef TRANSACTIONS_CONTROLLER_CONTROLLER_H_
#define TRANSACTIONS_CONTROLLER_CONTROLLER_H_

#include "../bst/self_balancing_binary_search_tree.h"

namespace s21 {
class Controller {
public:

	Controller(std::string obj);
	~Controller();
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
	BaseClass* model_ = nullptr;
};

}  // namespace s21
#endif  // TRANSACTIONS_CONTROLLER_CONTROLLER_H_