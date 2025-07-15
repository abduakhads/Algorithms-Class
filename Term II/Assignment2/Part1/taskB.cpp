#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <tuple>

using namespace std;

const int INF = numeric_limits<int>::max();
const vector<pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

vector<vector<int>> bfs(const vector<vector<char>>& maze, pair<int, int> exit_pos) {
    int rows = maze.size();
    int cols = maze[0].size();
    vector<vector<int>> distances(rows, vector<int>(cols, -1));
    queue<pair<int, int>> q;

    q.push(exit_pos);
    distances[exit_pos.first][exit_pos.second] = 0;

    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();

        for (const auto& [dx, dy] : directions) {
            int nx = x + dx;
            int ny = y + dy;

            if (nx >= 0 && ny >= 0 && nx < rows && ny < cols &&
                maze[nx][ny] != '#' && distances[nx][ny] == -1) {
                distances[nx][ny] = distances[x][y] + 1;
                q.emplace(nx, ny);
            }
        }
    }

    return distances;
}

int find_fastest_wizard(const vector<vector<char>>& maze,
                        const vector<pair<pair<int, int>, double>>& wizards,
                        pair<int, int> exit_pos) {
    vector<vector<int>> distances = bfs(maze, exit_pos);
    double min_time = INF;
    int winner = -1;

    for (int i = 0; i < wizards.size(); ++i) {
        auto [pos, speed] = wizards[i];
        int dist = distances[pos.first][pos.second];

        if (dist == -1) continue; // cannot reach the exit

        double time = dist / speed;

        if (time < min_time) {
            min_time = time;
            winner = i;
        }
    }

    return winner;
}

int main() {
    vector<vector<char>> maze = {
        {'.', '.', '.', '#', '.', '.'},
        {'#', '#', '.', '#', '.', '#'},
        {'.', '.', '.', '.', '.', '.'},
        {'.', '#', '#', '#', '.', '.'}
    };

    pair<int, int> exit_pos = {2, 5};

    vector<pair<pair<int, int>, double>> wizards = {
        {{0, 0}, 1.0},   // wizzard A
        {{3, 0}, 2.0},   // wizzard B
        {{1, 2}, 1.5}    // wizzard C
    };

    int winner = find_fastest_wizard(maze, wizards, exit_pos);

    if (winner != -1)
        cout << "Wizard " << static_cast<char>('A' + winner) << " wins!" << endl;
    else
        cout << "No wizard can reach the exit." << endl;

    return 0;
}
