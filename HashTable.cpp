#include "HashTable.h"

#include <cassert>
#include <iostream>
#include <iomanip>

const int KeyedHashTable::PRIME_LIST[PRIME_TABLE_COUNT] =
{
     2,    3,   5,   7,  11,  13,  17,  19,
     23,  29,  31,  37,  41,  43,  47,  53,
     59,  61,  67,  71,  73,  79,  83,  89,
     97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311,
    313, 317, 331, 337, 347, 349, 353, 359,
    367, 373, 379, 383, 389, 397, 401, 409,
    419, 421, 431, 433, 439, 443, 449, 457,
    461, 463, 467, 479, 487, 491, 499, 503,
    509, 521, 523, 541
};

int KeyedHashTable::Hash(const std::string& key) const {
    // TODO
    int hashValue = 0;
    for (int i = 0; i < key.length(); i++) {
        hashValue += ((int)key[i]) * PRIME_LIST[i];
    }
    hashValue = hashValue % tableSize;
    return hashValue;
}


void KeyedHashTable::ReHash() {
    // TODO
    int newHashValue;
    // save the old table size
    int oldTableSize = tableSize;
    // update the table size
    tableSize = FindNearestLargerPrime(2 * tableSize);
    // save a pointer to the old table
    HashData* oldTablePtr = table;    
    // create a new table, which is pointed by table, with the new table size
    table = new HashData[tableSize];

    // traverse the old table and hash the all data to the new table
    for (int i = 0; i < oldTableSize; i++) {
        // find the hash value of the HashData according to the new table
        newHashValue = Hash(oldTablePtr[i].key);
        // use collision resolution by quadratic probing
        // search a place to put your hash data
        int k = 0;
        while (true) {
            // if the position is empty
            if (table[ ( newHashValue + k*k ) % tableSize ].key == "") {
                // put the hash data to this position
                table[ (newHashValue + (k*k)) % tableSize ].intArray = oldTablePtr[i].intArray;
                table[ (newHashValue + (k*k)) % tableSize ].key = oldTablePtr[i].key;
                // stop searching
                break;
            }
            // if the position is not empty
            else {
                // increase k by 1
                k++;
                // continue searching an empty place
                continue;
            }
        }
    }
    // delete oldTable
    delete[] oldTablePtr;
}

int KeyedHashTable::FindNearestLargerPrime(int requestedCapacity) {
    // TODO
    // traverse the whole prime list
    for (int i = 0; i < PRIME_TABLE_COUNT; i++) {
        // if this element is the first element greater than the requested capacity
        if (requestedCapacity < PRIME_LIST[i]) {
            // return this element and terminate the process
            return PRIME_LIST[i];
        }
        // if this element is not greater than the requested capacity
        else {
            // continue searching
            continue;
        }
    }
}

KeyedHashTable::KeyedHashTable() {
    // TODO
    // save a memory of 2 hash data variable
    table = new HashData[2];
    // assign table size as the first prime number 2
    tableSize = 2;
    // initialize occupied element to 0
    occupiedElementCount = 0;
}

KeyedHashTable::KeyedHashTable(int requestedCapacity) {
    // TODO
    // find the size of the table
    int size = FindNearestLargerPrime(requestedCapacity);
    // save a memory of size of HashData variable
    table = new HashData[size];
    // assign table size as the size of the table
    tableSize = size;
    // initialize occupied element to 0
    occupiedElementCount = 0;
}

KeyedHashTable::KeyedHashTable(const KeyedHashTable& other) {
    // TODO
    // set table size and occupied element count
    tableSize = other.tableSize;
    occupiedElementCount = other.occupiedElementCount;
    // create a table in the memory of the size tableSize
    table = new HashData[tableSize];
    // traverse the table of other
    for (int i = 0; i < other.tableSize; i++) {
        // assign their values
        table[i].key = (other.table)[i].key;
        table[i].intArray = (other.table)[i].intArray;
    }
}

