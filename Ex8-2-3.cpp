/**
 * Solutions to Chapter 8, Section 2, Exercise 3 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 3:
 * In random probing, the search for an identifier x in a hash table with b 
 * buckets is carried out by examining the buckets in the order h(x), 
 * (h(x)+S(i)) % b, where h is the hash function, 1 <= i <= b-1, and S(i) is a 
 * pseudo-random number. The random number generator must satisfy the property 
 * that every number from 1 to b-1 must be generated exactly once as i ranges 
 * from 1 to b-1. Convert the following pseudocode into a C++ function, which 
 * generates numbers with the following properties for a table of size 2^r:
 * 
 * 1. Initialize R to 1 each time the search routine is called. 
 * 2. On successive calls for a random number do the following:
 *    a. R *= 5 
 *    b. R = low order r + 2 bits of R
 *    c. S(i) = R/4
 * 
 * Write search and insert algorithms for a hash table using random probing and
 * the mid-square hash function. Use the random number generator described
 * above.
 */

#include <cstring> // For strcmp, strcpy, etc.
#include <iostream>
#include <string> // For std::string

struct identifier {
    char* id;
    int n;
    bool deleted;
    
    identifier() : id(nullptr), n(0), deleted(false) {} // Default constructor
};

// Equality operator for identifier - fixed to handle const
bool operator==(const identifier& a, const identifier& b) {
    if (a.id == nullptr || b.id == nullptr) return false;
    return strcmp(a.id, b.id) == 0 && a.n == b.n;
}

// Inequality operator for identifier
bool operator!=(const identifier& a, const identifier& b) {
    return !(a == b);
}

template <typename Name, typename Attribute>
class SymbolTable {
public:
    static const int defaultSize = 100; // Added defaultSize constant

    // Create an empty symbol table with capacity size
    SymbolTable(int size = defaultSize) {
        buckets = size;
        hashTable = new identifier[buckets];
        for (int i = 0; i < buckets; ++i) {
            hashTable[i].id = nullptr;
            hashTable[i].n = 0;
        }
    }

    // Destructor to clean up dynamically allocated memory
    ~SymbolTable() {
        for (int i = 0; i < buckets; ++i) {
            if (hashTable[i].id != nullptr) {
                delete[] hashTable[i].id;
            }
        }
        delete[] hashTable;
    }

    // If name is in symbol table, return 1; otherwise, return 0
    bool IsIn(Name name) const {
        if (name == nullptr) return false;
        int index = hashFunction(name);
        int start = index;
        
        do {
            if (hashTable[index].id == nullptr) return false;
            if (!hashTable[index].deleted && strcmp(hashTable[index].id, name) == 0) {
                return true;
            }
            index = (index + 1) % buckets;
        } while (index != start);
        
        return false;
    }

    // If name is in symbol table, return a pointer to the corresponding
    // attribute; otherwise, return nullptr
    Attribute* Find(Name name) {
        if (name == nullptr) return nullptr;
        int index = hashFunction(name);
        int start = index;
        
        do {
            if (hashTable[index].id == nullptr) return nullptr;
            if (!hashTable[index].deleted && strcmp(hashTable[index].id, name) == 0) {
                return &hashTable[index].n;
            }
            index = (index + 1) % buckets;
        } while (index != start);
        
        return nullptr;
    }

    // Insert a new identifier into the hash table using random probing
    void Insert(Name name, Attribute attr) {
        if (name == nullptr) return;

        // Calculate r such that buckets = 2^r
        int r = 0;
        int temp = buckets;
        while (temp >>= 1) ++r;

        // Compute the initial hash index using the mid-square hash function
        int index = midSquareHash(name, r);

        // Initialize R for the random number generator
        int R = 1;

        // Probe the hash table using random probing
        for (int i = 0; i < buckets; ++i) {
            // If the current bucket is empty or deleted, insert the new identifier
            if (hashTable[index].id == nullptr || hashTable[index].deleted) {
                hashTable[index].id = new char[strlen(name) + 1];
                strcpy(hashTable[index].id, name);
                hashTable[index].n = attr;
                hashTable[index].deleted = false;
                return;
            }

            // If the identifier already exists, update its attribute
            if (strcmp(hashTable[index].id, name) == 0) {
                hashTable[index].n = attr;
                return;
            }

            // Generate the next pseudo-random number
            R *= 5;
            R &= ((1 << (r + 2)) - 1); // Keep the low-order r+2 bits
            int S_i = R / 4;

            // Calculate the next index to probe
            index = (midSquareHash(name, r) + S_i) % buckets;
        }

        // If the table is full, throw an error or resize the table
        std::cerr << "Hash table is full. Cannot insert " << name << std::endl;
    }

