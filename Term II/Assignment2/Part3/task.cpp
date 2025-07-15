#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <unordered_set>
#include <fstream>
#include <random>
#include <algorithm>

using namespace std;
using namespace std::chrono;

const int MAX_PROBES = 50;
const int TABLE_SIZE = 300;
const vector<double> LOAD_FACTORS = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};

// random keys
vector<int> generate_keys(int count, int max_value) {
    unordered_set<int> unique_keys;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, max_value);

    while (unique_keys.size() < count) {
        unique_keys.insert(dis(gen));
    }
    return vector<int>(unique_keys.begin(), unique_keys.end());
}



class HashTableChaining {
public:
    vector<list<int>> table;
    int size;

    HashTableChaining(int sz) : size(sz), table(sz) {}

    void insert(int key) {
        int index = key % size;
        for (int k : table[index]) {
            if (k == key) return;
        }
        table[index].push_back(key);
    }

    bool search(int key) {
        int index = key % size;
        for (int k : table[index]) {
            if (k == key) return true;
        }
        return false;
    }
};

// linear
class HashTableLinearProbing {
public:
    vector<int> table;
    int size;

    HashTableLinearProbing(int sz) : size(sz), table(sz, -1) {}

    void insert(int key) {
        int idx = key % size;
        for (int i = 0; i < MAX_PROBES; ++i) {
            int probe = (idx + i) % size;
            if (table[probe] == -1 || table[probe] == key) {
                table[probe] = key;
                return;
            }
        }
    }

    bool search(int key) {
        int idx = key % size;
        for (int i = 0; i < MAX_PROBES; ++i) {
            int probe = (idx + i) % size;
            if (table[probe] == -1) return false;
            if (table[probe] == key) return true;
        }
        return false;
    }
};

// double
class HashTableDoubleHashing {
public:
    vector<int> table;
    int size;

    HashTableDoubleHashing(int sz) : size(sz), table(sz, -1) {}

    int hash1(int key) { return key % size; }
    int hash2(int key) { return 1 + (key % (size - 1)); }

    void insert(int key) {
        int h1 = hash1(key);
        int h2 = hash2(key);
        for (int i = 0; i < MAX_PROBES; ++i) {
            int idx = (h1 + i * h2) % size;
            if (table[idx] == -1 || table[idx] == key) {
                table[idx] = key;
                return;
            }
        }
    }

    bool search(int key) {
        int h1 = hash1(key);
        int h2 = hash2(key);
        for (int i = 0; i < MAX_PROBES; ++i) {
            int idx = (h1 + i * h2) % size;
            if (table[idx] == -1) return false;
            if (table[idx] == key) return true;
        }
        return false;
    }
};

// Benchmarking function
template <typename HashTable>
pair<double, double> benchmark(HashTable& table, const vector<int>& keys) {
    auto start_insert = high_resolution_clock::now();
    for (int key : keys) table.insert(key);
    auto end_insert = high_resolution_clock::now();

    vector<int> search_keys(keys.begin(), keys.begin() + min(50, (int)keys.size()));
    auto start_search = high_resolution_clock::now();
    for (int key : search_keys) table.search(key);
    auto end_search = high_resolution_clock::now();

    double insert_time = duration_cast<microseconds>(end_insert - start_insert).count();
    double search_time = duration_cast<microseconds>(end_search - start_search).count();
    return {insert_time, search_time};
}

void save_results(const string& filename,
                  const vector<double>& alphas,
                  const vector<double>& chaining,
                  const vector<double>& linear,
                  const vector<double>& double_hash) {
    ofstream file(filename);
    file << "LoadFactor,Chaining,LinearProbing,DoubleHashing\n";
    for (size_t i = 0; i < alphas.size(); ++i) {
        file << alphas[i] << "," << chaining[i] << "," << linear[i] << "," << double_hash[i] << "\n";
    }
    file.close();
}

int main() {
    string cmd = "python plot.py";
    vector<double> insert_chain, search_chain;
    vector<double> insert_linear, search_linear;
    vector<double> insert_double, search_double;

    for (double alpha : LOAD_FACTORS) {
        int n = static_cast<int>(TABLE_SIZE * alpha);
        vector<int> keys = generate_keys(n, 10 * TABLE_SIZE);

        HashTableChaining ht_chain(TABLE_SIZE);
        auto [ic, sc] = benchmark(ht_chain, keys);
        insert_chain.push_back(ic);
        search_chain.push_back(sc);

        HashTableLinearProbing ht_linear(TABLE_SIZE);
        auto [il, sl] = benchmark(ht_linear, keys);
        insert_linear.push_back(il);
        search_linear.push_back(sl);

        HashTableDoubleHashing ht_double(TABLE_SIZE);
        auto [id, sd] = benchmark(ht_double, keys);
        insert_double.push_back(id);
        search_double.push_back(sd);
    }

    save_results("results/insert_results.csv", LOAD_FACTORS, insert_chain, insert_linear, insert_double);
    system((cmd + " results/insert_results.csv 'Insertion Time'").c_str());
    save_results("results/search_results.csv", LOAD_FACTORS, search_chain, search_linear, search_double);
    system((cmd + " results/search_results.csv 'Search Time'").c_str());

    cout << "Benchmark complete. Results saved to insert_results.csv and search_results.csv\n";
    return 0;
}