KeyedHashTable& KeyedHashTable::operator=(const KeyedHashTable& other) {
    // TODO
    // delete the table from heap
    delete[] table;
    // set table size and occupied element count 0
    tableSize = 0;
    occupiedElementCount = 0;
    
    // set table size and occupied element count
    tableSize = other.tableSize;
    occupiedElementCount = other.occupiedElementCount;
    // create a table in the memory of the size tableSize
    table = new HashData[tableSize];
    // traverse the table of other
    for (int i = 0; i < other.tableSize; i++) {
        // assign their values
        table[i].key = (other.table)[i].key;
        table[i].intArray = (other.table)[i].intArray;
    }
}

KeyedHashTable::~KeyedHashTable() {
    // TODO
    // delete the table from heap
    delete[] table;
    // set table size and occupied element count 0
    tableSize = 0;
    occupiedElementCount = 0;
}

bool KeyedHashTable::Insert(const std::string& key, const std::vector<int>& intArray) {
    // TODO
    std::vector<int> valueOut;
    // if there is an entry with the given name
    if (Find(valueOut, key)) {
        // terminate the process
        return false;
    }
    // determine its hashValue
    int hashValue = Hash(key);

    // use collision resolution by quadratic probing
    // search a place to put your hash data
    int k = 0;
    while (true) {
        // if the position is empty
        if (table[(hashValue + (k*k)) % tableSize].key == "") {
            // put the hash data to this position
            table[(hashValue + (k*k)) % tableSize].intArray = intArray;
            table[(hashValue + (k*k)) % tableSize].key = key;
            // increase occupied element count
            occupiedElementCount++;
            // stop searching
            break;
        }
        // if the position is not empty
        else {
            // increase k by 1
            k++;
            // continue searching an empty place
            continue;
        }
    }
    // if we reach the threshold
    if (occupiedElementCount * EXPAND_THRESHOLD >= tableSize) {
        // rehash the table
        ReHash();
    }
    // since insertion operation is successful, return true
    return true;
}

bool KeyedHashTable::Remove(const std::string& key) {
    // TODO
    // traverse the whole array
    for (int i = 0; i < tableSize; i++) {
        // if this is the hash data with the given key
        if (table[i].key == key) {
            // create a new empty vector
            std::vector<int> emptyVector;
            // assign it to the intArray of the hash data
            table[i].intArray = emptyVector;
            // set its key to empty key
            table[i].key = "_DELETED_";
            // decrease occupied element count
            occupiedElementCount--;
            // terminate the process
            return true;
        }
        // if this is not the hash data with the given key
        else {
            // continue searching
            continue;
        }
    }
    // the hash data with the given key could not be found
    return false;
}

void KeyedHashTable::ClearTable() {
    // TODO
    // traverse the whole table
    for (int i = 0; i < tableSize; i++) {
        // create an empty vector
        std::vector<int> emptyVector;
        // set the intArray of the hash data to empty vector
        table[i].intArray = emptyVector;
        // set its key to empty string
        table[i].key = "";
    }
    // set the table size and occupied element count to 0
    tableSize = 0;
    occupiedElementCount = 0;
}

bool KeyedHashTable::Find(std::vector<int>& valueOut, const std::string& key) const {
    // TODO
    // traverse the whole table
    for (int i = 0; i < tableSize; i++) {
        // if this hash data has the same key with the given key
        if (table[i].key == key) {
            // copy the intArray to valueOut
            valueOut = table[i].intArray;
            // terminate the process
            return true;
        }
        // if this hash data has not the same key with the given key
        else {
            // continue searching
            continue;
        }
    }
    // we could not find any hash data with the given key
    return false;
}

void KeyedHashTable::Print() const
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    std::cout << "HT:";
    if(occupiedElementCount == 0)
    {
        std::cout << " Empty";
    }
    std::cout << "\n";
    for(int i = 0; i < tableSize; i++)
    {
        if(table[i].key == "") continue;

        std::cout << "[" << std::setw(3) << i << "] ";
        std::cout << "[" << table[i].key << "] ";
        std::cout << "[";
        for(size_t j = 0; j < table[i].intArray.size(); j++)
        {
            std::cout << table[i].intArray[j];
            if((j + 1) != table[i].intArray.size())
                std::cout << ", ";
        }
        std::cout << "]\n";
    }
    std::cout.flush();
}