#include "three.h"


int main() {
    Three<int, std::string> three;
    three.insert(4, "B");
    three.insert(7, "C");
    three.insert(1, "A");
    three.insert(2, "324");
    three.insert(22, "dasf");
    three.insert(11, "dasf");
    three.insert(-11, "dasf");
    std::cout << "======================================================" << std::endl;
    three.insert(4, "dasf alkjdsf ajsdf j;asd;fj a;sfdlj alksjdhgaspdhug wpe38290 hjep 9 ");
    three.insert(54, "GDG");

    Node<int, std::string>* root = three.root_;
    for (size_t i = 0; i < root->keys.size(); ++i) {
        std::cout << "key = " << root->keys[i] << std::endl;
        // std::cout << root->keys[i] << " = " << root->values[i] << std::endl;
    }
    std::cout << "Child 0:" << std::endl;
    Node<int, std::string>* left = root->childs[0];
    // // std::cout << left->keys.size();
    for (size_t i = 0; i < left->keys.size(); ++i) {
        std::cout << left->keys[i] << " = " << left->values[i] << std::endl;
    }
    std::cout << "Child 1:" << std::endl;
    Node<int, std::string>* right = root->childs[1];
    for (size_t i = 0; i < right->keys.size(); ++i) {
        std::cout << right->keys[i] << " = " << right->values[i] << std::endl;
    }

    std::cout << "Child 2:" << std::endl;
    Node<int, std::string>* third = root->childs[2];
    for (size_t i = 0; i < third->keys.size(); ++i) {
        std::cout << third->keys[i] << " = " << third->values[i] << std::endl;
    }

    // Node<std::string, int>* leaf = three.searchLeaf("abc");
    // for (size_t i = 0; i < leaf->keys.size(); ++i) {
    //     // std::cout << " = " << leaf->values[i] << std::endl;
    //     std::cout << leaf->keys[i] << " = " << leaf->values[i] << std::endl;
    // }

    return 0;
}
