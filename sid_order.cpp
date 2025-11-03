#include <bits/stdc++.h>
using namespace std;

static int find_min_moves(const vector<int>& currentOrder) {
    int n = (int)currentOrder.size();

    string startState(n, '\0'), goalState(n, '\0');
    for (int i = 0; i < n; ++i) startState[i] = char(currentOrder[i]);
    for (int i = 0; i < n; ++i) goalState[i] = char(i);

    if (startState == goalState) return 0;

    vector<array<int, 3>> moves;
    moves.reserve(n * n * n);
    for (int cutStart = 0; cutStart < n; ++cutStart) {
        for (int cutEnd = cutStart + 1; cutEnd <= n; ++cutEnd) {
            int blockLen = cutEnd - cutStart;
            for (int pastePos = 0; pastePos <= n - blockLen; ++pastePos) {
                if (pastePos == cutStart) continue;  
                moves.push_back({cutStart, cutEnd, pastePos});
            }
        }
    }

    deque<string> queueA, queueB;
    unordered_map<string, int> distA, distB;

    queueA.push_back(startState);
    distA[startState] = 0;

    queueB.push_back(goalState);
    distB[goalState] = 0;

    auto expandFrontier = [&](deque<string>& q,
                              unordered_map<string, int>& distSelf,
                              unordered_map<string, int>& distOther) -> int {
        int currentLevelSize = (int)q.size();
        while (currentLevelSize--) {
            string current = q.front();
            q.pop_front();

            int currentDist = distSelf[current];

            for (auto& move : moves) {
                int i = move[0], j = move[1], k = move[2];
                int len = j - i;

                string block = current.substr(i, len);
                string remaining = current.substr(0, i) + current.substr(j);
                string nextState = remaining.substr(0, k) + block + remaining.substr(k);

                if (distSelf.find(nextState) != distSelf.end()) continue;

                int newDist = currentDist + 1;
                auto foundInOther = distOther.find(nextState);
                if (foundInOther != distOther.end())
                    return newDist + foundInOther->second;  

                distSelf[nextState] = newDist;
                q.push_back(nextState);
            }
        }
        return -1;
    };

    while (!queueA.empty() && !queueB.empty()) {
        int answer = -1;
        if (queueA.size() <= queueB.size()) {
            answer = expandFrontier(queueA, distA, distB);
        } else {
            answer = expandFrontier(queueB, distB, distA);
        }
        if (answer != -1) return answer;
    }

    return 0;  
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    string line;
    getline(cin, line);          
    getline(cin, line);          

    vector<string> shuffledList(n), originalList(n);
    for (int i = 0; i < n; ++i)
        getline(cin, shuffledList[i]);

    getline(cin, line);          
    for (int i = 0; i < n; ++i)
        getline(cin, originalList[i]);

    unordered_map<string, int> positionInOriginal;
    positionInOriginal.reserve(n * 2);
    for (int i = 0; i < n; ++i)
        positionInOriginal[originalList[i]] = i;

    vector<int> orderIndices(n);
    for (int i = 0; i < n; ++i)
        orderIndices[i] = positionInOriginal[shuffledList[i]];

    cout << find_min_moves(orderIndices) << "\n";
    return 0;
}