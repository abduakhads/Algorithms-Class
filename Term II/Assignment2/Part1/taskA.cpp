#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <set>
#include <unordered_set>

#define iterations 1
#define warmUp 0

using namespace std;

vector<double> buildTimes;
vector<double> checkTimes;


// --- HELPER FUNCS DECLARATION ---
void printWords(const vector<string>&);
vector<string> prepText(const string&);
void saveResults(const string& filename, const vector<double>& times);

template<typename T>
T measureBuildTime(const string&, T (*buildFunc)(const string&));

template<typename T>
vector<string> measureCheckTime(const T&, const vector<string>&, 
    vector<string> (*checkFunc)(const T&, const vector<string>&));


// --- ALGORITHMS ---
class Naive {
    public:
        static vector<string> loadDict(const string& path) {
            vector<string> dictionary;
            ifstream file(path);
            string word;
            
            while (getline(file, word)) {
                // removing trailing spaces
                while (!word.empty() && isspace(word.back())) {
                    word.pop_back();
                }
                dictionary.push_back(word);
            }
            
            return dictionary;
        }


        static vector<string> spellCheck(const vector<string>& dictionary, 
                                             const vector<string>& words) {
            vector<string> misspelled;
            
            for (const auto& word : words) {

                int i = 0;
                for ( ; i < dictionary.size(); i++) {
                    if (word == dictionary[i])
                        break;
                }

                if (i == dictionary.size())
                    misspelled.push_back(word);
            }
            
            return misspelled;
        }
};


class BBST {
    public:
        static set<string> loadDict(const string& path) {
            ifstream file(path);
            set<string> dict;
            string word;

            while (getline(file, word)) {
                while (!word.empty() && isspace(word.back())) {
                        word.pop_back();
                    }
                dict.insert(word);
            }
            return dict;
        }

        static vector<string> spellCheck(const set<string>& dict, const vector<string>& words) {
            vector<string> misspelled;
            for (const auto& word : words) {
                if (dict.find(word) == dict.end()) {
                    misspelled.push_back(word);
                }
            }
            return misspelled;
        }
};


struct TrieNode {
    bool is_end = false;
    TrieNode* children[128] = {nullptr};
};

class Trie {
    private:
        TrieNode* root;

        void clear(TrieNode* node) {
            if (!node) return;

            for (int i = 0; i < 128; ++i) {
                if (node->children[i]) {
                    clear(node->children[i]);
                }
            }
            delete node;
        }

        int charToIndex(char c) const {
            // if (static_cast<unsigned char>(c) < 128)
            return static_cast<unsigned char>(c);
        }

    public:
        Trie() { root = new TrieNode(); }
        ~Trie() { clear(root); }


        void insert(const string& word) {
            TrieNode* node = root;
            for (char ch : word) {
                int idx = charToIndex(ch); // index from -> ASCII
                
                if (!node->children[idx]) {
                    node->children[idx] = new TrieNode();
                }
                node = node->children[idx];
            }
            node->is_end = true;
        }

        bool search(const string& word) const {
            TrieNode* node = root;
            for (char ch : word) {
                int idx = charToIndex(ch);

                if (!node->children[idx]) return false;
                node = node->children[idx];
            }
            return node->is_end;
        }
        
        static Trie* loadDict(const string& path) {
            ifstream file(path);

            Trie *trie = new Trie();
            string word;
            
            while (getline(file, word)) {
                while (!word.empty() && isspace(word.back())) {
                    word.pop_back();
                }
                trie->insert(word);
            }
            return trie;
        }
        
        static vector<string> spellCheck(const Trie& trie, const vector<string>& words) {
            vector<string> misspelled;

            for (const string& word : words) {
                if (!trie.search(word)) {
                    misspelled.push_back(word);
                }
            }
            return misspelled;
        }
};


class HashMap {
    public:
        static unordered_set<string> loadDict(const string& path) {
            ifstream file(path);
            unordered_set<string> dict;
            string word;

            while (getline(file, word)) {
                while (!word.empty() && isspace(word.back())) {
                    word.pop_back();
                }
                dict.insert(word);
            }
            return dict;
        }

        static vector<string> spellCheck(const unordered_set<string>& dict, const vector<string>& words) {
            vector<string> misspelled;

            for (const string& word : words) {
                if (dict.find(word) == dict.end()) {
                    misspelled.push_back(word);
                }
            }
            return misspelled;
        }
};

