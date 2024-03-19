#include "self_balancing_binary_search_tree.h"

namespace s21 {

	std::string SelfBalancingBinarySearchTree::Set(std::string key, Student student, int validity) {
		Data data(key, student, validity);
		Pointer tmp = new Node(data);
		if (!root_) {
			root_ = tmp;
			root_->color = Color::Black;
			root_->link[0] = root_->link[1] = leaf_;
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
					current_node = (current_node->data.GetKey() > key) ? current_node->link[0] : current_node->link[1];
				}
				current_node = tmp;
				current_node->parent = parent;
				current_node->link[0] = current_node->link[1] = leaf_;

				bool dir = parent->data.GetKey() > key;
				parent->link[!dir] = current_node;

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
			if (current_node->data.GetKey() > key) current_node = current_node->link[0];
			else current_node = current_node->link[1];
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

		Pointer tmp = node->link[1];
		node->link[1] = tmp->link[0];
		if (tmp->link[0] != leaf_) {
			tmp->link[0]->parent = node;
		}
		tmp->parent = node->parent;
		if (node->parent == nullptr) {
			root_ = tmp;
		}
		else if (node == node->parent->link[0]) {
			node->parent->link[0] = tmp;
		}
		else {
			node->parent->link[1] = tmp;
		}
		tmp->link[0] = node;
		node->parent = tmp;


	}

	void SelfBalancingBinarySearchTree::RightRotation(Pointer node) {
		Pointer tmp = node->link[0];
		node->link[0] = tmp->link[1];
		if (tmp->link[1] != leaf_) {
			tmp->link[1]->parent = node;
		}
		tmp->parent = node->parent;
		if (node->parent == nullptr) {
			root_ = tmp;
		}
		else if (node == node->parent->link[1]) {
			node->parent->link[1] = tmp;
		}
		else {
			node->parent->link[0] = tmp;
		}
		tmp->link[1] = node;
		node->parent = tmp;


	}
	void SelfBalancingBinarySearchTree::BalanceTree(Pointer node) {
		Pointer uncle;
		while (node->parent != nullptr && node->parent->color == Color::Red) {
			bool dir = NodeIsLeftChild(node->parent);
			uncle = node->parent->parent->link[dir];
			if (uncle && uncle->color == Color::Red) {
				uncle->color = Color::Black;
				node->parent->color = Color::Black;
				node->parent->parent->color = Color::Red;
				node = node->parent->parent;
			}
			else {
				if (node == node->parent->link[dir]) {
					node = node->parent;
					if (dir)	LeftRotation(node);
					else RightRotation(node);
				}
				node->parent->color = Color::Black;
				node->parent->parent->color = Color::Red;
				if (dir) RightRotation(node->parent->parent);
				else LeftRotation(node->parent->parent);
			}
		}
		root_->color = Color::Black;
	}


	void SelfBalancingBinarySearchTree::Clear() {

	}
	int SelfBalancingBinarySearchTree::CountChildren(Pointer node) {
		int count = 0;
		if (node) {
			count += node->link[0] != leaf_;
			count += node->link[1] != leaf_;
		}
		return count;
	}

	SelfBalancingBinarySearchTree::Pointer SelfBalancingBinarySearchTree::GetChild(Pointer node) {
		Pointer child = node->link[0] != leaf_ ? node->link[0] : node->link[1];
		return child;
	}

	SelfBalancingBinarySearchTree::Pointer SelfBalancingBinarySearchTree::GetParent(Pointer node) {
		return node->parent;
	}



	SelfBalancingBinarySearchTree::Pointer SelfBalancingBinarySearchTree::Search(std::string key) {
		Pointer node = root_;
		while (node && node != leaf_) {
			if (node->data.GetKey() == key) return node;
			node = (node->data.GetKey() > key) ? node->link[0] : node->link[1];
		}
		return nullptr;
	}

	void SelfBalancingBinarySearchTree::RebalanceTree(Pointer node) {
		while (node != root_ && node->color == Color::Black) {
			Pointer parent = GetParent(node);
			Pointer brother;
			bool dir = NodeIsLeftChild(node);

			brother = parent->link[dir];
			if (brother->color == Color::Red) {
				brother->color = Color::Black;
				parent->color = Color::Red;
				if (dir)	LeftRotation(parent);
				else RightRotation(parent);
				brother = parent->link[dir];
			}
			if (brother->link[!dir]->color == Color::Black && brother->link[dir]->color == Color::Black) {
				brother->color = Color::Red;
				node = parent;
			}
			else {
				if (brother->link[dir]->color == Color::Black) {
					brother->link[!dir]->color = Color::Black;
					brother->color = Color::Red;
					if (dir)	RightRotation(brother);
					else LeftRotation(brother);

					brother = parent->link[dir];
				}
				brother->color = parent->color;
				parent->color = Color::Black;
				brother->link[dir]->color = Color::Black;
				if (dir)	LeftRotation(parent);
				else RightRotation(parent);
				node = root_;
			}
		}
		node->color = Color::Black;
	}

	bool SelfBalancingBinarySearchTree::NodeIsLeftChild(Pointer node) {
		return GetParent(node)->link[0] == node;
	}

	SelfBalancingBinarySearchTree::Pointer SelfBalancingBinarySearchTree::MinValueFromRight(Pointer node) {
		Pointer tmp = node->link[1];
		while (tmp->link[0] != leaf_) {
			tmp = tmp->link[0];
		}
		return tmp;
	}

	void SelfBalancingBinarySearchTree::ExcludeNode(Pointer a, Pointer b) {
		Pointer parent = GetParent(a);
		if (!parent) root_ = b;
		else {
			bool dir = NodeIsLeftChild(a);
			parent->link[!dir] = b;
		}
		b->parent = parent;
	}

	void SelfBalancingBinarySearchTree::RemoveNodeWithoutChildren(Pointer node) {
		Pointer parent = GetParent(node);
		if (!parent) {
			root_ = nullptr;
		}
		else {
			if (parent->link[0] == node) parent->link[0] = leaf_;
			else parent->link[1] = leaf_;
		}
		delete node;
		node = nullptr;
	}

	void SelfBalancingBinarySearchTree::RemoveNodeWithOneChild(Pointer node) {
		Pointer child = GetChild(node);
		bool child_color = child->color;
		Swap(node, child);

		if (node->link[0] == child) node->link[0] = leaf_;
		else node->link[1] = leaf_;
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
		else {
			ExcludeNode(min_node, child);
			min_node->link[1] = node->link[1];
			min_node->link[1]->parent = min_node;
		}

		ExcludeNode(node, min_node);
		min_node->link[0] = node->link[0];
		min_node->link[0]->parent = min_node;
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