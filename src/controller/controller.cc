#include "controller.h"

namespace s21 {

	bool Controller::Set(Data data) {
		return bst_.Set(data);
	}

	Student Controller::Get(std::string key) {
		return bst_.Get(key);
	}

	bool Controller::Exists(std::string key) {
		return bst_.Exists(key);
	}

	bool Controller::Del(std::string key) {
		return bst_.Del(key);
	}

	bool Controller::Update(std::string key, Student student) {
		return bst_.Update(key, student);
	}

	std::vector<std::string> Controller::Keys() {
		return bst_.Keys();
	}

	bool Controller::Rename(std::string key, std::string new_key) {
		return bst_.Rename(key, new_key);
	}

	std::string Controller::Ttl(std::string param) {
		return bst_.Ttl(param);
	}

	std::vector<std::string> Controller::Find(Student student) {
		return bst_.Find(student);
	}

	std::vector<Student> Controller::Showall() {
		return bst_.Showall();
	}

	std::pair<bool, int> Controller::Upload(std::string path) {
		return bst_.Upload(path);
	}

	std::pair<bool, int> Controller::Export(std::string path) {
		return bst_.Export(path);
	}
}