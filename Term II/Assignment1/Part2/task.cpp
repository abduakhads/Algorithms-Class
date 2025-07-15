/* 
 - ? -> one any char
 - * -> zero or more any char
 - \\ -> escaping above wildcards
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <unordered_set>
#include <cstdlib>

#include <random>
#include <algorithm>

using namespace std;


// --- HELPER FUNCS DECLARATION ---
string readFromFileAndSearch(const string&);
void printIndexes(const vector<int>&);
double measureExecutionTime(const string&, const string&, vector<int> (*)(const string&, const string&));
void saveResults(const string&, int, const vector<string>&, const vector<double>&);
void compare(const string&, const string&, const string&, vector<string>, vector< vector<int> (*)(const string&, const string&) >);



// --- ALGORITHMS ---

// Calculates the number of wildcards in pattern which are needed to substract in the main loop
// Returns: number of wildcards (* and escaped ? or *)
int prep(const string& pattern) {
    int a = 0;  // the number which will be used to calc chars in pattern
    int i = 0;
    while (i < pattern.length()) {
        // adds 1 when * or ? has \\ before it
        if (pattern[i] == '\\' && i + 1 < pattern.length() && 
            (pattern[i + 1] == '*' || pattern[i + 1] == '?')) {
            a++;
            i++;
        }
        // can be zero chars when *
        else if (pattern[i] == '*') {
            a++;
        }
        i++;
    }
    return a;
}


bool brute_force_wildcard(const string& text, const string& pattern) {
    int tlen = text.length();
    int plen = pattern.length();
    int m = prep(pattern);
    
    int ti = 0;
    int pi = 0;

    if (tlen < plen - m) {
        cout << pattern << " is larger than given text" << endl;
        return false;
    }
    
    while (ti <= tlen - plen + m) {
        pi = 0;
        while (pi < plen && ti < tlen) {
            // Checking for escape char
            if (pattern[pi] == '\\') {
                if (pi + 1 < pattern.length() && 
                    (pattern[pi + 1] == '?' || pattern[pi + 1] == '*')) {
                    pi++;
                }
                if (text[ti] == pattern[pi]) {
                    pi++;
                    ti++;
                }
                else {
                    ti++;
                    break;
                }
            }
            else {
                if (pattern[pi] == '?') {
                    pi++;
                    ti++;
                }
                else if (pattern[pi] == '*') {
                    // if * is at the and it will match any number and type of char
                    if (pi == plen - 1) {
                        return true;
                    }
                    // handles multiple consecutive occurrences of char in text which is pattern[pi + 1] after '*'
                    while (ti < tlen) {
                        if (text[ti] == pattern[pi + 1]) {
                            while (ti < tlen && text[ti] == pattern[pi + 1]) {
                                ti++;
                            }
                            pi += 2;
                            break;
                        }
                        else {
                            ti++;
                        }
                    }
                }
                else {
                    if (text[ti] == pattern[pi]) {
                        pi++;
                        ti++;
                    }
                    else {
                        ti++;
                        break;
                    }
                }
            }
        }
        // checks for cases when * may occur at the end
        if (pi == plen || (pattern[pi] == '*' && pi == plen - 1 && pattern[pi - 1] != '\\')) {
            return true;
        }
    }
    return false;
}


bool sunday_search_wildcard(const string& text, const string& pattern) {
    int text_len = text.length();
    int pattern_len = pattern.length();
    int m = prep(pattern); 

    if (text_len < pattern_len - m) {
        cout << pattern << " is larger than given text" << endl;
        return false;
    }

    // creates shift table for ASCII
    vector<int> shift(256, pattern_len + 1);
    for (int i = 0; i < pattern_len; i++) {
        if (pattern[i] != '*' && pattern[i] != '?' && 
            (pattern[i] != '\\' || i + 1 >= pattern_len)) {
            shift[pattern[i]] = pattern_len - i;
        }
    }

    int i = 0;
    // slides the pattern over the text
    while (i <= text_len - pattern_len + m) {
        bool match = true;
        int j = 0;  // pattern index
        int k = i;  // text index
        
        // tries to match the pattern at current position
        while (j < pattern_len && k < text_len) {

            // handles escaped wildcards (\* or \?)
            if (pattern[j] == '\\') {
                if (j + 1 < pattern_len && 
                    (pattern[j + 1] == '?' || pattern[j + 1] == '*')) {
                    j++;
                    if (text[k] != pattern[j]) {
                        match = false;
                        break;
                    }
                    j++;
                    k++;
                    continue;
                }
            }
            
            // handles single-character wildcard '?'
            if (pattern[j] == '?') {
                j++;
                k++;
            }

            // handles multi-character wildcard '*'
            else if (pattern[j] == '*') {

                // if '*' is the last pattern character, it matches the rest of the text
                if (j == pattern_len - 1) {
                    return true;
                }
                j++;

                // skips text characters until the next pattern character matches
                while (k < text_len) {
                    if (text[k] == pattern[j]) {
                        break;
                    }
                    k++;
                }

                // if no match found for the next pattern character
                if (k == text_len) {
                    match = false;
                    break;
                }
            }

            // handles normal character match
            else {
                if (k >= text_len || text[k] != pattern[j]) {
                    match = false;
                    break;
                }
                j++;
                k++;
            }
        }

        // if full pattern matches or ends with '*', return true
        if (match && (j == pattern_len || 
            (pattern[j] == '*' && j == pattern_len - 1))) {
            return true;
        }

        // uses shift table to skip ahead efficiently (Sunday algorithm)
        if (i + pattern_len - m < text_len) {
            i += shift[text[i + pattern_len - m]];
        }
        else {
            i++;
        }
    }
    
    // No match found
    return false;
}


// --- MAIN ---
int main() {
    vector<pair<string, string>> true_tests = {
        // beginning tests
        {"dsabcd", "*abcd"},
        {"abcd", "*abcd"},
        {"*abcd", "\\*abcd"},
        {"\\abcd", "\\abcd"},
        {"aabcd", "?abcd"},
        {"?abcd", "\\?abcd"},
        {"dabcd", "?*abcd"},
        {"dfabcd", "?*abcd"},
        // middle tests
        {"abcd", "ab*cd"},
        {"abascd", "ab*cd"},
        {"ab*cd", "ab\\*cd"},
        {"ab\\cd", "ab\\cd"},
        {"abbcd", "ab?cd"},
        {"ab?cd", "ab\\?cd"},
        {"abrcd", "ab?*cd"},
        {"abreqwcd", "ab?*cd"},
        // end tests
        {"abcd", "abcd*"},
        {"abcdds", "abcd*"},
        {"abcd*", "abcd\\*"},
        {"abcd\\", "abcd\\"},
        {"abcdd", "abcd?"},
        {"abcd?", "abcd\\?"},
        {"abcdq", "abcd?*"},
        {"abcdreqw", "abcd?*"}
    };

    vector<pair<string, string>> false_tests = {
        // beginning tests
        {"adbcd", "*abcd"},
        {"abcdf", "\\*abcd"},
        {"abcdg", "\\abcd"},
        {"abcdr", "?abcd"},
        {"aabcd", "\\?abcd"},
        {"abcdt", "?*abcd"},
        // middle tests
        {"abad", "ab*cd"},
        {"abacd", "ab\\*cd"},
        {"abdcd", "ab\\cd"},
        {"abdscd", "ab?cd"},
        {"ab?dcd", "ab\\?cd"},
        {"abrcad", "ab?*cd"},
        // end tests
        {"abcg", "abcd*"},
        {"abcdq", "abcd\\*"},
        {"abcdf", "abcd\\"},
        {"abcad", "abcd?"},
        {"abcdq?", "abcd\\?"},
        {"abc?cd", "abcd?*"}
    };

    cout << "\nRunning tests...\n\n";
    
    bool all_passed = true;
    
    // Test cases that should return true
    for (const auto& test : true_tests) {
        bool bf_result = brute_force_wildcard(test.first, test.second);
        bool sunday_result = sunday_search_wildcard(test.first, test.second);
        
        if (!bf_result || !sunday_result) {
            cout << "Failed test (should be true):\n";
            cout << "Text: " << test.first << "\nPattern: " << test.second << "\n";
            cout << "Brute Force: " << bf_result << "\nSunday: " << sunday_result << "\n\n";
            all_passed = false;
        }
    }

    // Test cases that should return false
    for (const auto& test : false_tests) {
        bool bf_result = brute_force_wildcard(test.first, test.second);
        bool sunday_result = sunday_search_wildcard(test.first, test.second);
        
        if (bf_result || sunday_result) {
            cout << "Failed test (should be false):\n";
            cout << "Text: " << test.first << "\nPattern: " << test.second << "\n";
            cout << "Brute Force: " << bf_result << "\nSunday: " << sunday_result << "\n\n";
            all_passed = false;
        }
    }

    if (all_passed) {
        cout << "\033[32mAll tests passed successfully!\033[0m\n\n";
    }

    return 0;
}



// --- HELPER FUNCS ---
string readFromFileAndSearch(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filePath << endl;
        return "";
    }

    string text;
    string line;
    while (getline(file, line)) {
        text += line + " ";
    }

    file.close();

    return text;
}

void printIndexes(const vector<int>& vec) {
    for (const int& i : vec) {
        cout << i << " ";
    }
    cout << endl;
}

double measureExecutionTime(const string& text, 
                            const string& pattern, 
                            vector<int> (*searchFunc)(const string&, const string&)) 
{    
    auto start = chrono::high_resolution_clock::now();
    vector<int> result = searchFunc(text, pattern);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, micro> duration = end - start;
    return duration.count();

    
    //skip
    double totalDuration = 0.0;
    const int iterations = 20;
                            
    // Warm-up runs
    for (int i = 0; i < 5; ++i) {
        searchFunc(text, pattern);
    }

    for (int i = 0; i < iterations; ++i) {
        auto start = chrono::high_resolution_clock::now();
        vector<int> result = searchFunc(text, pattern);
        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double, micro> duration = end - start;
        totalDuration += duration.count();
    }

    return totalDuration / iterations;
}

// --- SAVE TO CSV ---
void saveResults(const string& filename,
                int size, 
                const vector<string>& funcNames, 
                const vector<double>& times) 
{
    ofstream file(filename, ios::app); // append mode

    if (file.tellp() == 0) {
        // writing CSV header
        file << "Size";
        for (string names : funcNames) {
            file << "," << names;
        }
        file << "\n";
    }

    file << size;
    for (double time : times) {
        file << "," << time;
    }
    file << "\n";
    file.close();
}