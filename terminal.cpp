#pragma once
#include <vector>
#include <iostream>
#include <unistd.h>
#include <string>
#include <termios.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#define whiteBgLen 11
using namespace std;
enum resolution{raw, one, two, half, quarter, sixth, eighth};

class terminal{
public:
    terminal() = delete;
    static resolution r;
    static vector<vector<bool>> rawBuffer;
    static vector<wstring> printBuffer;
    static const vector<wstring> emptyBuffer;
    static wstring whiteBg;
    static wstring close;
    static void print(){
        if (r != raw) map();
        clear();
        for(auto i: printBuffer) wcout << i << '\n';
        wcout << endl;
    }
    static void highlightArea(int left, int right, int top, int bottom){
            for (int i = top; i <= bottom; i++){
                printBuffer[i].insert(left, whiteBg);
                printBuffer[i].insert(whiteBgLen + right, close);
            }
    }
    static void wait(double seconds){
        usleep(int(seconds * 1e6));
    }
    // get key without blocking
    static char uGetKey(){
        char c = '\0';
        if (kbHit()) {
            c = getchar();
            if (c == 27) {  // ESC sequence
                getchar();  // skip the [
                c = getchar();
            }
        }
        return c;
    }
    static char getKey(){
        char c = '\0';
        system("stty raw");
        c = getchar();
        system("stty cooked");
        if (c == 27) {  // ESC sequence
            getchar();  // skip the [
            c = getchar();
        }
        return c;
    }
    static void cursorShow(){
        wcout << "\x1b[?25h";
    }
    static void cursorHide(){
        wcout << "\x1b[?25l";
    }
    static void init(){
        locale::global(locale("")); // necessary to output unicode characters
        cursorHide();
    }
    static void clear(){
        //wcout << "\033[2J\033[1;1H";
        system("clear");
    }
    static void exit(){
        cursorShow();
        std::exit(0);
    }
    static int lines(){
        static winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        return w.ws_row;
    }
    static int columns(){
        static winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        return w.ws_col;
    }
private:
    static int height(){
        return rawBuffer.size();
    }
    static int width(){
        return rawBuffer[0].size();
    }
    static int kbHit() {
        termios t1, t2;
        char c, f;
        tcgetattr(STDIN_FILENO, &t1);
        t2 = t1;
        t2.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &t2);
        f = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, f | O_NONBLOCK);
        c = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &t1);
        fcntl(STDIN_FILENO, F_SETFL, f);
        return c == EOF? 0 : (ungetc(c, stdin), 1);
    }
    static void map(){
        switch (r){
            case one    : pixelate();      break;
            case two    : duplicate();     break;
            case half   : halve();         break;
            case quarter: quadripartite(); break;
            case sixth  : hexapartite();   break;
            case eighth : braille();       break;
        }
    }
    static void pad(int minHeight, int minWidth){
        int h = height() + minHeight - height() % minHeight,
        w = width() + minWidth - width() % minWidth;
        for (auto &i: rawBuffer) i.resize(w, 0);
        rawBuffer.resize(h, vector<bool>(w, 0));
    }
    static void pixelate(){
        printBuffer = emptyBuffer;
        for (int i = 0, h = height(); i < h; i++){
            printBuffer.emplace_back(L"");
            for (int j = 0, w = width(); j < w; j++) printBuffer.back() += rawBuffer[i][j]? L'█': L' ';
        }
    }
    static void duplicate(){
        printBuffer = emptyBuffer;
        for (int i = 0, h = height(); i < h; i++){
            printBuffer.emplace_back(L"");
            for (int j = 0, w = width(); j < w; j++) printBuffer.back() += rawBuffer[i][j]? L"██": L"  ";
        }
    }
    static void halve(){
        pad(2, 1);
        printBuffer = emptyBuffer;
        auto lookupArray = L" ▀▄█";
        for (int i = 0, h = height(); i < h; i += 2){
            printBuffer.emplace_back(L"");
            for (int j = 0, w = width(); j < w; j++) printBuffer.back() += lookupArray[rawBuffer[i][j] + rawBuffer[i + 1][j] * 2];
        }
    }
    static void quadripartite() {
        pad(2, 2);
        printBuffer = emptyBuffer;
        auto lookupArray = L" ▘▝▀▖▌▞▛▗▚▐▜▄▙▟█";
        for (int i = 0, h = height(); i < h; i += 2){
            printBuffer.emplace_back(L"");
            for (int j = 0, w = width(); j < w; j += 2)
                printBuffer.back() += lookupArray[
                    rawBuffer[i][j]
                    | rawBuffer[i][j + 1]     << 1
                    | rawBuffer[i + 1][j]     << 2
                    | rawBuffer[i + 1][j + 1] << 3];
        }
    }
    static void hexapartite(){
        pad(3, 2);
        printBuffer = emptyBuffer;
        auto lookupArray = L" 🬀🬁🬂🬃🬄🬅🬆🬇🬈🬉🬊🬋🬌🬍🬎🬏🬐🬑🬒🬓▌🬔🬕🬖🬗🬘🬙🬚🬛🬜🬝🬞🬟🬠🬡🬢🬣🬤🬥🬦🬧▐🬨🬩🬪🬫🬬🬭🬮🬯🬰🬱🬲🬳🬴🬵🬶🬷🬸🬹🬺🬻█";
        for (int i = 0, h = height(); i < h; i += 3) {
            printBuffer.emplace_back(L"");
            for (int j = 0, w = width(); j < w; j += 2)
                printBuffer.back() += lookupArray[
                    rawBuffer[i][j]
                    | rawBuffer[i][j + 1]     << 1
                    | rawBuffer[i + 1][j]     << 2
                    | rawBuffer[i + 1][j + 1] << 3
                    | rawBuffer[i + 2][j]     << 4
                    | rawBuffer[i + 2][j + 1] << 5];
        }
    }
    static void braille(){
        pad(4, 2);
        printBuffer = emptyBuffer;
        for (int i = 0, h = height(); i < h; i += 4){
            printBuffer.emplace_back(L"");
            for (int j = 0, w = width(); j < w; j += 2)
                printBuffer.back() += 0x2800
                | rawBuffer[i + 0][j] << 0 | rawBuffer[i + 0][j + 1] << 3
                | rawBuffer[i + 1][j] << 1 | rawBuffer[i + 1][j + 1] << 4
                | rawBuffer[i + 2][j] << 2 | rawBuffer[i + 2][j + 1] << 5
                | rawBuffer[i + 3][j] << 6 | rawBuffer[i + 3][j + 1] << 7;
        }
    }
};
using t = terminal;
// Definitions
resolution t::r;
vector<vector<bool>> t::rawBuffer;
vector<wstring> t::printBuffer;
const vector<wstring> t::emptyBuffer;
wstring t::whiteBg = L"\033[47m\033[30m";
wstring t::close = L"\033[0m";