#include "self_balancing_binary_search_tree.h"

namespace s21 {

	std::string SelfBalancingBinarySearchTree::Set(std::string key, std::string name, std::string surname, int birthday, std::string city, int coins, int validity) {
		Data data(key, Student(name, surname, birthday, city, coins), validity);
		Pointer tmp = new Node(data);
		if (!root_) {
			root_ = tmp;
			root_->color = Color::Black;
			root_->left = root_->right = leaf_;
		}
		else {
			if (Exists(key)) {
				delete tmp;
				return "ERROR: Node with this key already exists";
			}
			else {
				Pointer current_node = root_;
				Pointer parent = root_;
				while (current_node != leaf_) {
					parent = current_node;
					current_node = (current_node->data.GetKey() > key) ? current_node->left : current_node->right;
				}
				current_node = tmp;
				current_node->parent = parent;
				current_node->left = current_node->right = leaf_;
				if (parent->data.GetKey() > key) {
					parent->left = current_node;
				}
				else {
					parent->right = current_node;
				}
				BalanceTree(current_node);
			}
		}
		if (validity) {
			//Delete node !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		}

		return "OK";
	}

	Student SelfBalancingBinarySearchTree::Get(std::string key) {
		Node* current_node = root_;

		while (current_node != leaf_) {
			if (current_node->data.GetKey() == key) return current_node->data.GetValue();
			if (current_node->data.GetKey() > key) current_node = current_node->left;
			else current_node = current_node->right;
		}
		return current_node->data.GetValue();
	}



	bool SelfBalancingBinarySearchTree::Exists(std::string key) {
		if (Search(key)) return true;
		return false;
	}

	bool SelfBalancingBinarySearchTree::Del(std::string key) {
		return true;
	}

	std::string SelfBalancingBinarySearchTree::Update(std::string param) {
		return std::string();
	}

	std::string SelfBalancingBinarySearchTree::Keys(std::string param) {
		return std::string();
	}

	std::string SelfBalancingBinarySearchTree::Rename(std::string param) {
		return std::string();
	}

	std::string SelfBalancingBinarySearchTree::Ttl(std::string param) {
		return std::string();
	}

	std::string SelfBalancingBinarySearchTree::Find(std::string param) {
		return std::string();
	}

	std::string SelfBalancingBinarySearchTree::Showall(std::string param) {
		return std::string();
	}

	std::string SelfBalancingBinarySearchTree::Upload(std::string param) {
		return std::string();
	}

	std::string SelfBalancingBinarySearchTree::Export(std::string param) {
		return std::string();
	}

	void SelfBalancingBinarySearchTree::Swap(Pointer a, Pointer b) {
		std::swap(a->data, b->data);
		std::swap(a->color, b->color);
	}

	void SelfBalancingBinarySearchTree::LeftRotation(Pointer node) {
		Swap(node, node->right);
		Pointer tmp = node->left;
		node->left = node->right;
		node->right = node->left->right;
		node->right->parent = node;

		node->left->right = node->left->left;
		node->left->left = tmp;
		if (tmp)	tmp->parent = node->left;
	}

	void SelfBalancingBinarySearchTree::RightRotation(Pointer node) {
		Swap(node, node->left);
		Pointer tmp = node->right;
		node->right = node->left;
		node->left = node->right->left;
		node->left->parent = node;

		node->right->left = node->right->right;
		node->right->right = tmp;
		if (tmp) tmp->parent = node->right;
	}
	void SelfBalancingBinarySearchTree::BalanceTree(Pointer node) {
		Pointer uncle;
		while (node->parent != nullptr && node->parent->color == Color::Red) {
			if (node->parent == node->parent->parent->left) {
				uncle = node->parent->parent->right;
				if (uncle && uncle->color == Color::Red) {
					uncle->color = Color::Black;
					node->parent->color = Color::Black;
					node->parent->parent->color = Color::Red;
					node = node->parent->parent;
				}
				else {
					if (node == node->parent->right) {
						node = node->parent;
						LeftRotation(node);
					}
					node->parent->color = Color::Black;
					node->parent->parent->color = Color::Red;
					RightRotation(node->parent->parent);
				}
			}
			else {
				uncle = node->parent->parent->left;
				if (uncle && uncle->color == Color::Red) {
					uncle->color = Color::Black;
					node->parent->color = Color::Black;
					node->parent->parent->color = Color::Red;
					node = node->parent->parent;
				}
				else {
					if (node == node->parent->left) {
						node = node->parent;
						RightRotation(node);
					}
					node->parent->color = Color::Black;
					node->parent->parent->color = Color::Red;
					LeftRotation(node->parent->parent);
				}
			}
		}
		root_->color = Color::Black;
	}
	void SelfBalancingBinarySearchTree::Clear() {

	}
	int SelfBalancingBinarySearchTree::CountChildren(Pointer node) {
		int count = 0;
		if (node) {
			count += node->left != leaf_;
			count += node->right != leaf_;
		}
		return count;
	}

	SelfBalancingBinarySearchTree::Pointer SelfBalancingBinarySearchTree::GetChild(Pointer node) {
		Pointer child = node->left != leaf_ ? node->left : node->right;
		return child;
	}

	SelfBalancingBinarySearchTree::Pointer SelfBalancingBinarySearchTree::GetParent(Pointer node) {
		return node->parent;
	}



	SelfBalancingBinarySearchTree::Pointer SelfBalancingBinarySearchTree::Search(std::string key) {
		Pointer node = root_;
		while (node && node != leaf_) {
			if (node->data.GetKey() == key) return node;
			node = (node->data.GetKey() > key) ? node->left : node->right;
		}
		return nullptr;
	}

	void SelfBalancingBinarySearchTree::RemoveNodeWithoutChildren(Pointer node) {
		Pointer parent = GetParent(node);
		if (!parent) {
			root_ = nullptr;
		}else{
			if (parent->left == node) parent->left = leaf_;
			else parent->right = leaf_;
		}
		delete node;
		node = nullptr;
	}

	bool SelfBalancingBinarySearchTree::RemoveNode(Pointer node) {
		if (!node || node == leaf_)	return false;
		bool color = node->color;
		switch (CountChildren(node))
		{
		case 0:
			RemoveNodeWithoutChildren(node);
			break;
		case 1:
			RemoveNodeWithOneChild(node);
			break;
		case 2:
			break;
		}
		if (!color) /*RebalanceTree()*/;
		return true;
	}

	/*SelfBalancingBinarySearchTree::Iterator SelfBalancingBinarySearchTree::Begin()
	{
		Pointer it = root_;
		while (it && it->left != leaf_) {
			it = it->left;
		}
		return Iterator(it, leaf_);
	}
	SelfBalancingBinarySearchTree::Iterator SelfBalancingBinarySearchTree::End()
	{
		Pointer it = root_;
		while (it && it->right != leaf_) {
			it = it->right;
		}
		return Iterator(it, leaf_);
	}*/
}