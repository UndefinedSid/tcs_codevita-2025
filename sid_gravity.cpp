#include <bits/stdc++.h>
using namespace std;

struct Point {
    int row, col;
    bool operator==(const Point &o) const { return row == o.row && col == o.col; }
};
struct PointHash {
    size_t operator()(Point const &p) const {
        return (uint64_t(uint32_t(p.row)) << 32) ^ uint32_t(p.col);
    }
};
struct SegmentPos {
    int row, col, id;
    bool operator==(const SegmentPos &o) const { return row == o.row && col == o.col && id == o.id; }
};
struct SegmentPosHash {
    size_t operator()(SegmentPos const &p) const {
        uint64_t h = p.row;
        h = (h << 21) ^ p.col;
        h = (h << 21) ^ p.id;
        return size_t(h);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<long long> inputData;
    long long temp;
    while (cin >> temp) inputData.push_back(temp);

    int idx = 0;
    int segmentCount = (int)inputData[idx++];
    vector<array<int,4>> segments(segmentCount);

    for (int k = 0; k < segmentCount; k++) {
        segments[k][0] = (int)inputData[idx++];
        segments[k][1] = (int)inputData[idx++];
        segments[k][2] = (int)inputData[idx++];
        segments[k][3] = (int)inputData[idx++];
    }

    int startRow = (int)inputData[idx++];
    int startCol = (int)inputData[idx++];
    int energy = (int)inputData[idx++];

    unordered_map<Point, vector<int>, PointHash> grid;
    unordered_map<SegmentPos, pair<int,int>, SegmentPosHash> nextCell;

    for (int s = 0; s < segmentCount; s++) {
        int r1 = segments[s][0], c1 = segments[s][1];
        int r2 = segments[s][2], c2 = segments[s][3];
        int rowStep = (r2 > r1) ? 1 : -1;
        int colStep = (c2 > c1) ? 1 : -1;
        int len = abs(r2 - r1);

        if (colStep == -1) {
            for (int k = 0; k < len; k++) {
                int rr = r1 + rowStep * k;
                int cc = c1 - k;
                grid[{rr,cc}].push_back(s);
                nextCell[{rr,cc,s}] = {rr + rowStep, cc - 1};
            }
            grid[{r2,c2}].push_back(s);
        } else {
            for (int k = 0; k < len; k++) {
                int rr = r2 - rowStep * k;
                int cc = c2 - k;
                grid[{rr,cc}].push_back(s);
                nextCell[{rr,cc,s}] = {rr - rowStep, cc - 1};
            }
            grid[{r1,c1}].push_back(s);
        }
    }

    auto drop = [&](int rr, int cc) -> pair<int,int> {
        for (int yy = cc - 1; yy >= 0; yy--) {
            auto it = grid.find({rr, yy});
            if (it != grid.end()) return {rr, yy};
        }
        return {rr, 0};
    };

    int cr = startRow, cc = startCol;
    if (grid.find({cr,cc}) == grid.end()) {
        auto p = drop(cr, cc);
        cr = p.first; cc = p.second;
    }

    while (true) {
        if (cc == 0) break;
        auto it = grid.find({cr,cc});
        if (it == grid.end()) {
            auto p = drop(cr, cc);
            cr = p.first; cc = p.second;
            continue;
        }
        auto &segIds = it->second;
        if (segIds.size() == 1) {
            int sid = segIds[0];
            auto it2 = nextCell.find({cr,cc,sid});
            if (it2 == nextCell.end()) {
                auto p = drop(cr, cc);
                cr = p.first; cc = p.second;
                continue;
            }
            if (energy == 0) break;
            energy--;
            cr = it2->second.first;
            cc = it2->second.second;
        } else {
            long long threshold = 1LL * cr * cc;
            vector<pair<int,pair<int,int>>> paths;
            paths.reserve(segIds.size());
            for (int sid : segIds) {
                auto it3 = nextCell.find({cr,cc,sid});
                if (it3 != nextCell.end()) paths.push_back({sid, it3->second});
            }
            if ((long long)energy <= threshold) {
                if (paths.empty()) {
                    auto p = drop(cr, cc);
                    cr = p.first; cc = p.second;
                    continue;
                }
                break;
            }
            energy -= (int)threshold;
            if (paths.empty()) {
                auto p = drop(cr, cc);
                cr = p.first; cc = p.second;
                continue;
            }
            int bestR = 0, bestC = -1;
            for (auto &p : paths) {
                int rr = p.second.first;
                int yy = p.second.second;
                if (yy > bestC) {
                    bestC = yy;
                    bestR = rr;
                }
            }
            if (energy == 0) break;
            energy--;
            cr = bestR; cc = bestC;
        }
    }

    cout << cr << " " << cc;
    return 0;
}