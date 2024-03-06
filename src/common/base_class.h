#ifndef TRANSACTIONS_COMMON_BASE_CLASS_H_
#define TRANSACTIONS_COMMON_BASE_CLASS_H_
#include <string>

namespace s21 {
 class BaseClass {
public:
	virtual std::string Set(std::string param) = 0;
	virtual std::string Get(std::string param) = 0;
	virtual std::string Exists(std::string param) = 0;
	virtual std::string Del(std::string param) = 0;
	virtual std::string Update(std::string param) = 0;
	virtual std::string Keys(std::string param) = 0;
	virtual std::string Rename(std::string param) = 0;
	virtual std::string Ttl(std::string param) = 0;
	virtual std::string Find(std::string param) = 0;
	virtual std::string Showall(std::string param) = 0;
	virtual std::string Upload(std::string param) = 0;
	virtual std::string Export(std::string param) = 0;
};
}  // namespace s21
#endif  // TRANSACTIONS_COMMON_BASE_CLASS_H_
