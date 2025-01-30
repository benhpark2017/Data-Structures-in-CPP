/**
 * Solutions to Chapter 8, Section 2, Exercise 4 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++, 1st Edition.
 *  
 * Task 4:
 * Write an algorithm to list all the identifiers in a hash table in
 * lexicographic order. Assume the hash function h is h(x) = first character of
 * x, and linear probing is used. How much time does your algorithm take?
 * 
 * Response to the conceptual question.
 * The overall time complexity of ListIdentifiers() function is the sum of time
 * complexities of three steps: O(b) for iterating over all buckets, O(n*log(n))
 * for the merge sort algorithm, and O(n) for iterating over all identifiers to
 * print them. Because O(n*log(n)) dominates O(n) and O(b), the overall time
 * complexity simplifies to O(b + n*log(n)). 
 */

#include <cstring> // For strcmp
#include <iostream>
#include <vector>

struct identifier {
    char* id;
    int n;
    bool deleted;
    
    identifier() : id(nullptr), n(0), deleted(false) {} // Default constructor
};

// Equality operator for identifier
int operator==(const identifier& a, const identifier& b) {
    return strcmp(a.id, b.id) == 0 && a.n == b.n;
}

// Inequality operator for identifier
int operator!=(const identifier& a, const identifier& b) {
    return !(a == b);
}

template <typename Name, typename Attribute>
class SymbolTable {
  public:
    // Create an empty symbol table with capacity size
    SymbolTable(int size = 26) { // Default size is 26 for each letter of the alphabet
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
    bool IsIn(Name name) {
        int index = hashFunction(name);
        return hashTable[index].id != nullptr && strcmp(hashTable[index].id, name) == 0;
    }

    // If name is in symbol table, return a pointer to the corresponding
    // attribute; otherwise, return nullptr
    Attribute* Find(Name name) {
        int index = hashFunction(name);
        if (hashTable[index].id != nullptr && strcmp(hashTable[index].id, name) == 0) {
            return &hashTable[index].n; // Assuming n is the attribute
        }
        return nullptr;
    }

    // If name is in symbol table, then replace its existing attribute with attr;
    // else, insert the pair (name, attr) into the symbol table.
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

    // Function to list all identifiers in lexicographic order using merge sort
    void ListIdentifiers() {
        std::vector<const char*> identifiers;

        // Extract all non-null identifiers from the hash table
        for (int i = 0; i < buckets; ++i) {
            if (hashTable[i].id != nullptr) {
                identifiers.push_back(hashTable[i].id);
            }
        }

        // Sort the identifiers using merge sort
        MergeSort(identifiers, 0, identifiers.size() - 1);

        // Print the sorted identifiers
        for (const char* id : identifiers) {
            std::cout << id << std::endl;
        }
    }

  private:
    int buckets;
    identifier* hashTable;

    // Simple hash function for demonstration purposes
    int hashFunction(Name name) {
        return name[0] % buckets;
    }

    // Merge function for merge sort
    void Merge(std::vector<const char*>& arr, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        // Create temporary arrays
        std::vector<const char*> L(n1), R(n2);

        // Copy data to temporary arrays
        for (int i = 0; i < n1; ++i) {
            L[i] = arr[left + i];
        }
        for (int i = 0; i < n2; ++i) {
            R[i] = arr[mid + 1 + i];
        }

        // Merge the temporary arrays back into arr[left..right]
        int i = 0, j = 0, k = left;
        while (i < n1 && j < n2) {
            if (strcmp(L[i], R[j]) <= 0) {
                arr[k] = L[i];
                ++i;
            } else {
                arr[k] = R[j];
                ++j;
            }
            ++k;
        }

        // Copy the remaining elements of L[], if any
        while (i < n1) {
            arr[k] = L[i];
            ++i;
            ++k;
        }

        // Copy the remaining elements of R[], if any
        while (j < n2) {
            arr[k] = R[j];
            ++j;
            ++k;
        }
    }

    // Merge sort function
    void MergeSort(std::vector<const char*>& arr, int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;

            // Sort first and second halves
            MergeSort(arr, left, mid);
            MergeSort(arr, mid + 1, right);

            // Merge the sorted halves
            Merge(arr, left, mid, right);
        }
    }
};

int main() {
    // Assuming 26 buckets for each letter of the alphabet
    SymbolTable<const char*, int> symbolTable(26); 

    // Insert some identifiers with mixed attribute values
    symbolTable.Insert("indian fig", 1);
    symbolTable.Insert("date", 2);
    symbolTable.Insert("elderberry", 3);
    symbolTable.Insert("cherry", 4);
    symbolTable.Insert("apple", 5);
    symbolTable.Insert("grape", 6);
    symbolTable.Insert("fig", 7);
    symbolTable.Insert("honeydew", 8);
    symbolTable.Insert("banana", 9);
    
    // List all identifiers in lexicographic order
    std::cout << "Identifiers in lexicographic order:" << std::endl;
    symbolTable.ListIdentifiers();

    return 0;
}
