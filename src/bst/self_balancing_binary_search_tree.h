#ifndef TRANSACTIONS_BST_SELF_BALANCING_BINARY_SEARCH_TREE_H_
#define TRANSACTIONS_BST_SELF_BALANCING_BINARY_SEARCH_TREE_H_

#include "../common/base_class.h"

namespace s21 {

class SelfBalancingBinarySearchTree:BaseClass
{
public:
	std::string Set(std::string param);
	std::string Get(std::string param);
	std::string Exists(std::string param);
	std::string Del(std::string param);
	std::string Update(std::string param);
	std::string Keys(std::string param);
	std::string Rename(std::string param);
	std::string Ttl(std::string param);
	std::string Find(std::string param);
	std::string Showall(std::string param);
	std::string Upload(std::string param);
	std::string Export(std::string param);
};

}  // namespace s21

#endif  // TRANSACTIONS_BST_SELF_BALANCING_BINARY_SEARCH_TREE_H_
