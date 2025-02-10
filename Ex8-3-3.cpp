/**
 * Solutions to Chapter 8, Section 3, Exercise 3 of Horowitz, Sahni, and Mehta's 
 * Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 3:
 * Another way of handling overflows in directory-based dynamic hashing is to
 * permit a page to be divided into as many multiple pages as necessary to hold 
 * all identifiers that hash to that page. Then you assign a limit on the size 
 * of the directory you are willing to accept, and once that size has been
 * reached, a page just continues to grow. Modify the algorithms in Program 8.5
 * to implement this strategy.
 */

#include <string>
#include <iostream>
#include <cstring>
#include <vector>

const int WordSize = 5; // Maximum number of directory bits
const int PageSize = 2; // Maximum of 2 identifiers per page
const int MaxDir = 32; // Maximum size of a directory

struct TwoChars {
    char str[2];
};

struct page {
    int LocalDepth; // Number of bits to distinguish identifiers
    TwoChars names[MaxDir]; // Allow for overflow beyond PageSize when directory is full
    int NumIdents; // Number of identifiers in this page
};

using paddr = page*;

struct record {
    TwoChars KeyField;
    int intData;
    char charData;
};

paddr rdirectory[MaxDir]; // Fixed-size directory
int gdepth; // Global depth

// Convert a character to its 3-bit binary representation
std::string charToBinary(char c) {
    switch (c) {
        case 'A': return "100";
        case 'B': return "101";
        case 'C': return "110";
        case 'D': return "111";
        case '0': return "000";
        case '1': return "001";
        case '2': return "010";
        case '3': return "011";
        case '4': return "100";
        case '5': return "101";
        case '6': return "110";
        case '7': return "111";
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
    if (!p) return;
    
    int pos = p->NumIdents;
    while (pos > 0 && strcmp(p->names[pos - 1].str, r.KeyField.str) > 0) {
        p->names[pos] = p->names[pos - 1];
        pos--;
    }
    p->names[pos] = r.KeyField;
    p->NumIdents++;
}

// Added this helper function to check for duplicates
bool isDuplicate(const TwoChars& key, paddr p) {
    if (!p) return false;
    for (int i = 0; i < p->NumIdents; i++) {
        if (key.str[0] == p->names[i].str[0] && 
            key.str[1] == p->names[i].str[1]) {
            return true;
        }
    }
    return false;
}

void insert(const record& r, const TwoChars& key) {
    paddr p = hash(key, gdepth);
    if (!p) {
        // Create a new page if the directory entry is empty
        p = new page();
        p->LocalDepth = gdepth;
        p->NumIdents = 0;
        unsigned int hashVal = getBinaryValue(keyToBinary(key).substr(6 - gdepth));
        rdirectory[hashVal] = p;
    }

    // Check for duplicates
    if (isDuplicate(key, p)) {
        std::cout << "Alert: Duplicate entry detected for key " 
                  << key.str[0] << key.str[1] 
                  << ". No insertion performed.\n";
        return;
    }

    // If the page is full, split it or grow it
    if (p->NumIdents >= PageSize) {
        // Check if we need to increase global depth
        if (p->LocalDepth == gdepth && gdepth < WordSize) {
            std::cout << "Increasing global depth from " << gdepth 
                      << " to " << (gdepth + 1) << "\n";

            // Double the directory size
            for (int i = (1 << gdepth); i < (1 << (gdepth + 1)); i++) {
                rdirectory[i] = rdirectory[i - (1 << gdepth)];
            }
            gdepth++;
        }

        // Split the page
        std::cout << "Splitting page " << convert(p) << " with local depth " << p->LocalDepth << "\n";
        paddr q = new page();
        q->LocalDepth = p->LocalDepth + 1;
        q->NumIdents = 0;

        // Store all entries including the new one
        std::vector<TwoChars> tempKeys;
        for (int i = 0; i < p->NumIdents; i++) {
            tempKeys.push_back(p->names[i]);
        }
        tempKeys.push_back(key);

        // Clear the original page
        p->NumIdents = 0;
        p->LocalDepth++;

        // Update directory pointers
        int mask = (1 << p->LocalDepth) - 1;
        for (int i = 0; i < (1 << gdepth); i++) {
            if (rdirectory[i] == p) {
                if ((i & mask) >= (1 << (p->LocalDepth - 1))) {
                    rdirectory[i] = q;
                }
            }
        }

        // Reinsert all keys
        for (const auto& tempKey : tempKeys) {
            record tempRecord = {tempKey, 0, 'A'};
            insert(tempRecord, tempKey);
        }
    } else {
        // Insert the key into the page
        enter(r, p);
    }
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

void printPages() {
    std::cout << "=== Directory Structure ===\n";
    std::cout << "Global Depth: " << gdepth << "\n";
    std::cout << "Directory Status: " << (gdepth >= WordSize ? "FULL" : "Not Full") << "\n\n";
    
    for (int i = 0; i < (1 << gdepth); i++) {
        paddr page = rdirectory[i];
        if (!page) continue;

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
        std::cout << "  Identifiers: \n";
        for (int j = 0; j < page->NumIdents; j++) {
            TwoChars key = page->names[j];
            std::string binaryKey = keyToBinary(key);
            std::cout << "    " << key.str[0] << key.str[1] 
                     << " (Binary: " << binaryKey << ")\n";
        }
        std::cout << "\n";

        if (page->NumIdents > PageSize) {
            std::cout << "  ** This page has overflowed beyond " << PageSize << " identifiers **\n";
        }
    }
}

int main() {
    initializeDirectory();

    std::cout << "\n=== Phase 1: Testing Directory Growth and Page Splitting ===\n";
    // Using only A-D and 0-7
    record records[] = {
        {{'A', '0'}, 0, 'A'},  // 100000
        {{'A', '4'}, 4, 'A'},  // 100100
        {{'B', '1'}, 1, 'B'},  // 101001
        {{'B', '5'}, 5, 'B'},  // 101101
        {{'C', '2'}, 2, 'C'},  // 110010
        {{'C', '6'}, 6, 'C'},  // 110110
        {{'D', '3'}, 3, 'D'},  // 111011
        {{'D', '7'}, 7, 'D'}   // 111111
    };

    for (const auto& r : records) {
        std::cout << "\nInserting " << r.KeyField.str[0] << r.KeyField.str[1] 
                  << " (Binary: " << keyToBinary(r.KeyField) << "):\n";
        insert(r, r.KeyField);
        std::cout << "\nDirectory status after insertion:\n";
        printPages();
    }

    std::cout << "\n=== Phase 2: Testing Page Growth Beyond Directory Full ===\n";
    // Additional records that will hash to existing pages
    record overflowRecords[] = {
        {{'A', '1'}, 1, 'A'},  // 100001
        {{'B', '2'}, 2, 'B'},  // 101010
        {{'C', '3'}, 3, 'C'},  // 110011
        {{'D', '4'}, 4, 'D'},  // 111100
        {{'A', '5'}, 5, 'A'},  // 100101
        {{'B', '6'}, 6, 'B'}   // 101110
    };

    for (const auto& r : overflowRecords) {
        std::cout << "\nInserting " << r.KeyField.str[0] << r.KeyField.str[1] 
                  << " (Binary: " << keyToBinary(r.KeyField) << "):\n";
        insert(r, r.KeyField);
        std::cout << "\nDirectory status after insertion:\n";
        printPages();
    }

    std::cout << "\n=== Phase 3: Testing Directory Cleanup ===\n";
    std::cout << "\nDeleting entries to test cleanup:\n";
    Delete({'A', '4'});
    Delete({'B', '5'});
    Delete({'C', '6'});
    printPages();

    std::cout << "\n=== Phase 4: Testing Maximum Overflow ===\n";
    // Try to insert beyond MaxDir using valid characters
    record overflowTest[] = {
        {{'D', '0'}, 0, 'D'},
        {{'D', '1'}, 1, 'D'},
        {{'D', '2'}, 2, 'D'},
        {{'D', '5'}, 5, 'D'},
        {{'D', '6'}, 6, 'D'}
    };

    for (const auto& r : overflowTest) {
        std::cout << "\nTrying to insert " << r.KeyField.str[0] << r.KeyField.str[1] 
                  << " (Binary: " << keyToBinary(r.KeyField) << "):\n";
        insert(r, r.KeyField);
        printPages();
    }

    return 0;
}
