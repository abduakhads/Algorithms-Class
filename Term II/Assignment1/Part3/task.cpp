#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
using namespace std;

typedef uint64_t u64;

// for char
u64 val_hash(char v) {
    return static_cast<u64>(v);
}

// for int
u64 val_hash(int v) {
    return static_cast<u64>(v);
}

template<typename T>
bool rabinKarp2D(const vector<vector<T>>& picture, int K) {
    int M = picture.size();  
    int N = picture[0].size();

    // if the KxK block larger, return False
    if (K > M || K > N) return false;

    const u64 BASE_ROW = 257;      // base hashing row
    const u64 BASE_COL = 359;      // base hashing column
    const u64 MASK = (1ULL << 61) - 1; 

    // computes the hash of a KxK block starting at (r, c)
    auto compute_block_hash = [&](int r, int c) -> u64 {
        u64 h = 0;
        for (int i = 0; i < K; ++i) {
            u64 row_hash = 0;
            for (int j = 0; j < K; ++j) {

                // builds hash for the row
                row_hash = (row_hash * BASE_ROW + val_hash(picture[r + i][c + j])) & MASK;
            }
            // combines row hashes into a full 2D block hash
            h = (h * BASE_COL + row_hash) & MASK;
        }
        return h;
    };

    // exacts comparison of two KxK blocks (to confirm matches after hashing)
    auto blocks_equal = [&](int r1, int c1, int r2, int c2) -> bool {
        for (int i = 0; i < K; ++i)
            for (int j = 0; j < K; ++j)
                if (picture[r1 + i][c1 + j] != picture[r2 + i][c2 + j])
                    return false;
        return true;
    };

    // coordinates of the top-right KxK corner
    int tr_row = 0, tr_col = N - K;
    u64 target_hash = compute_block_hash(tr_row, tr_col); // Hash of top-right block

    // scans every possible KxK block in the picture
    for (int i = 0; i <= M - K; ++i) {
        for (int j = 0; j <= N - K; ++j) {
            // skips comparing the top-right block with itself
            if (i == tr_row && j == tr_col) continue;
            
            // hashes current KxK block
            u64 block_hash = compute_block_hash(i, j);

            // ff hashes match, do exact comparison to confirm
            if (block_hash == target_hash && blocks_equal(i, j, tr_row, tr_col))
                return true;
        }
    }
    return false;
}


void test_algorithm() {

    // false
    vector<vector<char>> picture1 = {
        {'A', 'B', 'C', 'D'},
        {'E', 'F', 'G', 'H'},
        {'A', 'B', 'I', 'J'},
        {'E', 'F', 'K', 'L'}
    };

    // false
    vector<vector<char>> picture2 = {
        {'A', 'B', 'C'},
        {'D', 'E', 'F'},
        {'G', 'H', 'I'}
    };

    // true
    vector<vector<char>> picture3 = {
        {'X', 'Y', 'A', 'B'},
        {'Z', 'W', 'C', 'D'},
        {'A', 'B', 'M', 'N'},
        {'C', 'D', 'O', 'P'}
    };
    
    // true
    vector<vector<int>> picture4 = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 0, 3, 4},
        {5, 3, 7, 8}
    };


    cout << "Test 1:" << endl;
    bool result1 = rabinKarp2D(picture1, 2);
    cout << "Matches found: " << (result1 ? "True" : "False") << endl;

    cout << "\nTest 2:" << endl;
    bool result2 = rabinKarp2D(picture2, 2);
    cout << "Matches found: " << (result2 ? "True" : "False") << endl;

    cout << "\nTest 3:" << endl;
    bool result3 = rabinKarp2D(picture3, 2);
    cout << "Matches found: " << (result3 ? "True" : "False") << endl;

    cout << "\nTest 4:" << endl;
    bool result4 = rabinKarp2D(picture4, 2);
    cout << "Matches found: " << (result4 ? "True" : "False") << endl;
}

// Main function
int main() {
    test_algorithm();
    return 0;
}
