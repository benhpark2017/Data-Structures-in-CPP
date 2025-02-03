/**
 * Solutions to Chapter 8, Section 3, Exercise 1 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 1:
 * The text points out that nonuniform distributions of keys cause a skewed
 * directory and a waste of directory space. One way to avoid this problem
 * in the directory scheme is to store the directory not as a table but as a
 * forest of tries. A new key is hashed to one of the tries, and then its
 * nodes are traversed to a leaf node that points to a page containing the
 * desired record. Splitting is still required. The tries grow and contract
 * with respect to the file. Write out the algorithms for maintaining a trie
 * as a directory.
 */

#include <iostream>
#include <cstring>
#include <bitset>

const int WordSize = 5;    // maximum number of directory bits
const int PageSize = 2;   // maximum size of a page (reduced for testing)
const int MaxDir = 32;     // maximum size of a directory

struct TwoChars {
    char str[3];
    // Helper to convert TwoChars to binary string
    std::string toBinary() const {
        std::string binary;
        // First character (letter)
        switch(str[0]) {
            case 'A': binary = "100"; break;
            case 'B': binary = "101"; break;
            case 'C': binary = "110"; break;
            default: throw std::runtime_error("Invalid identifier");
        }
        // Second character (number)
        int num = str[1] - '0';
        binary += std::bitset<3>(num).to_string();
        return binary;
    }
};

struct page {
    int LocalDepth;	// number of bits to distinguish identifiers
    TwoChars names[PageSize]; // actual identifiers
    int NumIdents;	// number of identifiers in this page
    page *left, *right;    // trie pointers
    page(int depth = 0) : LocalDepth(depth), NumIdents(0), left(nullptr), right(nullptr) {}
};

using paddr = page*;

struct record {
    TwoChars KeyField;
    int intData;
    char charData;
};

class TrieDirectory {
  private:
    paddr rdirectory[MaxDir];  // directory of pages
    int gdepth;	// global depth

    // Helper function to get bit at position
    bool getBit(const std::string& binary, int pos) {
        if (pos >= binary.length()) return false;
        return binary[binary.length() - 1 - pos] == '1';
    }

    void printPage(paddr p, int indent) {
        if(!p) return;
        std::string indentation(indent * 2, ' ');
        // Print page information
        std::cout << indentation << "Local Depth: " << p->LocalDepth << "\n";
        std::cout << indentation << "Number of Identifiers: " << p->NumIdents << "\n";
        std::cout << indentation << "Identifiers: ";
        // Print identifiers in this page
        for(int i = 0; i < p->NumIdents; i++) {
            std::cout << p->names[i].str << " ";
        }
        std::cout << "\n";
        // Print binary representation of each identifier
        std::cout << indentation << "Binary representations:\n";
        for(int i = 0; i < p->NumIdents; i++) {
            std::cout << indentation << "  " << p->names[i].str << ": " 
                      << p->names[i].toBinary() << "\n";
        }
        // Print child nodes if they exist
        if(p->left) {
            std::cout << indentation << "Left child:\n";
            printPage(p->left, indent + 1);
        }
        if(p->right) {
            std::cout << indentation << "Right child:\n";
            printPage(p->right, indent + 1);
        }
        std::cout << "\n";
    }

    // Add helper method to check if global depth can be reduced
    bool canReduceGlobalDepth() {
        int maxLocalDepth = 0;
        // Check all valid directory entries
        for (int i = 0; i < (1 << gdepth); i++) {
            if (rdirectory[i]) {
                maxLocalDepth = std::max(maxLocalDepth, rdirectory[i]->LocalDepth);
            }
        }
        // Global depth can be reduced if all local depths are less than current global depth
        return maxLocalDepth < gdepth;
    }

    // Add helper method to update global depth
    void updateGlobalDepth() {
        while (canReduceGlobalDepth() && gdepth > 0) {
            gdepth--;
            // Clean up unused directory entries
            for (int i = (1 << gdepth); i < (1 << (gdepth + 1)); i++) {
                rdirectory[i] = nullptr;
            }
        }
    }

  public:
    TrieDirectory() : gdepth(0) {
        // Initialize first page
        rdirectory[0] = new page();
        for(int i = 1; i < MaxDir; i++) {
            rdirectory[i] = nullptr;
        }
    }

    paddr hash(const TwoChars& key, const int precision) {
        std::string binary = key.toBinary();
        int index = 0;
        // Use last 'precision' bits for hashing
        for(int i = 0; i < precision && i < binary.length(); i++) {
            if(getBit(binary, i)) {
                index |= (1 << i);
            }
        }
        return rdirectory[index];
    }

    paddr buddy(const paddr ptr) {
        // Find buddy by traversing up the trie and complementing the leading bit
        if(!ptr) return nullptr;
        int depth = ptr->LocalDepth;
        if(depth == 0) return nullptr;
        // Find the buddy's index by flipping the bit at LocalDepth-1
        int index = convert(ptr);
        index ^= (1 << (depth - 1));
        return rdirectory[index];
    }

    int size(const paddr ptr) {
        return ptr ? ptr->NumIdents : 0;
    }

    paddr coalesce(const paddr ptr, const paddr bud) {
        if (!ptr || !bud || ptr->NumIdents + bud->NumIdents > PageSize) {
            return ptr;
        }

        // Copy records from buddy to ptr
        for (int i = 0; i < bud->NumIdents; i++) {
            ptr->names[ptr->NumIdents + i] = bud->names[i];
        }
        ptr->NumIdents += bud->NumIdents;

        // Update directory entries
        int index = convert(bud);
        delete bud;
        rdirectory[index] = ptr;
        ptr->LocalDepth--;

        // Check and update global depth if necessary
        updateGlobalDepth();

        return ptr;
    }

