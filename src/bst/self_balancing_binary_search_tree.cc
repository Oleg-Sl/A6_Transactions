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
		
		Pointer tmp = node->right;
		node->right = tmp->left;
		if (tmp->left != leaf_) {
			tmp->left->parent = node;
		}
		tmp->parent = node->parent;
		if (node->parent == nullptr) {
			root_ = tmp;
		}
		else if (node == node->parent->left) {
			node->parent->left = tmp;
		}
		else {
			node->parent->right = tmp;
		}
		tmp->left = node;
		node->parent = tmp;
		
		
	}

	void SelfBalancingBinarySearchTree::RightRotation(Pointer node) {
		Pointer tmp = node->left;
		node->left = tmp->right;
		if (tmp->right != leaf_) {
			tmp->right->parent = node;
		}
		tmp->parent = node->parent;
		if (node->parent == nullptr) {
			root_ = tmp;
		}
		else if (node == node->parent->right) {
			node->parent->right = tmp;
		}
		else {
			node->parent->left = tmp;
		}
		tmp->right = node;
		node->parent = tmp;

		
	}
	void SelfBalancingBinarySearchTree::BalanceTree(Pointer node) {
		
		while (node->parent != nullptr && node->parent->color == Color::Red) {
			if (node->parent == node->parent->parent->left) {
				BalanceLeftSubTree(node);
			}
			else {
				BalanceRightSubTree(node);
			}
		}
		root_->color = Color::Black;
	}

	void SelfBalancingBinarySearchTree::BalanceLeftSubTree(Pointer node){
		Pointer uncle;
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

	void SelfBalancingBinarySearchTree::BalanceRightSubTree(Pointer node){
		Pointer uncle;
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
		}
		else {
			if (parent->left == node) parent->left = leaf_;
			else parent->right = leaf_;
		}
		delete node;
		node = nullptr;
	}



	void SelfBalancingBinarySearchTree::RebalanceTree(Pointer node) {
		while (node != root_ && node->color == Color::Black) {
			Pointer parent = GetParent(node);
			Pointer brother;
			if (NodeIsLeftChild(node)) {
				brother = parent->right;
				if (brother->color == Color::Red) {
					brother->color = Color::Black;
					parent->color = Color::Red;
					LeftRotation(parent);
					brother = parent->right;
				}
				if (brother->left->color == Color::Black && brother->right->color == Color::Black) {
					brother->color = Color::Red;
					node = parent;
				}
				else {
					if (brother->right->color == Color::Black) {
						brother->left->color = Color::Black;
						brother->color = Color::Red;
						RightRotation(brother);
						brother = parent->right;
					}
					brother->color = parent->color;
					parent->color = Color::Black;
					brother->right->color = Color::Black;
					LeftRotation(parent);
					node = root_;
				}
			}
			else {
				brother = parent->left;
				if (brother->color == Color::Red) {
					brother->color = Color::Black;
					parent->color = Color::Red;
					RightRotation(parent);
					brother = parent->left;
				}
				if (brother->right->color == Color::Black && brother->left->color == Color::Black) {
					brother->color = Color::Red;
					node = parent;
				}
				else {
					if (brother->left->color == Color::Black) {
						brother->right->color = Color::Black;
						brother->color = Color::Red;
						LeftRotation(brother);
						brother = parent->left;
					}
					brother->color = parent->color;
					parent->color = Color::Black;
					brother->left->color = Color::Black;
					RightRotation(parent);
					node = root_;
				}
			}
		}
		node->color = Color::Black;
	}

	bool SelfBalancingBinarySearchTree::NodeIsLeftChild(Pointer node) {
		return GetParent(node)->left == node;
	}

	SelfBalancingBinarySearchTree::Pointer SelfBalancingBinarySearchTree::MinValueFromRight(Pointer node) {
		Pointer tmp = node->right;
		while (tmp->left != leaf_) {
			tmp = tmp->left;
		}
		return tmp;
	}

	void SelfBalancingBinarySearchTree::ExcludeNode(Pointer a, Pointer b) {
		Pointer parent = GetParent(a);
		if (!parent) root_ = b;
		else if (NodeIsLeftChild(a)) {
			parent->left = b;
		}
		else {
			parent->right = b;
		}
		b->parent = parent;
	}

	void SelfBalancingBinarySearchTree::RemoveNodeWithOneChild(Pointer node) {
		Pointer child = GetChild(node);
		bool child_color = child->color;
		Swap(node, child);

		if (node->left == child) node->left = leaf_;
		else node->right = leaf_;
		delete child;
		child = nullptr;

		if (child_color) node->color = Color::Black;
		else RebalanceTree(node);
	}

	void SelfBalancingBinarySearchTree::RemoveNodeWithTwoChild(Pointer node) {
		Pointer min_node = MinValueFromRight(node);
		bool color = min_node->color;
		Pointer child = GetChild(min_node);
		Pointer parent = GetParent(min_node);

		if (parent == node) {
			child->parent = min_node;
		}
		else{
			ExcludeNode(min_node, child);
			min_node->right = node->right;
			min_node->right->parent = min_node;
		}

		ExcludeNode(node, min_node);
		min_node->left = node->left;
		min_node->left->parent = min_node;
		min_node->color = node->color;

		delete node;
		if (color == Color::Black)RebalanceTree(child);
		if (child == leaf_) child->parent = nullptr;
	}

	bool SelfBalancingBinarySearchTree::RemoveNode(Pointer node) {
		if (!node || node == leaf_)	return false;
		switch (CountChildren(node))
		{
		case 0:
			RemoveNodeWithoutChildren(node);
			break;
		case 1:
			RemoveNodeWithOneChild(node);
			break;
		case 2:
			RemoveNodeWithTwoChild(node);
			break;
		}
		return true;
	}	
}