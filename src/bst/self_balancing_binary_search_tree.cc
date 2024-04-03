#include "self_balancing_binary_search_tree.h"

namespace s21 {

SelfBalancingBinarySearchTree::SelfBalancingBinarySearchTree() {
  root_ = nullptr;
  leaf_ = new Node();
}

SelfBalancingBinarySearchTree::~SelfBalancingBinarySearchTree() {
  Clear();
  delete leaf_;
}

bool SelfBalancingBinarySearchTree::Set(Data data) {
  Pointer tmp = new Node(data);
  if (!root_) {
    root_ = tmp;
    root_->color = Color::Black;
    root_->link[0] = root_->link[1] = leaf_;
  } else {
    if (Exists(data.GetKey())) {
      delete tmp;
      return false;
    } else {
      Pointer current_node = root_;
      Pointer parent = root_;
      while (current_node != leaf_) {
        parent = current_node;
        current_node = (current_node->data.GetKey() > data.GetKey())
                           ? current_node->link[0]
                           : current_node->link[1];
      }
      current_node = tmp;
      current_node->parent = parent;
      current_node->link[0] = current_node->link[1] = leaf_;

      bool dir = parent->data.GetKey() > data.GetKey();
      parent->link[!dir] = current_node;

      BalanceTree(current_node);
    }
  }
  if (data.GetValidity()) {
    // Delete node !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  }
  return true;
}

Student SelfBalancingBinarySearchTree::Get(std::string key) {
  Node* current_node = root_;

  while (current_node != leaf_) {
    if (current_node->data.GetKey() == key)
      return current_node->data.GetValue();
    if (current_node->data.GetKey() > key)
      current_node = current_node->link[0];
    else
      current_node = current_node->link[1];
  }
  return current_node->data.GetValue();
}

bool SelfBalancingBinarySearchTree::Exists(std::string key) {
  if (Search(key)) return true;
  return false;
}

bool SelfBalancingBinarySearchTree::Del(std::string key) {
  Pointer node = Search(key);
  if (!node) return false;
  RemoveNode(node);
  return true;
}

bool SelfBalancingBinarySearchTree::Update(std::string key, Student student) {
  Pointer node = Search(key);
  if (!node) return false;
  Student value = node->data.GetValue();
  if (student.name != "-") value.name = student.name;
  if (student.surname != "-") value.surname = student.surname;
  if (student.birthday != INT32_MAX) value.birthday = student.birthday;
  if (student.city != "-") value.city = student.city;
  if (student.coins != INT32_MAX) value.coins = student.coins;
  node->data.SetValue(value);
  return true;
}

std::vector<std::string> SelfBalancingBinarySearchTree::Keys() {
  std::vector<std::string> keys;
  Iterator i = Begin();
  if (i.IsValid()) {
    while (i != End()) {
      keys.push_back(i->GetKey());
      ++i;
    }
    keys.push_back(i->GetKey());
  }
  return keys;
}

bool SelfBalancingBinarySearchTree::Rename(std::string key,
                                           std::string new_key) {
  Pointer node = Search(key);
  if (!node) {
    Set(Data(new_key, Student()));
  } else {
    Set(Data(new_key, node->data.GetValue(), node->data.GetValidity()));
    RemoveNode(node);
  }
  return true;
}

std::string SelfBalancingBinarySearchTree::Ttl(std::string param) {
  return std::string();
}

std::vector<std::string> SelfBalancingBinarySearchTree::Find(Student student) {
  std::vector<std::string> keys;
  Iterator i = Begin();
  if (i.IsValid()) {
    while (i != End()) {
      bool cmp = CompareValue(i->GetValue(), student);
      if (cmp) keys.push_back(i->GetKey());
      ++i;
    }
    bool cmp = CompareValue(i->GetValue(), student);
    if (cmp) keys.push_back(i->GetKey());
  }
  return keys;
}

std::vector<Student> SelfBalancingBinarySearchTree::Showall() {
  std::vector<Student> nodes;
  Iterator i = Begin();
  if (i.IsValid()) {
    while (i != End()) {
      nodes.push_back(i->GetValue());
      ++i;
    }
    nodes.push_back(i->GetValue());
  }
  return nodes;
}

std::pair<bool, int> SelfBalancingBinarySearchTree::Upload(std::string path) {
  std::ifstream in;
  int count = 0;
  in.open(path);
  if (in.is_open()) {
    std::string line;
    while (std::getline(in, line)) {
      if (!line.empty()) {
        if (ParseLine(line)) {
          ++count;
        }
      }
    }
  } else {
    return std::make_pair(false, 0);
  }
  in.close();
  return std::make_pair(true, count);
}

std::pair<bool, int> SelfBalancingBinarySearchTree::Export(std::string path) {
  int count = 0;
  std::ofstream out;
  out.open(path);
  if (out.is_open()) {
    Iterator it = Begin();
    if (it.IsValid()) {
      while (it != End()) {
        out << it->GetKey() << " " << it->GetValue().ToString() << std::endl;
        ++count;
        ++it;
      }
      out << it->GetKey() << " " << it->GetValue().ToString() << std::endl;
      ++count;
    }
  } else {
    return std::make_pair(false, 0);
  }
  out.close();
  return std::make_pair(true, count);
}

bool SelfBalancingBinarySearchTree::CompareValue(Student student,
                                                 Student search_template) {
  if (search_template.name != "-" && search_template.name != student.name)
    return false;
  if (search_template.surname != "-" &&
      search_template.surname != student.surname)
    return false;
  if (search_template.birthday != INT32_MAX &&
      search_template.birthday != student.birthday)
    return false;
  if (search_template.city != "-" && search_template.city != student.city)
    return false;
  if (search_template.coins != INT32_MAX &&
      search_template.coins != student.coins)
    return false;
  return true;
}

bool SelfBalancingBinarySearchTree::ParseLine(std::string line) {
  std::stringstream ss(line);
  std::vector<std::string> param;
  std::string token;
  while (ss >> token) {
    param.push_back(token);
  }
  if (param.size() != 6) return false;
  try {
    std::string key = param[0];
    std::string name = param[1];
    std::string surname = param[2];
    int birthday = (param[3] == "-") ? 0 : stoi(param[3]);
    std::string city = param[4];
    int coins = (param[5] == "-") ? 0 : stoi(param[5]);
    Set(Data(key, Student(name, surname, birthday, city, coins)));
  } catch (std::exception ex) {
    return false;
  }
  return true;
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
  } else {
    bool dir = NodeIsLeftChild(node);
    node->parent->link[!dir] = tmp;
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
  } else {
    bool dir = NodeIsLeftChild(node);
    node->parent->link[!dir] = tmp;
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
    } else {
      if (node == node->parent->link[dir]) {
        node = node->parent;
        if (dir)
          LeftRotation(node);
        else
          RightRotation(node);
      }
      node->parent->color = Color::Black;
      node->parent->parent->color = Color::Red;
      if (dir)
        RightRotation(node->parent->parent);
      else
        LeftRotation(node->parent->parent);
    }
  }
  root_->color = Color::Black;
}

