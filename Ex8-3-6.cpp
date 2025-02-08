/**
 * Solutions to Chapter 8, Section 3, Exercises 6 and 7 of Horowitz, Sahni, and 
 * Mehta's Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 6:
 * Function insert() (Program 8.5) is a complete C++ program except for a few
 * lines of pseudocode. Replace the pseudocode by actual C++ code that places
 * all identifiers in page p into the temp area and then rehashes those
 * identifiers into either page p or q. 
 * 
 * Task 7:
 * Program 8.5 contains a reference to a function coalesce() that combines the
 * identifiers in two pages into a single page. Using the types and functions
 * already defined, write a C++ version of this function.
 * 
 */

#include <string>
#include <iostream>
#include <cstring>

const int WordSize = 5; // Maximum number of directory bits
const int PageSize = 10; // Maximum size of a page
const int MaxDir = 32; // Maximum size of a directory

struct TwoChars {
    char str[2];
};

struct page {
    int LocalDepth; // Number of bits to distinguish identifiers
    TwoChars names[PageSize]; // Actual identifiers
    int NumIdents; // Number of identifiers in this page
};

using paddr = page*;

struct record {
    TwoChars KeyField;
    int intData; // A number
    char charData; // An alphabetic letter
};

paddr rdirectory[MaxDir]; // Will contain pointers to pages
int gdepth; // Not to exceed WordSize

// Convert a character to its 3-bit binary representation
std::string charToBinary(char c) {
    switch (c) {
        case 'A': return "100";
        case 'B': return "101";
        case 'C': return "110";
        case '0': return "000";
        case '1': return "001";
        case '2': return "010";
        case '3': return "011";
        case '4': return "100";
        case '5': return "101";
        default: return "000";
    }
}

// Convert a key (TwoChars) to its binary representation
std::string keyToBinary(const TwoChars& key) {
    return charToBinary(key.str[0]) + charToBinary(key.str[1]);
}

// Convert a binary string to its unsigned integer value
unsigned int getBinaryValue(const std::string& binary) {
    unsigned int value = 0;
    for (size_t i = 0; i < binary.length(); i++) {
        if (binary[i] == '1') {
            value |= (1 << (binary.length() - 1 - i));
        }
    }
    return value;
}

// Hash function to map a key to a page
paddr hash(const TwoChars& key, const int precision) {
    std::string binaryKey = keyToBinary(key); // Convert the key to its binary representation
    unsigned int hashValue = 0;

    // Use the last 'precision' bits for hashing
    for (int i = 0; i < precision && i < binaryKey.length(); ++i) {
        if (binaryKey[binaryKey.length() - 1 - i] == '1') { // Extract LSBs from the binary string
            hashValue |= (1 << i); // Set the corresponding bit in the hash value
        }
    }

    return rdirectory[hashValue & ((1 << precision) - 1)]; // Mask the hash value to fit within the directory size
}

// Convert a page pointer to its equivalent directory index
int convert(const paddr p) {
    if (!p) return -1;
    for (int i = 0; i < MaxDir; ++i) {
        if (rdirectory[i] == p) {
            return i;
        }
    }
    return -1;
}

// Get the buddy page of a given page
paddr buddy(const paddr index) {
    int pageIndex = convert(index);
    int localDepth = index->LocalDepth;
    int buddyIndex = pageIndex ^ (1 << (localDepth - 1));
    return rdirectory[buddyIndex];
}

// Get the number of identifiers in a page
int size(const paddr ptr) {
    return ptr ? ptr->NumIdents : 0;
}