    bool PageSearch(const TwoChars& key, const paddr index) {
        if(!index) return false;
        for(int i = 0; i < index->NumIdents; i++) {
            if(strcmp(key.str, index->names[i].str) == 0) {
                return true;
            }
        }
        return false;
    }

    int convert(const paddr p) {
        if(!p) return -1;
        // Find the index in rdirectory that points to p
        for(int i = 0; i < MaxDir; i++) {
            if(rdirectory[i] == p) return i;
        }
        return -1;
    }

    void enter(const record r, const paddr p) {
        if(!p || p->NumIdents >= PageSize) return;
        p->names[p->NumIdents] = r.KeyField;
    }

    void PageDelete(const TwoChars& key, const paddr p) {
        if(!p) return;
        for(int i = 0; i < p->NumIdents; i++) {
            if(strcmp(key.str, p->names[i].str) == 0) {
                // Move remaining records up
                for(int j = i; j < p->NumIdents - 1; j++) {
                    p->names[j] = p->names[j + 1];
                }
                p->NumIdents--;
                return;
            }
        }
    }

    paddr find(const TwoChars* key) {
        if(!key) return nullptr;
        paddr index = hash(*key, gdepth);
        if(!index) return nullptr;
        int IntIndex = convert(index);
        paddr ptr = rdirectory[IntIndex];
        if(PageSearch(*key, ptr)) return ptr;
        return nullptr;
    }

    void insert(const record& r, const TwoChars* key) {
        if (!key) return;
    
        // Check if the key already exists in the trie
        paddr p = find(key);
        if (p) return; // Key already exists, no need to insert again
    
        std::string binary = key->toBinary();
        int index = 0;
    
        // Compute the directory index using the current global depth
        for (int i = 0; i < gdepth; i++) {
            if (getBit(binary, i)) index |= (1 << i);
        }
    
        p = rdirectory[index];
    
        if (!p || p->NumIdents < PageSize) {
            // Page has space or doesn't exist
            if (!p) {
                p = new page(gdepth);
                rdirectory[index] = p;
            }
            enter(r, p);
            p->NumIdents++;
        } else {
            // Split needed
            if (gdepth >= WordSize) {
                std::cerr << "Error: Maximum directory size exceeded\n";
                return;
            }
    
            // Create a new page for the split
            paddr newPage = new page(gdepth + 1);
    
            // Increase the local depth of the original page
            p->LocalDepth = gdepth + 1;
            newPage->LocalDepth = gdepth + 1;
    
            // Redistribute entries between the original page and the new page
            int j = 0; // Index for retaining keys in the original page
            for (int i = 0; i < p->NumIdents; i++) {
                std::string entryBinary = p->names[i].toBinary();
                if (getBit(entryBinary, gdepth)) {
                    // Move key to the new page
                    newPage->names[newPage->NumIdents++] = p->names[i];
                } else {
                    // Retain key in the original page
                    p->names[j++] = p->names[i];
                }
            }
            p->NumIdents = j; // Update the number of keys in the original page
    
            // Update directory entries for both pages
            int newIndex = index | (1 << gdepth);
            rdirectory[newIndex] = newPage;
    
            // Increment global depth
            gdepth++;
    
            // Retry the insertion after the split
            insert(r, key);
        }
    }

    void Delete(const TwoChars& key) {
        paddr p = find(&key);
        if(p) {
            PageDelete(key, p);
            paddr bud = buddy(p);
            if(bud && size(p) + size(bud) <= PageSize) {
                coalesce(p, bud);
            }
        }
    }

    void printTrie() {
        std::cout << "\n=== Directory Structure ===\n";
        std::cout << "Global Depth: " << gdepth << "\n\n";
        // Print each directory entry
        for(int i = 0; i < (1 << gdepth); i++) {
            if(rdirectory[i]) {
                std::cout << "Directory[" << i << "] -> Page Contents:\n";
                printPage(rdirectory[i], 1);
            }
        }
    }
};

int main() {
    TrieDirectory dir;
    // Create test records
    record r1 = {{"A0"}, 0, 'A'};
    record r2 = {{"B0"}, 0, 'B'};
    record r3 = {{"C2"}, 2, 'C'};
    record r4 = {{"A1"}, 1, 'A'};
    record r5 = {{"B1"}, 1, 'B'};
    record r6 = {{"C3"}, 3, 'C'};
    record r7 = {{"C5"}, 5, 'C'};
    record r8 = {{"C1"}, 1, 'C'};
    
    // Test insertions
    dir.insert(r1, &r1.KeyField);
    dir.insert(r2, &r2.KeyField);
    dir.insert(r3, &r3.KeyField);
    dir.insert(r4, &r4.KeyField);
    dir.insert(r5, &r5.KeyField);
    dir.insert(r6, &r6.KeyField);
    dir.printTrie();
    
    dir.insert(r7, &r7.KeyField);
    dir.printTrie();    
    
    dir.insert(r8, &r8.KeyField);
    dir.printTrie();
    
    std::cout << "After deletion of C1, the trie directory should be \n"
              << "identical to the directory after insertion of C5." << std::endl;
    dir.Delete(r8.KeyField);
    dir.printTrie();
    std::cout << std::endl;
    
    std::cout << "After deletion of C5, the trie directory should be \n"
              << "identical to the directory after six initial insertions." << std::endl;
    dir.Delete(r7.KeyField);
    dir.printTrie();
    std::cout << std::endl;
    
    return 0;
}
