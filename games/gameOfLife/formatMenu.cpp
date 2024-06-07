#include "../../baseMenu.cpp"
#include "decode.cpp"
using namespace std;

class formatMenu : public baseMenu {
public:
    formatMenu(){
        cardHeight = 5,
        cardWidth = 13,
        gridWidth = 2,
        title = L"Choose a format:",
        cardsContent = {{L"rle"},  {L"cells"}};
        init();
    }
    pattern getPattern(){
        setFinalState();
        string content = "", line;
        terminal::clear();
        terminal::cursorShow();
        wcout << "Enter pattern code line by line, leave an empty line to finish:\n";
        do {
            getline(cin, line);
            content += '\n' + line;
        } while (line != "");
        content = content.substr(1);
        terminal::cursorHide();
        pattern p;
        switch (state){
            case 0: p = decode::rle(content); break;
            case 1: p = decode::cells(content); break;
        }
        return p;
    }
};