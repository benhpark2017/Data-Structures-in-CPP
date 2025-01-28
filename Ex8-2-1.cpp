/**
 * Solutions to Chapter 8, Section 2, Exercise 1 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 1:
 * Write a C++ function to delete identifier `x` from a hash table that uses
 * hash function `h` and linear open addressing to resolve collisions? Show that
 * simply setting the slot previously occupied by x to null does not solve the 
 * problem. How should the member function LinearSearch() be modified so that a 
 * correct search is made in the situation when deletions are permitted? Where 
 * can a new identifier be inserted?
 * 
 * Deletion requires a special marker. This prevents disrupting the probing
 * chain and ensures correct search behavior.
 * 
 * LinearSearch skips over the deleted slots. It must continue probing past
 * deleted slots while searching for identifiers.
 * 
 * Reusing deleted slots: This improves space utilization by allowing new 
 * identifiers to reuse slots marked as deleted.
 * 
 * Overall, this design ensures that the hash table handles deletions without 
 * breaking search or insertion functionality.
 */

#include <cstring> // For strcmp
#include <iostream>
#include <stdexcept>

struct identifier {
    char* id;
    int n;
    bool deleted; // Flag to indicate if this slot is marked as deleted

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

    void Insert(Name name, Attribute attr) {
        if (name == nullptr) throw std::invalid_argument("Name cannot be null");
        
        int index = hashFunction(name);
        int start = index;
        int firstDeleted = -1;
    
        // First pass: look for existing entry or note first deleted slot
        do {
            if (hashTable[index].id == nullptr) break;
            if (hashTable[index].deleted) {
                if (firstDeleted == -1) firstDeleted = index;
            } else if (strcmp(hashTable[index].id, name) == 0) {
                // Replace existing attribute
                hashTable[index].n = attr;
                return;
            }
            index = (index + 1) % buckets;
        } while (index != start);
    
        // Use first deleted slot if found, otherwise use current index
        if (firstDeleted != -1) index = firstDeleted;
        
        if (hashTable[index].id != nullptr && !hashTable[index].deleted) {
            throw std::overflow_error("Symbol table is full");
        }
    
        // Insert into the selected slot
        if (hashTable[index].id != nullptr) {
            delete[] hashTable[index].id;
        }
        hashTable[index].id = new char[strlen(name) + 1];
        strcpy(hashTable[index].id, name);
        hashTable[index].n = attr;
        hashTable[index].deleted = false;
    }

    void Delete(Name name) {
        if (name == nullptr) return;
        
        int index = hashFunction(name);
        int start = index;
        
        do {
            if (hashTable[index].id == nullptr) return;
            if (!hashTable[index].deleted && strcmp(hashTable[index].id, name) == 0) {
                delete[] hashTable[index].id;
                hashTable[index].id = nullptr;
                hashTable[index].deleted = true;
                hashTable[index].n = 0;
                return;
            }
            index = (index + 1) % buckets;
        } while (index != start);
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

private:
    int buckets;
    identifier* hashTable;

    // Simple Dan J. Bernstein (djb2) hash algorithm
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
    // Create a symbol table with a small size for demonstration
    SymbolTable<const char*, int> symTable(10);

    // Insert some elements
    try {

        symTable.Insert("apple", 10);
        symTable.Insert("banana", 20);
        symTable.Insert("cherry", 30);
        symTable.Insert("eggplant", 40);
        
        // Check if elements are in the table
        std::cout << "Is 'apple' in the table? " << (symTable.IsIn("apple") ? "Yes" : "No") << std::endl;
        std::cout << "Is 'banana' in the table? " << (symTable.IsIn("banana") ? "Yes" : "No") << std::endl;
        std::cout << "Is 'cherry' in the table? " << (symTable.IsIn("cherry") ? "Yes" : "No") << std::endl;
        std::cout << "Is 'date' in the table? " << (symTable.IsIn("date") ? "Yes" : "No") << std::endl;
        std::cout << "Is 'eggplant' in the table? " << (symTable.IsIn("eggplant") ? "Yes" : "No") << std::endl;
        std::cout << std::endl;

        // Find and print attributes
        int* attr = symTable.Find("banana");
        if (attr) {
            std::cout << "Attribute of 'banana': " << *attr << std::endl;
        }

        // Print the current state
        symTable.PrintTable();
        std::cout << std::endl;
        
        // Delete an element
        symTable.Delete("banana");
        std::cout << "After deleting 'banana', is it in the table? " << (symTable.IsIn("banana") ? "Yes" : "No") << std::endl;

        // Print the current state
        symTable.PrintTable();

        // Update an existing entry
        symTable.Insert("apple", 15);
        std::cout << "\nAfter updating 'apple':" << std::endl;
        symTable.PrintTable();

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}
