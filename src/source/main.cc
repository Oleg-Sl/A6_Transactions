#include <iostream>
#include <functional>

#include "model/hashtable/hash_table.h"

int main(){
    s21::HashTable<s21::Student> table;

    table.Set("KEY", s21::Student("NAME", "SURNAME", 12, "CITY", 5555));
    std::cout << table.Get("KEY").ToString();
    return 0;
}