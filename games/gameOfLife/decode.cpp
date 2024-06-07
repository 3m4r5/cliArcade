#pragma once
#include "../../terminal.cpp"
using namespace std;
#define padding 3

struct pattern {
    vector<vector<bool>> grid;
    vector<int> birth = {3};
    vector<int> survive = {2, 3};
};

class decode {
public:
    static pattern cells(string patternCode){
        return text(patternCode, 'O', '.', '!');
    }
    static pattern rle(string patternCode) {
        while (patternCode[0] == '#') patternCode = patternCode.substr(patternCode.find('\n') + 1);
        pattern p;
        int x = 0, y = 0;
        size_t pos = patternCode.find("\n");
        if (pos == string::npos) {
            wcout << "Invalid RLE: Missing header";
            terminal::exit();
        }
        string header = patternCode.substr(0, pos++);
        size_t xPos = header.find("x = "),
        yPos = header.find("y = "),
        rulePos = header.find("rule = ");
        if (xPos == string::npos || yPos == string::npos) {
            wcout << "Invalid RLE: Missing dimensions";
            terminal::exit();
        }
        x = stoi(header.substr(xPos + 4));
        y = stoi(header.substr(yPos + 4, header.find(',', yPos) - yPos - 4));
        p.grid.resize(y, vector<bool>(x, 0));
        if (rulePos != string::npos) {
            p.birth = {}, p.survive = {};
            string ruleStr = header.substr(rulePos + 7);
            size_t slashPos = ruleStr.find('/');
            string birthStr = ruleStr.substr(0, slashPos);
            string surviveStr = ruleStr.substr(slashPos + 1);
            if (!isdigit(birthStr[0])) birthStr = birthStr.substr(1);
            if (!isdigit(surviveStr[0])) surviveStr = surviveStr.substr(1);
            else swap(birthStr, surviveStr);
            for (char c: birthStr) p.birth.emplace_back(c - 48);
            for (char c: surviveStr) if (isdigit(c)) p.survive.emplace_back(c - 48);
        }
        int currentRow = 0,
        currentCol = 0,
        count = 0;
        char c;
        for (; pos < patternCode.size() && patternCode[pos] != '!'; pos++) {
            c = patternCode[pos];
            if (isdigit(c)) count = count * 10 + (c - 48);
            else {
                if (count == 0) count = 1;
                switch (c) {
                    case 'b': for (int i = 0; i < count; i++) p.grid[currentRow][currentCol++] = 0; break;
                    case 'o': for (int i = 0; i < count; i++) p.grid[currentRow][currentCol++] = 1; break;
                    case '$': currentRow += count, currentCol = 0; break;
                }
                count = 0;
            }
        }
        pad(p, padding);
        return p;
    }
private:
    static pattern text(string patternCode, char alive, const char dead, char comment){
        while (patternCode[0] == comment) patternCode = patternCode.substr(patternCode.find('\n') + 1);
        int width = patternCode.find('\n') + 1;
        pattern p;
        p.grid.emplace_back(vector<bool>());
        for (char c: patternCode){
            if (c == dead) p.grid.back().emplace_back(0);
            else if (c == alive) p.grid.back().emplace_back(1);
            else if (c == '\n'){
                p.grid.emplace_back(vector<bool>());
            }
        }
        pad(p, padding);
        return p;
    }
    static void pad(pattern &p, int pad){
        int maxWidth = 0;
        for (auto i: p.grid) maxWidth = max(maxWidth, int(i.size()));
        for (int j = 0; j < pad; j++) p.grid.emplace(p.grid.begin(), vector<bool>(maxWidth + 2 * pad, 0));
        for (auto &i: p.grid) {
            for (int j = 0; j < pad; j++) i.emplace(i.begin(), 0);
            i.resize(maxWidth + 2 * pad, 0);
        }
        p.grid.resize(p.grid.size() + 2 * pad, vector<bool>(maxWidth + 2 * pad));
    }
};
