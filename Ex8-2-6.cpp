/**
 * Solutions to Chapter 8, Section 2, Exercise 6 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++, 1st Edition.
 *  
 * Task 6:
 * Design a symbol table representation that allows for search, insertion, and
 * deletion of an identifier in O(1) time. Assume that 0 <= x < m and that m + n
 * units of space are available, where n is the number of insertions to be made.
 * Do this by using two arrays a[n] and b[m], where a[i] will be the (i+1)th 
 * identifier inserted into the table. If x is the ith identifier inserted, then 
 * b[x] = i. Write functions to search, insert, and delete identifiers. Note 
 * that you cannot initialize either a or b to zero as this would take O(n + m) 
 * time. Note that x is an integer.
 */
 
#include <iostream>
#include <cstring>

struct identifier {
    const char* id;  // Use const char* for string literals
    int n;
    int attr;        // Add an attribute member
};

int operator==(identifier& a, identifier& b) {
    return strcmp(a.id, b.id) == 0;
}

int operator!=(identifier& a, identifier& b) {
    return strcmp(a.id, b.id) != 0;
}

template <typename Name, typename Attribute>
class SymbolTable {
public:
    SymbolTable(int size = 20) : buckets(size), n(0) {
        a = new Name[size];
        b = new int[size];
        for (int i = 0; i < size; ++i) {
            b[i] = -1;  // Initialize b to -1 to indicate empty slots
        }
    }

    ~SymbolTable() {
        delete[] a;
        delete[] b;
    }

    bool IsIn(Name name) {
        int x = hash(name);
        return b[x] != -1 && a[b[x]] == name;
    }

    Attribute* Find(Name name) {
        int x = hash(name);
        if (b[x] != -1 && a[b[x]] == name) {
            return &a[b[x]].attr;
        }
        return nullptr;
    }

    void Insert(Name name, Attribute attr) {
        int x = hash(name);
        if (b[x] == -1) {
            a[n] = name;
            a[n].attr = attr;  // Set the attribute
            b[x] = n;
            n++;
        } else {
            a[b[x]].attr = attr;  // Update the attribute
        }
    }

    void Delete(Name name) {
        int x = hash(name);
        if (b[x] != -1 && a[b[x]] == name) {
            b[x] = -1;  // Mark as deleted
        }
    }

    void PrintTable() {
        std::cout << "Symbol Table Contents:" << std::endl;
        for (int i = 0; i < buckets; ++i) {
            if (b[i] != -1) {
                std::cout << "Index " << i << ": ID = " << a[b[i]].id 
                          << ", Attribute = " << a[b[i]].attr << std::endl;
            } else {
                std::cout << "Index " << i << ": Empty" << std::endl;
            }
        }
    }

private:
    int buckets;
    int n;  // Number of identifiers inserted
    Name* a;  // Array to store identifiers
    int* b;   // Array to map identifier to index in a

    int hash(Name name) {
        // Simple hash function for demonstration purposes
        return name.n % buckets;
    }
};

int main() {
    SymbolTable<identifier, int> st;

    identifier id1 = {"var1", 1};
    identifier id2 = {"var2", 2};
    identifier id3 = {"var3", 3};

    st.Insert(id1, 10);
    st.Insert(id2, 20);
    st.Insert(id3, 30);

    std::cout << "Initial Symbol Table:" << std::endl;
    st.PrintTable();

    st.Delete(id2);

    std::cout << "\nSymbol Table after deleting id2:" << std::endl;
    st.PrintTable();

    return 0;
}
