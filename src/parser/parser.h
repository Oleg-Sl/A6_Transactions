#ifndef TRANSACTIONS_PARSER_PARSER_H_
#define TRANSACTIONS_PARSER_PARSER_H_
#include <string>
#include <vector>
#include <sstream>

namespace s21 {

class Parser {
public:
	Parser() = default;
	std::vector<std::string> Parse(std::string str){
        std::vector<std::string> param;
        std::stringstream ss(str);
        std::string token;
        while (ss >> token) {
            param.push_back(token);
        }
        return param;
	}
};

}  // namespace s21
#endif  // TRANSACTIONS_PARSER_PARSER_H_