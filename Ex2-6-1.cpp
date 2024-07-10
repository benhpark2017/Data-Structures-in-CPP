/**
 * Solutions to Chapter 2, Section 6, Exercises 1 to 5 of Horowitz's
 * Data Structures in C++.
 */

/**************************************main.cpp*******************************************/
#include <iostream>
#include "String.h"

int main() {
    // Test constructor and Length
    char init1[] = "Hello";
    String str1(init1, 5);
    std::cout << "String 1: " << init1 << ", Length: " << str1.Length() << std::endl;

    char init2[] = "World";
    String str2(init2, 5);
    std::cout << "String 2: " << init2 << ", Length: " << str2.Length() << std::endl;

    // Test Concat
    String str3 = str1.Concat(str2);
    std::cout << "Concatenated string: " << str3 << std::endl;
    std::cout << "Length of concatenated string: " << str3.Length() << std::endl;

    // Test Substr
    String substr = str3.Substr(3, 4);
    std::cout << "Substring is " << substr << std::endl;
    std::cout << "Substring (3, 4): " << substr.Length() << std::endl;

    // Test Find
    char pattern1[] = "loWo";
    String pat1(pattern1, 4);
    int findIndex = str3.Find(pat1);
    std::cout << "Find 'loWo' in 'HelloWorld': " << findIndex << std::endl;

    // Test FastFind
    int fastFindIndex = str3.FastFind(pat1);
    std::cout << "FastFind 'loWo' in 'HelloWorld': " << fastFindIndex << std::endl;

    // Test Frequency
    char pattern2[] = "l";
    String pat2(pattern2, 1);
    int frequency = str3.Frequency(pat2);
    std::cout << "Frequency of 'l' in 'HelloWorld': " << frequency << std::endl;

    // Test Delete
    String delStr = str3.Delete(3, 4);
    std::cout << "Delete 4 characters from position 3: " << delStr.Length() << std::endl;
    std::cout << "Result after deletion is " << delStr << std::endl;

    // Test CharDelete
    String charDelStr = str3.CharDelete('o');
    std::cout << "Delete all occurrences of 'o': " << charDelStr.Length() << std::endl;
    std::cout << "New string after deleting all 'o's is " << charDelStr << std::endl;

    // Test Equality
    char init3[] = "HelloWorld";
    String str4(init3, 10);
    int isEqual = (str3 == str4);
    if (isEqual) std::cout << "String 3 is equal to String 4." << std::endl;
    else std::cout << "String 3 is not equal to String 4." << std::endl;

    // Test Replace function
    String str5(init3, 10);
    String w("loW", 3);
    String x("Wol", 3);
    String str6 = str5.Replace(w, x);
    std::cout << "Original string is: " << str5 << std::endl;
    std::cout << "The modified string is: " << str6 << std::endl;

    // Test Compare function
    if (Compare(str5, str6) == -1) std::cout << "str5 is less than str6." << std::endl;
    else if (Compare(str5, str6) == 0) std::cout << "str5 is same as str6." << std::endl;
    else if (Compare(str5, str6) == 1) std::cout << "str5 is greater than str6." << std::endl;

    // Test Empty
    String emptyStr("", 0);
    int isEmpty = !emptyStr;
    if (isEmpty) std::cout << "The empty string is empty." << std::endl;
    else std::cout << "The string is not empty." << std::endl;

    return 0;
}



/***********************************String.h****************************************/
#ifndef STRING_H
#define STRING_H

#include <cstring>
#include <iostream>

class String {
  private:
    char *str; // Pointer to the actual string data
    int len;   // Length of the string
    int *f;    // Failure function array for pattern matching

  public:
    String(const char *init, int m); //Constructor.
    ~String(); //Destructor.
    String(const String& other); // Copy constructor
    String& operator=(const String& other); // Copy assignment operator
    
    char At(int index) const; //accessor of pointer *str.
    
    int operator==(String t);
    int operator!();
    int Length() const;
    String Concat(String t);
    String Substr(int i, int j);
    int Find(String pat);
    int FastFind(String pat);
    int Frequency(String pat); //Exercise 1
    String Delete(int start, int length); //Exercise 2
    String CharDelete(char c); //Exercise 3
    void fail();
    String Replace(String w, String x); //Exercise 4

    friend int Compare(const String& s, const String& t); //Added function prototype here
    friend std::ostream &operator<<(std::ostream &os, const String &t);
};

std::ostream &operator<<(std::ostream &os, const String &t);
int Compare(const String& s, const String& t); //Exercise 5

#endif



/***********************************String.cpp**************************************/
#include <iostream>
#include "String.h"

// Constructor
String::String(const char *init, int m) {
    len = m;
    str = new char[len + 1];
    std::strncpy(str, init, len);
    str[len] = '\0';
    f = nullptr;  // Initialize f to nullptr
}

// Copy constructor
String::String(const String& other) {
    len = other.len;
    str = new char[len + 1];
    std::strcpy(str, other.str);
    if (other.f) {
        f = new int[len];
        std::memcpy(f, other.f, len * sizeof(int));
    } else {
        f = nullptr;
    }
}

// Destructor
String::~String() {
    delete[] str;
    delete[] f;
}

char String::At(int index) const {
    if (index >= 0 && index < len) {
        return str[index];
    }
    return '\0';  // Return null character for out-of-bounds access
}