void SelfBalancingBinarySearchTree::Clear() {
  while (root_) {
    Pointer node = Begin().current_;
    RemoveNode(node);
  }
}

int SelfBalancingBinarySearchTree::CountChildren(Pointer node) const {
  int count = 0;
  if (node) {
    count += node->link[0] != leaf_;
    count += node->link[1] != leaf_;
  }
  return count;
}

SelfBalancingBinarySearchTree::Pointer SelfBalancingBinarySearchTree::GetChild(
    Pointer node) const {
  Pointer child = node->link[0] != leaf_ ? node->link[0] : node->link[1];
  return child;
}

SelfBalancingBinarySearchTree::Pointer SelfBalancingBinarySearchTree::GetParent(
    Pointer node) const {
  return node->parent;
}

SelfBalancingBinarySearchTree::Pointer SelfBalancingBinarySearchTree::Search(
    std::string key) const {
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
      if (dir)
        LeftRotation(parent);
      else
        RightRotation(parent);
      brother = parent->link[dir];
    }
    if (brother->link[!dir]->color == Color::Black &&
        brother->link[dir]->color == Color::Black) {
      brother->color = Color::Red;
      node = parent;
    } else {
      if (brother->link[dir]->color == Color::Black) {
        brother->link[!dir]->color = Color::Black;
        brother->color = Color::Red;
        if (dir)
          RightRotation(brother);
        else
          LeftRotation(brother);

        brother = parent->link[dir];
      }
      brother->color = parent->color;
      parent->color = Color::Black;
      brother->link[dir]->color = Color::Black;
      if (dir)
        LeftRotation(parent);
      else
        RightRotation(parent);
      node = root_;
    }
  }
  node->color = Color::Black;
}

bool SelfBalancingBinarySearchTree::NodeIsLeftChild(Pointer node) {
  return GetParent(node)->link[0] == node;
}

SelfBalancingBinarySearchTree::Pointer
SelfBalancingBinarySearchTree::MinValueFromRight(const Pointer node) const {
  Pointer tmp = node->link[1];
  while (tmp->link[0] != leaf_) {
    tmp = tmp->link[0];
  }
  return tmp;
}

void SelfBalancingBinarySearchTree::ExcludeNode(Pointer a, Pointer b) {
  Pointer parent = GetParent(a);
  if (!parent)
    root_ = b;
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
  } else {
    bool dir = (parent->link[1] == node);
    parent->link[dir] = leaf_;
  }
  delete node;
  node = nullptr;
}

void SelfBalancingBinarySearchTree::RemoveNodeWithOneChild(Pointer node) {
  Pointer child = GetChild(node);
  bool child_color = child->color;
  Swap(node, child);

  bool dir = (node->link[1] == child);
  node->link[dir] = leaf_;

  delete child;
  child = nullptr;

  if (child_color)
    node->color = Color::Black;
  else
    RebalanceTree(node);
}

void SelfBalancingBinarySearchTree::RemoveNodeWithTwoChild(Pointer node) {
  Pointer min_node = MinValueFromRight(node);
  bool color = min_node->color;
  Pointer child = GetChild(min_node);
  Pointer parent = GetParent(min_node);

  if (parent == node) {
    child->parent = min_node;
  } else {
    ExcludeNode(min_node, child);
    min_node->link[1] = node->link[1];
    min_node->link[1]->parent = min_node;
  }

  ExcludeNode(node, min_node);
  min_node->link[0] = node->link[0];
  min_node->link[0]->parent = min_node;
  min_node->color = node->color;

  delete node;
  if (color == Color::Black) RebalanceTree(child);
  if (child == leaf_) child->parent = nullptr;
}

bool SelfBalancingBinarySearchTree::RemoveNode(Pointer node) {
  if (!node || node == leaf_) return false;
  switch (CountChildren(node)) {
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

SelfBalancingBinarySearchTree::Iterator SelfBalancingBinarySearchTree::Begin() {
  Pointer it = root_;
  while (it && it->link[0] != leaf_) {
    it = it->link[0];
  }
  return Iterator(it, leaf_);
}

SelfBalancingBinarySearchTree::Iterator SelfBalancingBinarySearchTree::End() {
  Pointer it = root_;
  while (it && it->link[1] != leaf_) {
    it = it->link[1];
  }
  return Iterator(it, leaf_);
}
}  // namespace s21