int main() {
    string cmd = "python plotTaskA.py";
    const string dictPath = "res/english_words.txt";
    vector<string> misspelled;
    ifstream input_file("res/text2.txt");
    string content((istreambuf_iterator<char>(input_file)),
                    istreambuf_iterator<char>());

    vector<string> textWords = prepText(content);

    cout << "\nNaive: " << endl;
    vector<string> naive = measureBuildTime(dictPath, Naive::loadDict);
    misspelled = measureCheckTime(naive, textWords, Naive::spellCheck);
    cout << misspelled.size() << endl;
    
    cout << "\nBBST: " << endl;
    set<string> bbst = measureBuildTime(dictPath, BBST::loadDict);
    misspelled = measureCheckTime(bbst, textWords, BBST::spellCheck);
    cout << misspelled.size() << endl;
    
    cout << "\nTrie: " << endl;
    Trie* trie = measureBuildTime(dictPath, Trie::loadDict);
    misspelled = measureCheckTime(*trie, textWords, Trie::spellCheck);
    cout << misspelled.size() << endl;
    
    cout << "\nHash Map: " << endl;
    unordered_set<string> hashmap = measureBuildTime(dictPath, HashMap::loadDict);
    misspelled = measureCheckTime(hashmap, textWords, HashMap::spellCheck);
    cout << misspelled.size() << endl;
    

    saveResults("results/build_times.csv", buildTimes);
    system((cmd + " results/build_times.csv").c_str());
    
    saveResults("results/check_times.csv", checkTimes);
    system((cmd + " results/check_times.csv").c_str());
    // printWords(misspelled);
    
    delete trie;
    return 0;
}


// --- HELPER FUNCS ---
vector<string> prepText(const string& text) {
    /* returns vector of words from text in lower and removed ',' and '.' */
    vector<string> words;
    string word;
    for (char c : text) {
        if (isspace(c)) {
            if (!word.empty()) {
                // converts to lower
                transform(word.begin(), word.end(), word.begin(), ::tolower);
                words.push_back(word);
                word.clear();
            }
        // avoids punctuation
        } else if (c != '.' && c != ',') {
            word += c;
        }
    }
    if (!word.empty()) {
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        words.push_back(word);
    }
    return words;
}


template<typename T>
T measureBuildTime(const string& text, T (*buildFunc)(const string&)) 
{    
    double totalDuration = 0.0;
    T result; 

    // Warm-up runs
    for (int i = 0; i < warmUp; ++i) {
        buildFunc(text);
    }

    for (int i = 0; i < iterations; ++i) {
        auto start = chrono::high_resolution_clock::now();
        result = buildFunc(text);
        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double, micro> duration = end - start;
        totalDuration += duration.count();
    }

    buildTimes.push_back(totalDuration / iterations);
    cout << "Build Time: " << totalDuration / iterations << endl;

    return result;
}

template<typename T>
vector<string> measureCheckTime(const T& dict, 
                              const vector<string>& text, 
                              vector<string> (*checkFunc)(const T&, const vector<string>&)) 
{    
    double totalDuration = 0.0;
    vector<string> result; 

    // Warm-up runs
    for (int i = 0; i < warmUp; ++i) {
        checkFunc(dict, text);
    }

    for (int i = 0; i < iterations; ++i) {
        auto start = chrono::high_resolution_clock::now();
        result = checkFunc(dict, text);
        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double, micro> duration = end - start;
        totalDuration += duration.count();
    }

    // printWords(result);
    checkTimes.push_back(totalDuration / iterations);
    cout << "Check Time: " << totalDuration / iterations << endl;

    return result;
}


void printWords(const vector<string>& words) {
    for (auto word : words) {
        cout << word << " ";
    }
    cout << endl;
}

// --- SAVE TO CSV ---
// TODO: adjust
void saveResults(const string& filename, const vector<double>& times) {
    ofstream file(filename);

    if (file.tellp() == 0) {
        file << "Naive,BBST,Trie,HashMap\n";
    }

    file << times[0];
    for (int i = 1; i < times.size(); i++) {
        file << "," << times[i];
    }
    file << "\n";
    file.close();
}