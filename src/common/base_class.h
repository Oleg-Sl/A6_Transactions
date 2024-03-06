#ifndef TRANSACTIONS_COMMON_BASE_CLASS_H_
#define TRANSACTIONS_COMMON_BASE_CLASS_H_
#include <string>

namespace s21 {
 class BaseClass {
public:
	virtual std::string Set(std::string param) {};
	virtual std::string Get(std::string param) {};
	virtual std::string Exists(std::string param) {};
	virtual std::string Del(std::string param) {};
	virtual std::string Update(std::string param) {};
	virtual std::string Keys(std::string param) {};
	virtual std::string Rename(std::string param) {};
	virtual std::string Ttl(std::string param) {};
	virtual std::string Find(std::string param) {};
	virtual std::string Showall(std::string param) {};
	virtual std::string Upload(std::string param) {};
	virtual std::string Export(std::string param) {};
};
}  // namespace s21
#endif  // TRANSACTIONS_COMMON_BASE_CLASS_H_