// Coalesce two pages if possible
paddr coalesce(const paddr ptr, const paddr buddy) {
    if (!ptr || !buddy) return nullptr;
    if (ptr->LocalDepth != buddy->LocalDepth) return nullptr;

    paddr q = new page();
    q->LocalDepth = ptr->LocalDepth - 1;
    q->NumIdents = 0;

    // Merge identifiers from both pages into the new page
    for (int i = 0; i < ptr->NumIdents; ++i) {
        q->names[q->NumIdents++] = ptr->names[i];
    }
    for (int i = 0; i < buddy->NumIdents; ++i) {
        q->names[q->NumIdents++] = buddy->names[i];
    }

    // Update directory entries
    int ptrIndex = convert(ptr);
    int buddyIndex = convert(buddy);
    for (int i = 0; i < MaxDir; ++i) {
        if (rdirectory[i] == ptr || rdirectory[i] == buddy) {
            rdirectory[i] = q;
        }
    }

    delete ptr;
    delete buddy;
    return q;
}

// Search for a key in a page
bool PageSearch(const TwoChars& key, const paddr index) {
    if (!index) return false;
    for (int i = 0; i < index->NumIdents; ++i) {
        if (key.str[0] == index->names[i].str[0] && key.str[1] == index->names[i].str[1]) {
            return true;
        }
    }
    return false;
}

void enter(const record& r, paddr p) {
    if (!p || p->NumIdents >= PageSize) return;
    int pos = p->NumIdents;
    while (pos > 0 && strcmp(p->names[pos - 1].str, r.KeyField.str) > 0) {
        p->names[pos] = p->names[pos - 1];
        pos--;
    }
    
    p->names[pos] = r.KeyField;
}

// Delete a key from a page
void PageDelete(const TwoChars& key, const paddr p) {
    if (!p) return;
    for (int i = 0; i < p->NumIdents; ++i) {
        if (key.str[0] == p->names[i].str[0] && key.str[1] == p->names[i].str[1]) {
            for (int j = i; j < p->NumIdents - 1; ++j) {
                p->names[j] = p->names[j + 1];
            }
            p->NumIdents--;
            return;
        }
    }
}

// Find the page containing a key
paddr find(const TwoChars& key) {
    paddr index = hash(key, gdepth);
    int IntIndex = convert(index);
    paddr ptr = rdirectory[IntIndex];
    return PageSearch(key, ptr) ? ptr : nullptr;
}

void insert(const record& r, const TwoChars& key) {
    paddr p = find(key);
    if (p) return;
    
    // Get the page where the key should be inserted
    p = hash(key, gdepth);
    if (!p) {
        // Create new page if it doesn't exist
        p = new page();
        p->LocalDepth = gdepth;
        p->NumIdents = 0;
        unsigned int hashVal = getBinaryValue(keyToBinary(key).substr(6 - gdepth));
        rdirectory[hashVal] = p;
    }
    
    if (p->NumIdents < 2) {
        enter(r, p);
        p->NumIdents++;
    } else {
        // Split the page
        if (p->LocalDepth == gdepth) {
            // Need to increase global depth
            if (gdepth >= WordSize) {
                std::cout << "Error: Maximum global depth exceeded\n";
                return;
            }
            gdepth++;
            
            // Double the directory size
            for (int i = (1 << (gdepth - 1)); i < (1 << gdepth); i++) {
                rdirectory[i] = rdirectory[i - (1 << (gdepth - 1))];
            }
        }
        
        // Create new page
        paddr q = new page();
        q->LocalDepth = p->LocalDepth + 1;
        q->NumIdents = 0;
        
        // Store all keys (including the new one) temporarily
        TwoChars tempKeys[3];
        for (int i = 0; i < p->NumIdents; i++) {
            tempKeys[i] = p->names[i];
        }
        tempKeys[2] = key;
        
        // Clear original page and increase its local depth
        p->NumIdents = 0;
        p->LocalDepth++;
        
        // Update directory pointers
        std::string binaryKey = keyToBinary(key);
        int localMask = (1 << p->LocalDepth) - 1;
        for (int i = 0; i < (1 << gdepth); i++) {
            if (rdirectory[i] == p) {
                if ((i & localMask) != (getBinaryValue(binaryKey.substr(6 - p->LocalDepth)))) {
                    rdirectory[i] = q;
                }
            }
        }
        
        // Rehash all keys into page p or q.
        for (int i = 0; i < 3; i++) {
            record tempRecord = {tempKeys[i], 0, 'A'};
            insert(tempRecord, tempKeys[i]);
        }
    }
}


