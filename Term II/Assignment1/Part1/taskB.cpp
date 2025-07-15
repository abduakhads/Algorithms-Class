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
vector<int> sundaySearch(const string& text, const string& pattern) {
    vector<int> indices;
    int textLen = text.length();
    int patternLen = pattern.length();

    // creating shift table
    int shift[256]; // ?for ascii only?
    fill(shift, shift + 256, patternLen + 1);
    for (int i = 0; i < patternLen; ++i) {
        // filling values how far to shift
        shift[pattern[i]] = patternLen - i;
    }

    int i = 0;
    while (i <= textLen - patternLen) {

        bool match = true;
        for (int j = 0; j < patternLen; ++j) {
            // on mismatch
            if (text[i + j] != pattern[j]) {
                match = false;
                break;
            }
        }
        // on match when 
        if (match) {
            indices.push_back(i);
        }

        // shift
        if (i + patternLen < textLen) {
            char nextChar = text[i + patternLen];
            i += shift[(unsigned char)nextChar];
        } else {
            break;
        }
    }

    return indices;
}

vector<int> binarySundaySearch(const string& text, const string& pattern) {
    vector<int> indices;
    int textLen = text.length();
    int patternLen = pattern.length();

    unordered_map<char, int> shiftMap;
    for (int i = 0; i < patternLen; ++i) {
        shiftMap[pattern[i]] = patternLen - i;
    }
    
    auto getShift = [&](char c) -> int {
        auto it = shiftMap.find(c);
        return (it != shiftMap.end()) ? it->second : patternLen + 1;
    };

    int i = 0;
    while (i <= textLen - patternLen) {
        bool match = true;
        for (int j = 0; j < patternLen; ++j) {
            if (text[i + j] != pattern[j]) {
                match = false;
                break;
            }
        }

        if (match)
            indices.push_back(i);

        if (i + patternLen < textLen) {
            char nextChar = text[i + patternLen];
            i += getShift(nextChar);
        } else {
            break;
        }
    }

    return indices;
}

vector<int> computeLPS(const string& pattern) {
    int patternLen = pattern.length();
    vector<int> lps(patternLen, 0);
    
    int j = 0, i = 1;
    
    while (i < patternLen) {
        if (pattern[i] == pattern[j]) {
            // on same char
            j++;
            lps[i] = j;
            i++;
        } 
        else if (j != 0) {
            j = lps[j - 1];
        }
        else {
            lps[i] = 0;
            i++;
        }
    }
    return lps;
}

vector<int> KMPMatch(const string& text, const string& pattern) {
    vector<int> lps = computeLPS(pattern);

    int textLen = text.length();
    int patternLen = pattern.length();

    vector<int> indices;
    int i = 0;
    int j = 0;

    while (i < textLen) {
        if (text[i] == pattern[j]) {
            ++i;
            ++j;
        }
        //on match found
        if (j == patternLen) {
            indices.push_back(i - j);
            j = lps[j - 1];
        //on mismatch
        } else if (i < textLen && text[i] != pattern[j]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                ++i;
            }
        }
    }

    return indices;
}


vector<int> rabinKarpMatch(const string& text, const string& pattern) {
    vector<int> indices;

    // long long q = 1000000007;
    int q = 101; // prime num for modulo
    int d = 256; // base 

    int m = pattern.length();
    int n = text.length();
    
    // long long p = 0; 
    // long long t = 0; 
    int p = 0; // pattern hash
    int t = 0; // text hash

    // pre compute radix (h = d^(m-1) % q)
    // long long h = 1;
    int h = 1;
    for (int i = 0; i < m - 1; i++) {
        h = (h * d) % q;
    }

    // calculating pattern hash (+ first window of text)
    for (int i = 0; i < m; i++) {
        p = ((d * p) + pattern[i]) % q;
        t = ((d * t) + text[i]) % q;
    }


    for (int i = 0; i <= n - m; i++) {
        // hash match
        if (p == t) {
            // char to char comparison
            bool match = true;
            for (int j = 0; j < m; j++) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                indices.push_back(i);
            }
        }

        // compute hash of next window in text
        if (i < n - m) {
            t = (d * (t - text[i] * h) + text[i + m]) % q;

            // if t is negative
            if (t < 0) {
                t = (t + q);
            }
        }
    }

    return indices;
}