// Copy assignment operator
String& String::operator=(const String& other) {
    if (this != &other) {
        // Allocate new memory first
        char* new_str = new char[other.len + 1];
        int* new_f = other.f ? new int[other.len] : nullptr;

        // Copy data
        for (int i = 0; i < other.len; ++i) {
            new_str[i] = other.str[i];
            if (new_f) {
                new_f[i] = other.f[i];
            }
        }
        new_str[other.len] = '\0';

        // Delete old data
        delete[] str;
        delete[] f;

        // Assign new data
        str = new_str;
        f = new_f;
        len = other.len;
    }
    return *this;
}

// Operator==
int String::operator==(String t) {
    if (len != t.len) return 0;
    for (int i = 0; i < len; ++i) {
        if (str[i] != t.str[i]) return 0;
    }
    return 1;
}

// Operator!
int String::operator!() {
    return len == 0;
}

// Length
int String::Length() const {
    return len;
}

// Concat
String String::Concat(String t) {
    int new_len = len + t.len;
    char *new_str = new char[new_len + 1];
    for (int i = 0; i < len; ++i) {
        new_str[i] = str[i];
    }
    for (int i = 0; i < t.len; ++i) {
        new_str[len + i] = t.str[i];
    }
    new_str[new_len] = '\0';
    return String(new_str, new_len);
}

// Substr
String String::Substr(int i, int j) {
    if (i < 0 || j < 0 || i + j > len) {
        return String("", 0); // Return empty string for invalid positions
    }
    char *substr = new char[j + 1];
    for (int k = 0; k < j; ++k) {
        substr[k] = str[i + k];
    }
    substr[j] = '\0';
    return String(substr, j);
}

// Find
int String::Find(String pat) {
    if (pat.len == 0 || len == 0) return -1;
    for (int i = 0; i <= len - pat.len; ++i) {
        int j = 0;
        while (j < pat.len && str[i + j] == pat.str[j]) {
            ++j;
        }
        if (j == pat.len) return i;
    }
    return -1;
}

int String::FastFind(String pat) {
    if (pat.len == 0 || len == 0) return -1;
    if (pat.f == nullptr) pat.fail();
    int posP = 0, posS = 0;
    while (posS < len) {
        if (posP < pat.len && pat.str[posP] == str[posS]) {
            posP++;
            posS++;
        } else {
            if (posP == 0) posS++;
            else posP = pat.f[posP - 1];
        }
        if (posP == pat.len) return posS - pat.len;
    }
    return -1;
}

// Frequency
// Solution to Chapter 2, Section 6, Exercise 1.
int String::Frequency(String pat) {
    if (pat.len == 0 || len == 0) return 0;
    int freq = 0;
    int pos = 0;
    while (pos < len) {
        int found = this->FastFind(pat);
        if (found == -1) break;
        freq++;
        pos += found + pat.len;
    }
    return freq;
}

// Delete
// Solution to Chapter 2, Section 6, Exercise 2.
String String::Delete(int start, int length) {
    if (start < 0 || start + length > len) {
        return String(str, len); // Invalid range, return original string
    }
    int new_len = len - length;
    char *new_str = new char[new_len + 1];
    for (int i = 0; i < start; ++i) {
        new_str[i] = str[i];
    }
    for (int i = start + length; i < len; ++i) {
        new_str[i - length] = str[i];
    }
    new_str[new_len] = '\0';
    return String(new_str, new_len);
}

// CharDelete
// Solution to Chapter 2, Section 6, Exercise 3.
String String::CharDelete(char c) {
    char *new_str = new char[len + 1];
    int new_len = 0;
    for (int i = 0; i < len; ++i) {
        if (str[i] != c) {
            new_str[new_len++] = str[i];
        }
    }
    new_str[new_len] = '\0';
    return String(new_str, new_len);
}

// Fail
void String::fail() {
    f = new int[len];
    f[0] = -1;
    for (int j = 1; j < len; j++) {
        int i = f[j - 1];
        while (i >= 0 && str[j] != str[i + 1]) {
            i = f[i];
        }
        if (str[j] == str[i + 1]) {
            f[j] = i + 1;
        } else {
            f[j] = -1;
        }
    }
}

// Solution to Chapter 2, Section 6, Exercise 4.
String String::Replace(String w, String x) {
    int start = Find(w);
    if (start == -1) {
        // If w is not found in the string, return the original string
        return *this;
    }
        // Break down the operation into multiple steps
    String prefix = Substr(0, start);
    String suffix = Substr(start + w.Length(), Length() - (start + w.Length()));
    
    // Construct the result
    String result = prefix;
    result = result.Concat(x);
    result = result.Concat(suffix);
    
    return result;
}

//Compare function
//Solution to Chapter 2, Section 6, Exercise 5.
int Compare(const String &s, const String &t) {
    
    char *copy = new char[s.Length() + 1];
    char *copy2 = new char[s.Length() + 1];
    
    for (int i = 0; i < s.Length(); i++)
        copy[i] = s.At(i);
    for (int i = 0; i < t.Length(); i++)
        copy2[i] = t.At(i);
    
    if (s.Length() < t.Length())
        return -1;
    else if (s.Length() == t.Length()) {
        int equal = 0;
        for (int i = 0; i < s.Length(); i++) {
            if (copy[i] != copy2[i] && copy[i] > copy2[i]) {
                equal++;
                break;
            } else if (copy[i] != copy2[i] && copy[i] < copy2[i]) {
                equal--;
                break;
            }
        }
        if (equal == 0)
            return 0;
        else if (equal > 0)
            return 1;
        else
            return -1;
    } else
        return 1;
}

std::ostream& operator<<(std::ostream& os, const String& t) {
    os << t.str;
    return os;
}
