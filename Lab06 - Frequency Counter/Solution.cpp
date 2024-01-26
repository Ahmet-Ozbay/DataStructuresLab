#include <iostream>
#include "WordFreq.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>   
#include <map>
#include <algorithm>
#include <cctype>

using namespace std;

// Helper function to check if a string contains only space characters
bool isOnlySpaces(const string& str) {
    return all_of(str.begin(), str.end(), isspace);
}

// Helper function to convert uppercase letters to lowercase
string toLowerCase(const string& str) {
    string lowerCaseStr;
    transform(str.begin(), str.end(), back_inserter(lowerCaseStr), ::tolower);
    return lowerCaseStr;
}

// Processes the given text file and returns the most frequent 'k' words
vector<WordFreq> GetMostFrequentKWords(string filename, int k) {
    map<string, int> wordCount; // Map to keep track of word frequency
    vector<WordFreq> result;    // Vector to store the results
    string word;                // Variable to store each word read from the file
    ifstream file(filename);    // File stream for reading the text file

    // Check if the file is open successfully
    if (!file.is_open()) {
        cerr << "Error: File could not be opened - " << filename << endl;
        return result; // Return an empty result if the file cannot be opened
    }

    // Read words from the file one by one
    while (file >> word) {
        // Remove punctuation and spaces from the word
        word.erase(remove_if(word.begin(), word.end(), [](char c) { return !isalnum(c); }), word.end());

        // Convert the word to lowercase to make the process case-insensitive
        word = toLowerCase(word);

        // Skip words that are empty, only spaces, or shorter than three characters
        if (word.empty() || isOnlySpaces(word) || word.size() <= 2) {
            continue;
        }

        // Increment the frequency count for the word
        ++wordCount[word];
    }

    // Check for errors during file reading, e.g., if the end of file is not reached
    if (!file.eof() && file.fail()) {
        cerr << "Error: A problem occurred while reading the file - " << filename << endl;
        return result; // Return an empty result if there is a reading error
    }

    // Transfer the word counts from the map to a vector for sorting
    vector<pair<string, int>> pairs;
    for (auto& it : wordCount) {
        pairs.push_back(it);
    }

    // Sort the words by frequency in descending order
    sort(pairs.begin(), pairs.end(), [](pair<string, int>& a, pair<string, int>& b) {
        return a.second > b.second;
        });

    // Take the most frequent 'k' words and store them in the result vector
    for (int i = 0; i < k && i < pairs.size(); ++i) {
        result.push_back(WordFreq(pairs[i].first));
        result.back().freq = pairs[i].second;
    }

    // Return the final list of the most frequent words
    return result;
}
