#ifndef TRANSACTIONS_COMMON_BASE_CLASS_H_
#define TRANSACTIONS_COMMON_BASE_CLASS_H_
#include <string>
#include "data.h"

namespace s21 {
	class BaseClass {
	public:
		//<ключ> <Фамилия> <Имя> <Год рождения> <Город> <Число текущих коинов> EX <время в секундах>
		virtual std::string Set(std::string key, Student student, int validity=0) = 0;
		virtual Student Get(std::string key) = 0;
		virtual bool Exists(std::string key) = 0;
		virtual bool Del(std::string key) = 0;
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
