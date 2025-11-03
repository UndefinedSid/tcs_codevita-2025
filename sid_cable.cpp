#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int rows, cols;
    cin >> rows >> cols;

    vector<string> circuit(rows);
    for (int r = 0; r < rows; r++) {
        circuit[r].resize(cols);
        for (int c = 0; c < cols; c++) {
            cin >> circuit[r][c];
        }
    }

    vector<int> fullRowRods, fullColRods;
    for (int r = 0; r < rows; r++) {
        if (all_of(circuit[r].begin(), circuit[r].end(), [](char cell){ return cell != '.'; }))
            fullRowRods.push_back(r);
    }
    for (int c = 0; c < cols; c++) {
        bool allFilled = true;
        for (int r = 0; r < rows; r++)
            if (circuit[r][c] == '.') allFilled = false;
        if (allFilled) fullColRods.push_back(c);
    }

    vector<vector<bool>> crossPoint(rows, vector<bool>(cols, false));
    for (int col : fullColRods) {
        for (int r = 0; r < rows; r++) {
            int left = col - 1, right = col + 1;
            if (left >= 0 && right < cols && circuit[r][left] == 'C' && circuit[r][right] == 'C')
                crossPoint[r][col] = true;
        }
    }
    for (int row : fullRowRods) {
        for (int c = 0; c < cols; c++) {
            int up = row - 1, down = row + 1;
            if (up >= 0 && down < rows && circuit[up][c] == 'C' && circuit[down][c] == 'C')
                crossPoint[row][c] = true;
        }
    }

    vector<vector<bool>> isCable(rows, vector<bool>(cols, false));
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            if (circuit[r][c] == 'C' || crossPoint[r][c])
                isCable[r][c] = true;

    vector<vector<int>> graph(rows * cols);
    int dRow[4] = {-1, 0, 1, 0};
    int dCol[4] = {0, 1, 0, -1};

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (!isCable[r][c]) continue;
            int node = r * cols + c;
            for (int d = 0; d < 4; d++) {
                int nr = r + dRow[d], nc = c + dCol[d];
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && isCable[nr][nc])
                    graph[node].push_back(nr * cols + nc);
            }
        }
    }

    int startNode = -1;
    for (int r = 0; r < rows && startNode == -1; r++)
        for (int c = 0; c < cols; c++)
            if (isCable[r][c] && graph[r * cols + c].size() == 1) {
                startNode = r * cols + c;
                break;
            }

    vector<bool> visited(rows * cols, false);
    vector<int> rowSum(rows, 0), colSum(cols, 0);

    int current = startNode, previous = -1;
    visited[current] = true;

    while (true) {
        int currRow = current / cols, currCol = current % cols;
        int nextNode = -1;
        for (int neighbor : graph[current])
            if (neighbor != previous && !visited[neighbor]) {
                nextNode = neighbor;
                break;
            }

        if (crossPoint[currRow][currCol] && previous != -1) {
            int prevRow = previous / cols, prevCol = previous % cols;
            int polarity = (circuit[currRow][currCol] == 'C') ? 1 : -1;

            if (prevRow == currRow)
                colSum[currCol] += ((prevCol < currCol) ? 1 : -1) * polarity;
            else
                rowSum[currRow] += ((prevRow < currRow) ? 1 : -1) * polarity;
        }

        if (nextNode == -1) break;
        previous = current;
        current = nextNode;
        visited[current] = true;
    }

    long long result = 0;
    for (int r : fullRowRods) result += abs(rowSum[r]) / 2;
    for (int c : fullColRods) result += abs(colSum[c]) / 2;

    cout << result;
    return 0;
}