// Delete a key from the extendible hash table
void Delete(const TwoChars& key) {
    paddr p = find(key);
    if (p) {
        PageDelete(key, p);
        if (size(p) + size(buddy(p)) <= PageSize) coalesce(p, buddy(p));
    }
}

// Initialize the directory
void initializeDirectory() {
    gdepth = 0; // Start with global depth of 0
    for (int i = 0; i < MaxDir; ++i) {
        rdirectory[i] = nullptr;
    }
    paddr initialPage = new page();
    initialPage->LocalDepth = 0; // Local depth matches global depth initially
    initialPage->NumIdents = 0;
    rdirectory[0] = initialPage; // Only one directory entry at global depth 0
}

// Print the directory and page contents
void printPages() {
    std::cout << "=== Directory Structure ===\n";
    std::cout << "Global Depth: " << gdepth << "\n";
    for (int i = 0; i < (1 << gdepth); i++) {
        paddr page = rdirectory[i];
        if (!page) continue;

        // Avoid printing the same page multiple times.
        bool alreadyPrinted = false;
        for (int j = 0; j < i; j++) {
            if (rdirectory[j] == page) {
                alreadyPrinted = true;
                break;
            }
        }
        if (alreadyPrinted) continue;

        std::cout << "Directory[" << i << "] -> Page Contents:\n";
        std::cout << "  Local Depth: " << page->LocalDepth << "\n";
        std::cout << "  Number of Identifiers: " << page->NumIdents << "\n";
        std::cout << "  Identifiers: ";
        for (int j = 0; j < page->NumIdents; j++) {
            std::cout << page->names[j].str[0] << page->names[j].str[1] << " ";
        }
        std::cout << "\n";

        std::cout << "  Binary representations:\n";
        for (int j = 0; j < page->NumIdents; j++) {
            TwoChars key = page->names[j];
            std::string binaryKey = keyToBinary(key);
            std::cout << "    " << key.str[0] << key.str[1] << ": " << binaryKey << "\n";
        }

        if (i < (1 << gdepth) - 1) std::cout << "\n";
    }
}

int main() {
    initializeDirectory();

    record r1 = { {'A', '0'}, 0, 'A' };
    record r2 = { {'B', '0'}, 0, 'B' };
    record r3 = { {'C', '2'}, 2, 'C' };
    record r4 = { {'A', '1'}, 1, 'A' };
    record r5 = { {'B', '1'}, 1, 'B' };
    record r6 = { {'C', '3'}, 3, 'C' };
    record r7 = { {'C', '5'}, 5, 'C' };
    record r8 = { {'C', '1'}, 1, 'C' };

    insert(r1, r1.KeyField);
    insert(r2, r2.KeyField);
    insert(r3, r3.KeyField);
    insert(r4, r4.KeyField);
    insert(r5, r5.KeyField);
    insert(r6, r6.KeyField);
    std::cout << "After inserting initial six records:\n";
    printPages();
    std::cout << std::endl;
    
    insert(r7, r7.KeyField);
    std::cout << "After inserting record C5:\n";
    printPages();
    std::cout << std::endl;
    
    insert(r8, r8.KeyField);
    std::cout << "After inserting record C1:\n";
    printPages();
    std::cout << std::endl;
    
    Delete(r8.KeyField);
    std::cout << "After deleting record C1:\n";
    printPages();
    std::cout << std::endl;
    
    Delete(r7.KeyField);
    std::cout << "After deleting record C5:\n";
    printPages();
    std::cout << std::endl;
    
    return 0;
}
