#include "HashTable.h"
#include "HashTable.cpp"


int main()
{
    KeyedHashTable ht;
    
    ht.Insert("A", std::vector<int>());
    ht.Insert("X", std::vector<int>());
    ht.Insert("U", std::vector<int>());
    ht.Insert("U", std::vector<int>());
    

    ht.Print();
}