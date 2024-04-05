#include "controller.h"

namespace s21 {
	Controller::Controller(std::string obj)	{
		if(obj == "hash"){
			//model_ = new HashTable();
		}
		else if (obj == "bst") {
			model_ = new SelfBalancingBinarySearchTree();
		}
		else {
			//model_ = new Three();
		}
	}

	Controller::~Controller() {
		if (model_) delete model_;
	}

	bool Controller::Set(std::string key, Student student, int validity) {
		return model_->Set(key, student, validity);
	}	

	Student Controller::Get(std::string key) {
		return model_->Get(key);
	}

	bool Controller::Exists(std::string key) {
		return model_->Exists(key);
	}

	bool Controller::Del(std::string key) {
		return model_->Del(key);
	}

	bool Controller::Update(std::string key, Student student) {
		return model_->Update(key, student);
	}

	std::vector<std::string> Controller::Keys() {
		return model_->Keys();
	}

	bool Controller::Rename(std::string key, std::string new_key) {
		return model_->Rename(key, new_key);
	}

	std::string Controller::Ttl(std::string param) {
		return model_->Ttl(param);
	}

	std::vector<std::string> Controller::Find(Student student) {
		return model_->Find(student);
	}

	std::vector<Student> Controller::Showall() {
		return model_->Showall();
	}

	std::pair<bool, int> Controller::Upload(std::string path) {
		return model_->Upload(path);
	}

	std::pair<bool, int> Controller::Export(std::string path) {
		return model_->Export(path);
	}
}