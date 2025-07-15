#include<algorithm>
#include<iostream>
#include <chrono>
#include <vector>
#include <fstream>


class Vec {
    int cap;
    int len;
    int *mem;

public:
    Vec(int _len=0): cap(_len), len(_len), mem(new int[cap]){}

    int size() const {
        return len;
    }

    int vol() const {
        return cap;
    }

    int& get(int i) {
        return mem[i];
    }

    const int& get(int i) const {
        return mem[i];
    }

    ~Vec() {
        delete [] mem;
    }

    void resize(int new_cap) {
        cap = std::max(1, new_cap);
        int *new_mem = new int[cap];
        len = std::min(len, cap);
        for (int i = 0 ; i < len ; i++) {
            new_mem[i] = get(i);
        }
        delete [] mem;
        mem = new_mem;
    }

    void pop() {
        len--;
    }

    void remove(int index) {
        if (index < 0 || index >= len) {
            std::cout << "invalid index" << std::endl;
            return;
        }
        for (int i = index; i < len - 1; i++) {
            mem[i] = mem[i + 1];
        }
        len -= 1;
        if (len < cap / 4 && cap > 1) {
            resize(cap / 2);
        }
    }

    void remove_seg(int start, int end) {
        if (start < 0 || end >= len || start > end) {
            std::cout << "invalid index" << std::endl;
            return;
        }
        int seg = end - start + 1;
        for (int i = end + 1; i < len; i++) {
            mem[i - seg] = mem[i];
        }
        len -= seg;
        if (len < cap / 4 && cap > 1) {
            resize(cap / 2);
        }
    }

    void push(int el) {
        if (len == cap) {
            resize(2 * cap);
        }
        get(len) = el;
        len++;
    }

    Vec(const Vec &other):
        cap(other.cap),
        len(other.len),
        mem(new int[len])
    {
        for (int i = 0 ; i < len ; i++) {
            get(i) = other.get(i);
        }
    }

    // Vec &operator=(const Vec &other) {
    //     if (this != &other) {
    //         delete [] mem;
    //         cap = other.cap;
    //         len = other.len;
    //         mem = new int[cap];
    //         for (int i = 0 ; i < len ; i++) {
    //             get(i) = other.get(i);
    //         }
    //     }
    //     return *this;
    // }

    Vec &operator=(Vec cpy) {
        std::swap(cap, cpy.cap);
        std::swap(len, cpy.len);
        std::swap(mem, cpy.mem);

        return *this;
    }
};


int main(){
    const int N = 100000000;
    const int BR = 1000000;

    std::ofstream outfile("results/MyVector_results.csv");
    outfile << "Elements,Time,Reallocation\n";

    Vec my_vec;
    size_t prev_capacity = my_vec.vol();

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < N; i++) {
        my_vec.push(i);
        if (i % BR == 0) {
            auto now = std::chrono::high_resolution_clock::now();
            double long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
            elapsed /= 1000;
            if (my_vec.vol() != prev_capacity) {
                prev_capacity = my_vec.vol();
                outfile << i << "," << elapsed << ", here\n";
            } else {
                outfile << i << "," << elapsed << "\n";
            }
        }
    }

    outfile.close();

    std::ofstream outfile2("results/StdVector_results.csv");
    outfile2 << "Elements,Time,Reallocation\n";

    std::vector<int> std_vec;
    prev_capacity = std_vec.capacity();

    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < N; i++) {
        std_vec.push_back(i);
        if (i % BR == 0) {
            auto now = std::chrono::high_resolution_clock::now();
            double long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
            elapsed /= 1000;
            if (std_vec.capacity() != prev_capacity) {
                prev_capacity = std_vec.capacity();
                outfile2 << i << "," << elapsed << ", here\n";
            } else {
                outfile2 << i << "," << elapsed << "\n";
            }
        }
    }

    outfile2.close();
    return 0;
}