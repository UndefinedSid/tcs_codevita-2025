#include <bits/stdc++.h>
using namespace std;

class ExpressionEvaluator {
public:
    void execute() {
        vector<string> digitGrid(3), operGrid(3), exprGrid(3);

        for (int i = 0; i < 3; i++) getline(cin, digitGrid[i]);
        for (int i = 0; i < 3; i++) getline(cin, operGrid[i]);
        for (int i = 0; i < 3; i++) getline(cin, exprGrid[i]);

        map<string, string> patternToBinary;
        map<string, char> binaryToDigit;
        map<string, char> patternToOperator;

        for (int i = 0; i < 10; i++) {
            string pattern = extractPattern(digitGrid, i);
            string binary = convertToBinary(pattern);
            patternToBinary[pattern] = binary;
            binaryToDigit[binary] = '0' + i;
        }

        string operators = "|&!()";
        for (int i = 0; i < 5; i++) {
            string pattern = extractPattern(operGrid, i);
            patternToOperator[pattern] = operators[i];
        }

        vector<string> tokenList;
        string currentBinary;
        size_t totalChunks = exprGrid[0].size() / 3;

        for (size_t i = 0; i < totalChunks; i++) {
            string pattern = extractPattern(exprGrid, i);
            if (patternToBinary.count(pattern)) {
                currentBinary += patternToBinary[pattern];
            } else if (patternToOperator.count(pattern)) {
                if (!currentBinary.empty()) {
                    tokenList.push_back(currentBinary);
                    currentBinary.clear();
                }
                tokenList.push_back(string(1, patternToOperator[pattern]));
            }
        }
        if (!currentBinary.empty()) {
            tokenList.push_back(currentBinary);
        }

        stack<string> values;
        stack<char> ops;

        for (const string &token : tokenList) {
            if (token == "(") {
                ops.push('(');
            } else if (token == ")") {
                while (!ops.empty() && ops.top() != '(') {
                    computeTop(values, ops);
                }
                if (!ops.empty()) ops.pop();
            } else if (isOperator(token)) {
                char oper = token[0];
                while (!ops.empty() && ops.top() != '(' && precedence(ops.top()) >= precedence(oper)) {
                    computeTop(values, ops);
                }
                ops.push(oper);
            } else {
                values.push(token);
            }
        }

        while (!ops.empty()) computeTop(values, ops);
        string resultBinary = values.empty() ? "" : values.top();
        string result = decodeResult(resultBinary, binaryToDigit);

        cout << (result.empty() ? "0" : result);
    }

private:
    string extractPattern(const vector<string> &grid, size_t idx) {
        size_t col = idx * 3;
        return grid[0].substr(col, 3) + grid[1].substr(col, 3) + grid[2].substr(col, 3);
    }

    string convertToBinary(const string &pattern) {
        string bin;
        for (char ch : pattern) {
            bin += (ch == ' ') ? '0' : '1';
        }
        return bin;
    }

    string pad(const string &str, size_t len) {
        return (str.size() >= len) ? str : string(len - str.size(), '0') + str;
    }

    string binOr(const string &a, const string &b) {
        size_t len = max(a.size(), b.size());
        string aPadded = pad(a, len), bPadded = pad(b, len), result;
        for (size_t i = 0; i < len; i++) {
            result += (aPadded[i] == '1' || bPadded[i] == '1') ? '1' : '0';
        }
        return result;
    }

    string binAnd(const string &a, const string &b) {
        size_t len = max(a.size(), b.size());
        string aPadded = pad(a, len), bPadded = pad(b, len), result;
        for (size_t i = 0; i < len; i++) {
            result += (aPadded[i] == '1' && bPadded[i] == '1') ? '1' : '0';
        }
        return result;
    }

    string binNot(const string &a) {
        string result;
        for (char c : a) {
            result += (c == '1') ? '0' : '1';
        }
        return result;
    }

    int precedence(char oper) {
        if (oper == '!') return 3;
        if (oper == '|') return 2;
        if (oper == '&') return 1;
        return 0;
    }

    void computeTop(stack<string> &values, stack<char> &ops) {
        char oper = ops.top();
        ops.pop();
        if (oper == '!') {
            string a = values.top();
            values.pop();
            values.push(binNot(a));
        } else {
            string b = values.top(); values.pop();
            string a = values.top(); values.pop();
            values.push((oper == '|') ? binOr(a, b) : binAnd(a, b));
        }
    }

    bool isOperator(const string &token) {
        return token == "|" || token == "&" || token == "!";
    }

    string decodeResult(string bin, const map<string, char> &binToDigit) {
        if (bin.empty()) return "";
        if (bin.size() % 9 != 0)
            bin = pad(bin, bin.size() + (9 - bin.size() % 9));

        string decoded;
        for (size_t i = 0; i < bin.size(); i += 9) {
            string chunk = bin.substr(i, 9);
            if (binToDigit.count(chunk)) {
                decoded += binToDigit.at(chunk);
            }
        }
        return decoded;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ExpressionEvaluator evaluator;
    evaluator.execute();
    return 0;
}