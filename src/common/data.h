#ifndef TRANSACTIONS_COMMON_DATA_H_
#define TRANSACTIONS_COMMON_DATA_H_
#include <string>

namespace s21 {
	struct Student	{
	public:
		std::string name = "";
		std::string surname = "";
		int year_of_birthday = 0;
		std::string city = "";
		int count_of_points = 0;
	};

	class Data {
	public:
		Data() = default;
		Data(std::string key, Student value, int validity = 0) : key_(key), value_(value), validity_(validity) {}
		std::string GetKey() { return key_; }
		Student GetValue() { return value_; }
		int GetValidity() { return validity_; }
		void SetKey(std::string key) { key_ = key; }
		void SetKey(Student value) { value_ = value; }
		void SetKey(int validity) { validity_ = validity; }

	private:
		std::string key_ = "";
		Student value_;
		int validity_ = 0;

	};
}  // namespace s21
#endif  // TRANSACTIONS_COMMON_DATA_H_