    // If name is in symbol table, delete (name, attr) from symbol table
    void Delete(Name name) {
        int index = hashFunction(name);
        for (int i = 0; i < buckets; ++i) {
            int j = (index + i) % buckets;
            if (hashTable[j].id == nullptr && !hashTable[j].deleted) {
                return; // Not found
            }
            if (hashTable[j].id != nullptr && strcmp(hashTable[j].id, name) == 0) {
                // Mark as deleted
                delete[] hashTable[j].id;
                hashTable[j].id = nullptr;
                hashTable[j].n = 0;
                hashTable[j].deleted = true;
                return;
            }
        }
    }

    // In the SymbolTable class:
    int Search(const identifier& x, int (SymbolTable::*hashFunc)(Name, int) const) {
        if (x.id == nullptr) return -1;
    
        // Calculate r such that buckets = 2^r
        int r = 0;
        int temp = buckets;
        while (temp >>= 1) ++r;
    
        int R = 1; // Initialize R to 1
        // Use the passed hash function
        int index = (this->*hashFunc)(x.id, r);
    
        for (int i = 1; i <= buckets - 1; ++i) {
            if (hashTable[index].id != nullptr && !hashTable[index].deleted && hashTable[index] == x) {
                return index; // Found the identifier
            }
    
            // Generate the next pseudo-random number
            R *= 5;
            R &= ((1 << (r + 2)) - 1); // Keep the low-order r+2 bits
            int S_i = R / 4;
    
            // Calculate the next index using the passed hash function
            index = ((this->*hashFunc)(x.id, r) + S_i) % buckets;
        }
    
        return -1; // Identifier not found
    }

    // Added method to print table contents
    void PrintTable() const {
        std::cout << "Symbol Table Contents:" << std::endl;
        for (int i = 0; i < buckets; ++i) {
            if (hashTable[i].id != nullptr && !hashTable[i].deleted) {
                std::cout << "Bucket " << i << ": " << hashTable[i].id << " -> " << hashTable[i].n << std::endl;
            } else {
                std::cout << "Bucket " << i << ": Empty" << std::endl;
            }
        }
    }
    
    // Mid-square hash function
    int midSquareHash(Name name, int r) const {
        unsigned long hash = 5381;
        const char* str = name;
        int c;
        while ((c = *str++)) {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }

        // Square the hash value
        unsigned long squared = hash * hash;

        // Extract the middle r bits
        int shift = (sizeof(unsigned long) * 8 - r) / 2;
        unsigned long mask = (1 << r) - 1;
        return (squared >> shift) & mask;
    }
    
private:
    int buckets;
    identifier* hashTable;

    // Simple Daniel J. Bernstein hash algorithm (djb2)
    int hashFunction(Name name) const {
        unsigned long hash = 5381;
        const char* str = name;
        int c;
        while ((c = *str++)) {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return hash % buckets;
    }
};

int main() {
    SymbolTable<const char*, int> symTable(8); // Table size = 8 (2^3)
    
    // Insert some values
    symTable.Insert("apple", 10);
    symTable.Insert("banana", 20);
    symTable.Insert("cherry", 30);
    
    // Print initial table state
    std::cout << "Initial table state:" << std::endl;
    symTable.PrintTable();
    
    // Create an identifier to search for
    identifier searchId;
    searchId.id = new char[strlen("banana") + 1];
    strcpy(searchId.id, "banana");
    searchId.n = 20;
    searchId.deleted = false;
    
    // Search for "banana" using midSquareHash
    int result = symTable.Search(searchId, &SymbolTable<const char*, int>::midSquareHash);
    
    if (result != -1) {
        std::cout << "\nFound 'banana' at index: " << result << std::endl;
    } else {
        std::cout << "\n'banana' not found in table" << std::endl;
    }
    
    // Clean up
    delete[] searchId.id;
    
    return 0;
}
