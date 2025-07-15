#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <unordered_set>
#include <cstdlib>

using namespace std;


// --- HELPER FUNCS DECLARATION ---
string readFromFileAndSearch(const string&);
void printIndexes(const vector<int>&);
double measureExecutionTime(const string&, const string&, vector<int> (*)(const string&, const string&));
double measureExecutionTime(const string&, const string&, vector<int> (*)(const string&, const string&, const vector<int>&), const vector<int>&);
vector<double> measureAlgorithms(const string&, const string&);
void testInLengthes(const string&, const string&, const vector<int>&, const string&);
void testAlgo(const string&, const string&, vector<int> (*searchFunc)(const string&, const string&));
void saveResults(const string&, int, const vector<double>&);


// --- ALGORITHMS ---
vector<int> bruteForceSearch(const string& text, const string& pattern) {
    vector<int> indices;
    int textLen = text.length();
    int patternLen = pattern.length();

    // outer loop
    for (int i = 0; i <= textLen - patternLen; ++i) {
        int j = 0;

        // inner loop
        while (j < patternLen && text[i + j] == pattern[j]) {
            ++j;
        }
        if (j == patternLen) {
            indices.push_back(i);
        }
    }

    return indices;
}

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


// vector<vector<int>> buildFSM(const string &pattern) {
//     int m = pattern.length();

//     unordered_set<char> alphabet(pattern.begin(), pattern.end());
//     vector<vector<int>> fsm(m + 1, vector<int>(256, 0)); // 256 for all ASCII chars
    
//     for (int state = 0; state <= m; ++state) {
//         for (char ch : alphabet) {
//             int k = min(m, state + 1);
//             string prefix = pattern.substr(0, state) + ch;
//             while (k > 0 && pattern.substr(0, k) != prefix.substr(prefix.length() - k)) {
//                 --k;
//             }
//             fsm[state][(int)ch] = k;
//         }
//     }
//     return fsm;
// }

vector<vector<int>> buildFSM(const string &pattern) {
    int m = pattern.length();
    vector<vector<int>> fsm(m + 1, vector<int>(256, 0));
    
    // Initialize first row
    fsm[0][(int)pattern[0]] = 1;
    int X = 0;  // Longest prefix which is also a suffix

    for (int i = 1; i < m; ++i) {
        for (int c = 0; c < 256; ++c)
            fsm[i][c] = fsm[X][c];
        
        fsm[i][(int)pattern[i]] = i + 1;
        X = fsm[X][(int)pattern[i]];
    }

    // Final state (m) is left as all zeros (no transitions from end state)
    return fsm;
}

vector<int> fsmMatch(const string &text, const string &pattern) {
    vector<vector<int>> fsm = buildFSM(pattern);
    vector<int> indices;
    
    int state = 0;
    int m = pattern.length();

    for (int i = 0; i < text.length(); ++i) {
        state = fsm[state][(int)text[i]];
        if (state == m) {
            // found longest suffix
            indices.push_back(i - m + 1);
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
    
    // reading text from file 
    string filePath = "res/text0.txt";
    string text = readFromFileAndSearch(filePath);
    string pattern;

    
    // // test algos for correctness
    // cout << "Enter the pattern: ";
    // getline(cin, pattern);
    // testAlgo(text, pattern, gusfieldZMatch);
    // return 0;


    // increasing size of text by duplicating    
    for (int i = 0; i < 5; i++)
        text += text;
    vector<int> lengths = {1000, 5000, 10000, 20000, 30000};
    
    /*
    text: "abababcabababcabababcabababcabababc";
    patterns: abababc abababcabababcabababcabababcabababc
    */
    
    string cmd = "python plotTaskA.py";

    // pattern = "abababc";
    pattern = "finite";
    cout << endl;
    cout << "\n===================";
    cout << "\n|| SMALL PATTERN ||";
    cout << "\n===================";
    testInLengthes(text, pattern, lengths, "results/A/small.csv");
    system((cmd + " results/A/small.csv").c_str());
    
    // pattern = "abababcabababcabababcabababcabababc";
    pattern = "An automaton (automata in plural) is an abstract self-propelled computing device";
    cout << endl;
    cout << "\n===================";
    cout << "\n|| LARGE PATTERN ||";
    cout << "\n===================";
    testInLengthes(text, pattern, lengths, "results/A/large.csv");
    system((cmd + " results/A/large.csv").c_str());

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

// overload
double measureExecutionTime(const string& text, 
                            const string& pattern, 
                            vector<int> (*searchFunc)(const string&, const string&, const vector<int>&),
                            const vector<int>& table)
{
    auto start = chrono::high_resolution_clock::now();
    vector<int> result = searchFunc(text, pattern, table);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, micro> duration = end - start;
    // chrono::duration<double, milli> duration = end - start;
    
    // printIndexes(result);
    return duration.count();
}

// TODO: ADD ALGOS 
vector<double> measureAlgorithms(const string& text, const string& pattern) {
    // vector<int> lps = computeLPS(pattern);
    // printIndexes(lps);
    double bruteTime = measureExecutionTime(text, pattern, bruteForceSearch);
    double sundayTime = measureExecutionTime(text, pattern, sundaySearch);
    double kmpTime = measureExecutionTime(text, pattern, KMPMatch);
    double fsmTime = measureExecutionTime(text, pattern, fsmMatch);
    double rabinTime = measureExecutionTime(text, pattern, rabinKarpMatch);
    double zTime = measureExecutionTime(text, pattern, gusfieldZMatch);

    cout << "Brute Force: " << bruteTime << endl;
    cout << "Sunday Search: " << sundayTime << endl;
    cout << "KMP Search: " << kmpTime << endl;
    cout << "FSM Match: " << fsmTime << endl;
    cout << "Rabin-Karp Match: " << rabinTime << endl;
    cout << "GusfieldZ Match: " << zTime << endl;

    return {bruteTime, sundayTime, kmpTime, fsmTime, rabinTime, zTime};
}

void testInLengthes(const string& text, 
                    const string& pattern, 
                    const vector<int>& lengths, 
                    const string& fileName) 
{
    if (ifstream(fileName)) {
        ofstream clearFile(fileName, ios::trunc); // clearing file content
        clearFile.close();
    }
    for (int len : lengths) {
        if (len > text.size()) {
            cout << "[!] Skipping " << len << ": bigger.\n";
            continue;
        }
        cout << "\n---- Text length: " << len << " ----\n";
        vector<double> times = measureAlgorithms(text.substr(0, len), pattern);
        
        saveResults(fileName, len, times);
    }
}


// --- TESTER ---
void testAlgo(const string& text, const string& pattern, 
                            vector<int> (*searchFunc)(const string&, const string&)) {
    vector<int> result = searchFunc(text, pattern);
    vector<int> correct = bruteForceSearch(text, pattern);
    printIndexes(result);
    cout << endl;
    printIndexes(correct);
}

// --- SAVE TO CSV ---
void saveResults(const string& filename, int textLength, const vector<double>& times) {
    ofstream file(filename, ios::app); // append mode

    if (file.tellp() == 0) {
        // writing CSV header
        file << "TextLength,BruteForce,Sunday,KMP,FSM,RabinKarp,GusfieldZ\n";
    }

    file << textLength;
    for (double time : times) {
        file << "," << time;
    }
    file << "\n";
    file.close();
}