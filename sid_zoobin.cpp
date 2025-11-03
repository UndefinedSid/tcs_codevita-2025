#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int edgeCount;
    cin >> edgeCount;

    vector<pair<int, int>> firstGraph(edgeCount), targetGraph(edgeCount);
    set<int> vertexSet;

    for (int i = 0; i < edgeCount; ++i) {
        cin >> firstGraph[i].first >> firstGraph[i].second;
        vertexSet.insert(firstGraph[i].first);
        vertexSet.insert(firstGraph[i].second);
    }

    for (int i = 0; i < edgeCount; ++i)
        cin >> targetGraph[i].first >> targetGraph[i].second;

    vector<int> allVertices(vertexSet.begin(), vertexSet.end());

    auto normalizeEdges = [](vector<pair<int, int>> edges) {
        for (auto& edge : edges)
            if (edge.first > edge.second)
                swap(edge.first, edge.second);
        sort(edges.begin(), edges.end());
        return edges;
    };

    auto encodeEdges = [](const vector<pair<int, int>>& edges) {
        string encoded;
        for (auto [u, v] : edges)
            encoded += to_string(u) + "-" + to_string(v) + ",";
        return encoded;
    };

    vector<pair<int, int>> normalizedTarget = normalizeEdges(targetGraph);
    string targetCode = encodeEdges(normalizedTarget);

    vector<pair<int, int>> normalizedStart = normalizeEdges(firstGraph);
    string startCode = encodeEdges(normalizedStart);

    if (startCode == targetCode) {
        cout << 0;
        return 0;
    }

    map<string, int> visited;
    queue<pair<vector<pair<int, int>>, int>> bfsQueue;

    bfsQueue.push({normalizedStart, 0});
    visited[startCode] = 0;

    while (!bfsQueue.empty()) {
        auto [currentEdges, steps] = bfsQueue.front();
        bfsQueue.pop();

        map<int, vector<int>> adjacency;
        for (auto [u, v] : currentEdges) {
            adjacency[u].push_back(v);
            adjacency[v].push_back(u);
        }

        set<vector<int>> foundCycles;

        for (int start : allVertices) {
            function<void(int, int, vector<int>&, set<int>&)> dfs =
                [&](int node, int parent, vector<int>& path, set<int>& visitedSet) {
                    path.push_back(node);
                    visitedSet.insert(node);

                    for (int neighbor : adjacency[node]) {
                        if (neighbor == parent) continue;

                        if (visitedSet.count(neighbor)) {
                            auto it = find(path.begin(), path.end(), neighbor);
                            if (it != path.end()) {
                                vector<int> cycle(it, path.end());
                                if (cycle.size() >= 3) {
                                    int minPos = min_element(cycle.begin(), cycle.end()) - cycle.begin();
                                    rotate(cycle.begin(), cycle.begin() + minPos, cycle.end());
                                    foundCycles.insert(cycle);
                                }
                            }
                        } else if (path.size() < allVertices.size()) {
                            dfs(neighbor, node, path, visitedSet);
                        }
                    }

                    path.pop_back();
                    visitedSet.erase(node);
                };

            vector<int> currentPath;
            set<int> visitedSet;
            dfs(start, -1, currentPath, visitedSet);
        }

        for (const auto& cycle : foundCycles) {
            map<int, int> nextNode;
            for (int v : allVertices)
                nextNode[v] = v;

            for (size_t i = 0; i < cycle.size(); ++i)
                nextNode[cycle[i]] = cycle[(i + 1) % cycle.size()];

            vector<pair<int, int>> transformedEdges;
            for (auto [u, v] : currentEdges)
                transformedEdges.push_back({nextNode[u], nextNode[v]});

            transformedEdges = normalizeEdges(transformedEdges);
            string encodedNext = encodeEdges(transformedEdges);

            if (encodedNext == targetCode) {
                cout << steps + 1;
                return 0;
            }

            if (!visited.count(encodedNext)) {
                visited[encodedNext] = steps + 1;
                bfsQueue.push({transformedEdges, steps + 1});
            }
        }
    }

    cout << -1;
    return 0;
}