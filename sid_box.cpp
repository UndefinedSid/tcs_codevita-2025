#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int height, width;
    if (!(cin >> height >> width)) return 0;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<int> columnStars(width, 0);
    for (int r = 0; r < height; r++) {
        string rawLine, stripped;
        getline(cin, rawLine);
        for (char ch : rawLine) {
            if (ch != ' ') stripped += ch;
        }
        for (int c = 0; c < width; c++) {
            if (stripped[c] == '*') columnStars[c]++;
        }
    }

    int numOps;
    cin >> numOps;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    while (numOps--) {
        string direction;
        getline(cin, direction);

        vector<int> freq(height + 1, 0);
        for (int cnt : columnStars) freq[cnt]++;

        vector<int> suffix(height + 2, 0);
        for (int idx = height - 1; idx >= 0; idx--) {
            suffix[idx] = suffix[idx + 1] + freq[idx + 1];
        }

        int newHeight = width, newWidth = height;
        vector<int> rotatedStars(newWidth, 0);

        if (direction == "right") {
            for (int c = 0; c < newWidth; c++) {
                rotatedStars[c] = suffix[c];
            }
        } else { 
            for (int c = 0; c < newWidth; c++) {
                rotatedStars[c] = suffix[height - 1 - c];
            }
        }

        height = newHeight;
        width = newWidth;
        columnStars.swap(rotatedStars);
    }

    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            char cellChar = (r >= height - columnStars[c]) ? '*' : '.';
            if (c) cout << ' ';
            cout << cellChar;
        }
        if (r + 1 < height) cout << '\n';
    }

    return 0;
}