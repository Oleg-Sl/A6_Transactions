#include "three.h"


int main() {
    Three<int, std::string> three;
    for (int i = 0; i <= 100; ++i) {
        three.insert(i, std::to_string(i));
    }
    for (int i = 0; i < 55; ++i) {
        three.remove(i);
    }
    three.printTree();

    // three.insert(3, "dasf");
    // three.insert(2, "324");
    // three.insert(1, "A");
    // three.insert(6, "dasf");
    // three.insert(5, "dasf");
    // three.insert(4, "dasf");
    // three.insert(7, "dasf");
    // three.insert(8, "dasf");
    // three.insert(10, "dasf");
    // three.insert(0, "dasf");
    // three.insert(9, "dasf");
    // three.insert(11, "dasf");
    // three.insert(12, "dasf");
    // three.insert(101, "dasf");
    // three.insert(102, "dasf");
    // three.printTree();

    // three.printTree();
    // three.remove(3);
    // three.remove(2);
    // std::cout << "======================================================" << std::endl;
    // three.remove(8);
    // std::cout << "======================================================" << std::endl;
    // three.remove(11);
    // three.remove(12);
    // three.remove(101);
    // three.remove(102);
    // three.remove(1);
    // three.remove(2);
    // three.remove(7);
    // three.remove(6);
    // three.remove(5);
    // three.remove(4);
    // three.remove(9);
    // three.remove(10);
    // three.remove(0);
    // three.remove(3);
    // three.remove(10);

    // // three.remove(5);
    // three.remove(6);
    // three.printTree();
    // three.remove(4);

    // three.printTree();


    
    // Node<int, std::string>* root = three.root_;
    // std::cout << ">>> Root, is leaf = " << root->is_leaf << std::endl;
    // three.printNode(root);
    // // std::cout << ">>> Child 0, is leaf = " << root->childs[0]->is_leaf << std::endl;
    // three.printNode(root->childs[0]);
    // // std::cout << ">>> Child 1, is leaf = " << root->childs[1]->is_leaf << std::endl;
    // three.printNode(root->childs[1]);
    // three.printNode(root->childs[1]);
    // // three.printNode(root->childs[0]->childs[0]);
    // // three.printNode(root->childs[0]->childs[1]);
    // // three.printNode(root->childs[0]->childs[2]);
    // // three.printNode(root->childs[1]->childs[0]);
    // // three.printNode(root->childs[1]->childs[1]);



    // std::cout << ">>> Child 2, is leaf = " << root->childs[2]->is_leaf << std::endl;
    // three.printNode(root->childs[2]);
    // std::cout << "Child 3" << std::endl;
    // three.printNode(root->childs[3]);

    // std::cout << "Child 00" << std::endl;
    // three.printNode(root->childs[0]->childs[0]);
    // std::cout << "Child 01" << std::endl;
    // three.printNode(root->childs[0]->childs[1]);
    // std::cout << "Child 02" << std::endl;
    // three.printNode(root->childs[0]->childs[2]);

    // std::cout << "Child 10" << std::endl;
    // three.printNode(root->childs[1]->childs[0]);
    // std::cout << "Child 11" << std::endl;
    // three.printNode(root->childs[1]->childs[1]);

    // Node<int, std::string>* root = three.root_;
    // std::cout << "Root: " << std::endl;
    // for (size_t i = 0; i < root->keys.size(); ++i) {
    //     std::cout << "key = " << root->keys[i] << std::endl;
    //     // std::cout << root->keys[i] << " = " << root->values[i] << std::endl;
    // }
    // std::cout << "Child 0:" << std::endl;
    // Node<int, std::string>* left = root->childs[0];
    // // std::cout << "Child 0 node parent: " << left->parent << std::endl;
    // for (size_t i = 0; i < left->keys.size(); ++i) {
    //     std::cout << left->keys[i] << " = " << left->values[i] << std::endl;
    // }
    // std::cout << "Child 1:" << std::endl;
    // Node<int, std::string>* right = root->childs[1];
    // // std::cout << "Child 0 node parent: " << right->parent << std::endl;
    // for (size_t i = 0; i < right->keys.size(); ++i) {
    //     std::cout << right->keys[i] << " = " << right->values[i] << std::endl;
    // }
    // std::cout << "Child 2:" << std::endl;
    // Node<int, std::string>* third = root->childs[2];
    // for (size_t i = 0; i < third->keys.size(); ++i) {
    //     std::cout << third->keys[i] << " = " << third->values[i] << std::endl;
    // }

    // std::cout << "Child 3:" << std::endl;
    // Node<int, std::string>* four = root->childs[3];
    // for (size_t i = 0; i < four->keys.size(); ++i) {
    //     std::cout << four->keys[i] << " = " << four->values[i] << std::endl;
    // }


    // Node<std::string, int>* leaf = three.searchLeaf("abc");
    // for (size_t i = 0; i < leaf->keys.size(); ++i) {
    //     // std::cout << " = " << leaf->values[i] << std::endl;
    //     std::cout << leaf->keys[i] << " = " << leaf->values[i] << std::endl;
    // }

    return 0;
}
