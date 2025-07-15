#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <fstream>

using namespace std;

vector<string> prepText(const string&);


bool canSeatGuests(const vector<string>& guest_list,
                     const vector<pair<string, string>>& dislikes,
                     vector<string>& table1,
                     vector<string>& table2) {
    unordered_map<string, vector<string>> graph;
    
    // build the graph
    for (const auto& pair : dislikes) {
        graph[pair.first].push_back(pair.second);
        graph[pair.second].push_back(pair.first);
    }

    // map (-1 = unvisited)
    unordered_map<string, int> color;
    for (const auto& guest : guest_list) {
        color[guest] = -1;
    }

    for (const auto& guest : guest_list) {
        if (color[guest] == -1) {
            stack<string> stk;
            stk.push(guest);
            color[guest] = 0;

            while (!stk.empty()) {
                string current = stk.top();
                stk.pop();

                for (const string& neighbor : graph[current]) {
                    if (color[neighbor] == -1) {
                        color[neighbor] = 1 - color[current];
                        stk.push(neighbor);
                    } else if (color[neighbor] == color[current]) {
                        // found a conflict
                        return false;
                    }
                }
            }
        }
    }

    // seperation
    for (const auto& guest : guest_list) {
        if (color[guest] == 0) {
            table1.push_back(guest);
        } else {
            table2.push_back(guest);
        }
    }

    return true;
}

int main() {
    vector<string> names = prepText("english_names.txt");
    vector<string> guests = {names[1001], names[2000], names[3000], names[4000]};
    
    vector<pair<string, string>> dislikes = {
        {names[1001], names[3000]},
        {names[2000], names[3000]},
        {names[3000], names[4000]}
    };
    
    vector<string> table1, table2;
    if (canSeatGuests(guests, dislikes, table1, table2)) {
        cout << "Yes, guests can be seated with no conflicts!" << endl;
        cout << "Table 1: ";
        for (const string& name : table1) cout << name << " ";
        cout << endl;
        
        cout << "Table 2: ";
        for (const string& name : table2) cout << name << " ";
        cout << endl;
    } else {
        cout << "No valid seating arrangement is possible." << endl;
    }
    
    // cout << guests.size() << endl;
    return 0;
}

vector<string> prepText(const string& path) {
    ifstream input_file(path);
    string content((istreambuf_iterator<char>(input_file)),
                    istreambuf_iterator<char>());

    vector<string> words;
    string word;

    for (char c : content) {
        if (isspace(c)) {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        // avoid punctuation
        } else if (c != '.' && c != ',') {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }
    return words;
}