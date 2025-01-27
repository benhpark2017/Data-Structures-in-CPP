/**
 * Solutions to Chapter 7, Section 9, Exercise 6 of Horowitz, Sahni, and Mehta's
 * Fundamentals of Data Structures in C++, 1st Edition.
 * 
 * Task 6: 
 * Assume you are given a list of five-letter English words and are faced with
 * the challenge of listing these words in sequences such that the owrds in each
 * sequence are anagrams (i.e. if x and y are in the same sequence, then word x
 * is a permutation of word y). You are required to list out the fewest such
 * sequences. With this restriction, show that no word can appear in more than 
 * one sequence. How would you go about solving this problem?
 * 
 * Our approach:
 * Sort the letters of each word: For each word, sort its letters alphabetically.
 * This sorted version will serve as a unique key for all anagrams of that word.
 * Group words by their sorted keys: Use a dictionary (or hash map) where the
 * key is the sorted version of the letters, and the value is a list of words
 * that share that sorted version. 
 * Finally, output the groups: Each list in the dictionary corresponds to a 
 * sequence of anagrams.
 */

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

// Function to group anagrams
std::vector<std::vector<std::string>> groupAnagrams(const std::vector<std::string>& words) {
    std::unordered_map<std::string, std::vector<std::string>> anagramGroups;

    // Iterate through each word
    for (const std::string& word : words) {
        // Sort the letters of the word to create a key
        std::string sortedWord = word;
        std::sort(sortedWord.begin(), sortedWord.end());

        // Add the word to the corresponding group in the map
        anagramGroups[sortedWord].push_back(word);
    }

    // Extract the groups from the map into a vector
    std::vector<std::vector<std::string>> result;
    for (const auto& group : anagramGroups) {
        result.push_back(group.second);
    }

    return result;
}

int main() {
    // Example list of five-letter words
    std::vector<std::string> words = {"listen", "silent", "enlist", "google", "gogole", "inlets", "banana"};

    // Group the anagrams
    std::vector<std::vector<std::string>> anagramSequences = groupAnagrams(words);

    // Print the results
    for (const auto& sequence : anagramSequences) {
        for (const std::string& word : sequence) {
            std::cout << word << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