vector<int> computeZ(const string& str) {
    int n = str.length();
    vector<int> Z(n, 0);
    int L = 0, R = 0; // [L, R] window

    for (int i = 1; i < n; ++i) {
        if (i <= R) {
            // i within [L, R], using computed Z values
            Z[i] = min(R - i + 1, Z[i - L]);
        }
        // trying to extend the Z-box from position i
        while (i + Z[i] < n && str[Z[i]] == str[i + Z[i]]) {
            ++Z[i];
        }
        // if the extended Z-box goes beyond R, updating L and R
        if (i + Z[i] - 1 > R) {
            L = i;
            R = i + Z[i] - 1;
        }
    }

    return Z;
}

vector<int> gusfieldZMatch(const string& text, const string& pattern) {
    string combined = pattern + "$" + text;
    
    vector<int> Z = computeZ(combined);
    vector<int> indices;

    int patternLen = pattern.length();
    for (int i = patternLen + 1; i < Z.size(); ++i) {
        // match is found when Z[i] == pattenr len
        if (Z[i] == patternLen) {
            indices.push_back(i - patternLen - 1);
        }
    }

    return indices;
}


// --- MAIN ---
int main() {

    string output;
    string text;
    string pattern;
    string cmd = "python plotTaskB.py";
    string folder = "results/B/";


    // first - Binary Sunday vs Gusfield Z
    text = "";
    text.reserve(102400);
    string base = "aaaatyuiaaaaqweryaaa";
    int repeatCount = 102400 / base.length();
    
    for (int i = 0; i < repeatCount; i++) {
        text += base;
    }
    
    pattern = "bbbbbbb";
    output = folder + "bsunday_and_z.csv";
    
    compare(text, pattern, output, {"Binary Sunday Search", "GusfieldZ Match"}, {binarySundaySearch, gusfieldZMatch});
    system((cmd + " " + output).c_str());
    

    // second - KMP vs Rabin-Karp
    text = "";
    base = "abcd";
    repeatCount = 25600; // ("abcd" * 25600) = 102400 characters
    
    for (int i = 0; i < repeatCount; ++i) {
        text += base;
    }
    
    pattern = "abcdabcdabcdabcdabcdabcdabcdabcdabcd";
    output = folder + "kmp_and_rabin.csv";
    compare(text, pattern, output, {"KMP Match", "Rabin-Karp Match"}, {KMPMatch, rabinKarpMatch});
    system((cmd + " " + output).c_str());
    
    
    //third - Rabin-Karp vs Sunday
    text = "";
    text = string(102400, 'a');  // 102400 'a' characters
    text += 'b'; 

    pattern = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab";
    output = folder + "rabin_and_sunday.csv";
    compare(text, pattern, output, {"Rabin-Karp Match", "Sunday Search"}, {rabinKarpMatch, sundaySearch});
    system((cmd + " " + output).c_str());
    
    cout << "Results were saved to results/B/ folder" << endl;

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

void compare(const string& text, 
            const string& pattern, 
            const string& fileName,
            vector<string> funcNames,
            vector< vector<int> (*)(const string&, const string&) > funcs) 
{
    if (ifstream(fileName)) {
        ofstream clearFile(fileName, ios::trunc); // clearing file content
        clearFile.close();
    }

    vector<double> times;
    
    for (auto& func : funcs) {
        double funcTime = measureExecutionTime(text, pattern, func);
        times.push_back(funcTime);
    }
    
    saveResults(fileName, text.length(), funcNames, times);
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