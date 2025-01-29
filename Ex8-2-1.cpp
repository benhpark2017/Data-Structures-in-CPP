/**
 * Solutions to Chapter 8, Section 2, Exercise 1 and 2(b) of Horowitz, Sahni, 
 * and Mehta's Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 1:
 * Write a C++ function to delete identifier `x` from a hash table that uses
 * hash function `h` and linear open addressing to resolve collisions. Show that
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
 * 
 * Task 2(b):
 * If quadratic searching is carried out in the sequence (h(x)+q^2), 
 * (h(x)+(q-1)^2), (h(x)+(q-2)^2), ... , (h(x)+1), h(x), (h(x)-1), ... , 
 * (h(x)-q^2) with q = (b-1)/2, then the address difference % b between 
 * successive buckets being examined is b-2, b-4, b-6, ... , 5, 3, 1, 1, 3, 5, 
 * ..., b-6, b-4, b-2. Write a C++ function to search a hash table hashTable of 
 * size b for the identifier x. Use h as the hash function and use the quadratic 
 * probe scheme to resolve overflows. If x is not in the table, it is to be 
 * entered. Use the address difference of successive buckets to reduce the 
 * computations.
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

    int LinearSearch(const identifier& x, int (*hashFunction)(const identifier&)) {
        int i = hashFunction(x); // Compute the starting index using the hash function
        int start = i; // Save the starting index to detect full loops
    
        // Loop through the hash table
        while (true) {
            // If the current bucket is empty (not just deleted), the search ends
            if (hashTable[i].id == nullptr && !hashTable[i].deleted) {
                return -1; // Not found
            }
    
            // If the current bucket is not deleted and matches the target identifier, return the index
            if (!hashTable[i].deleted && hashTable[i] == x) {
                return i; // Found the identifier
            }
    
            // Move to the next bucket (with wrap-around)
            i = (i + 1) % buckets;
    
            // If we've looped back to the start, the search ends
            if (i == start) {
                return -1; // Not found
            }
        }
    }

    // Solution to Chapter 8, Section 2, Exercise 2(b)
    int QuadraticSearch(const identifier& x, int (*hashFunction)(const identifier&)) {
        // Calculate initial hash value
        int h = hashFunction(x);
        int b = buckets;  // table size
        int q = (b - 1) / 2;  // maximum probe distance
        
        // Variables to track the first empty or deleted slot
        int firstEmpty = -1;
        int firstDeleted = -1;
        
        // Initialize address difference sequence
        int diff = b - 2;  // Start with b-2
        bool increasing = false;  // Flag to switch between decreasing and increasing sequence
        int current = h;  // Current bucket being examined
        int probeCount = 0;  // Count of probes made
        
        while (probeCount < b) {  // Prevent infinite loop
            // Check current bucket
            if (hashTable[current].id == nullptr) {
                if (firstEmpty == -1) firstEmpty = current;
                break;
            } else if (hashTable[current].deleted) {
                if (firstDeleted == -1) firstDeleted = current;
            } else if (!hashTable[current].deleted && hashTable[current] == x) {
                return current;  // Found exact match
            }
            
            // Update probe sequence
            if (!increasing) {
                // Decreasing sequence: b-2, b-4, b-6, ..., 3, 1
                if (diff <= 1) {
                    increasing = true;
                    diff = 1;
                } else {
                    diff -= 2;
                }
            } else {
                // Increasing sequence: 1, 3, 5, ..., b-6, b-4, b-2
                diff += 2;
                if (diff >= b - 2) {
                    break;  // We've completed the sequence
                }
            }
            
            // Calculate next bucket
            current = (current + diff) % b;
            if (current < 0) current += b;  // Ensure positive index
            
            probeCount++;
        }
        
        // If item not found, determine insertion point
        int insertPos = -1;
        if (firstDeleted != -1) {
            insertPos = firstDeleted;  // Prefer reusing deleted slots
        } else if (firstEmpty != -1) {
            insertPos = firstEmpty;  // Use first empty slot if no deleted slots
        }
        
        // If we found a valid insertion point, insert the new item
        if (insertPos != -1) {
            // Clean up existing entry if any
            if (hashTable[insertPos].id != nullptr) {
                delete[] hashTable[insertPos].id;
            }
            
            // Allocate and copy new identifier
            hashTable[insertPos].id = new char[strlen(x.id) + 1];
            strcpy(hashTable[insertPos].id, x.id);
            hashTable[insertPos].n = x.n;
            hashTable[insertPos].deleted = false;
            
            return insertPos;
        }
        
        // Table is full
        throw std::overflow_error("Hash table is full");
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

// Custom hash function for identifier
int identifierHashFunction(const identifier& id) {
    unsigned long hash = 5381;
    const char* str = id.id;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % 10; // Assuming a small table size for demonstration
}

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

        // Test LinearSearch() function
        identifier appleId;
        appleId.id = new char[6];
        strcpy(appleId.id, "apple");
        appleId.n = 15;

        int indexOne = symTable.LinearSearch(appleId, identifierHashFunction);
        if (indexOne != -1) {
            std::cout << "\nLinearSearch found 'apple' at index: " << indexOne << std::endl;
        } else {
            std::cout << "\nLinearSearch did not find 'apple'" << std::endl;
        }

        delete[] appleId.id;

        // Test QuadraticSearch() function
        identifier cherryId;
        cherryId.id = new char[7];
        strcpy(cherryId.id, "cherry");
        cherryId.n = 30;

        int indexTwo = symTable.QuadraticSearch(cherryId, identifierHashFunction);
        if (indexTwo != -1) {
            std::cout << "\nQuadraticSearch found 'cherry' at index: " << indexTwo << std::endl;
        } else {
            std::cout << "\nQuadraticSearch did not find 'cherry'" << std::endl;
        }
        std::cout << std::endl;

        delete[] cherryId.id;
        
        identifier figId;
        figId.id = new char[4];
        strcpy(figId.id, "fig");
        figId.n = 50;
        
        std::cout << "Adding 'fig' to the table." << std::endl;
        int indexThree = symTable.QuadraticSearch(figId, identifierHashFunction);
        if (indexThree != -1) {
            std::cout << "\nQuadraticSearch found 'fig' at index: " << indexThree << std::endl;
        } else {
            std::cout << "\nQuadraticSearch did not find 'fig', adding " 
                      << figId.n << " at index " << indexThree << std::endl;
        }
        std::cout << std::endl;
        
        symTable.PrintTable();
        std::cout << std::endl;
        
        // Delete an element
        symTable.Delete("fig");
        symTable.PrintTable();
        std::cout << std::endl;
        
        std::cout << "After deleting 'fig', is it in the table? " << (symTable.IsIn("fig") ? "Yes" : "No") << std::endl;
        
        delete[] figId.id;

    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}
