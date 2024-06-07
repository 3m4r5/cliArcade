#include "baseMenu.cpp"

enum gameName{SNAKE, LIFE};

class mainMenu : public baseMenu{
public:
    mainMenu(){
        cardWidth = 21,
        cardHeight = 13,
        gridWidth = 2,
        title = L"Choose a game:",
        cardsContent = {
            {
                L" Cellular Automata",
                L"                 ",
                L"       ▄▀▀▀▄     ",
                L"      █     █    ",
                L"       ▄ ▀ ▄     ",
                L"        ▀█▀      ",
                L"      ███        ",
                L"     ▀   ▀       ",
                L"    ▀▀   ▀▀      ",
            },{
                L"      Snake      ",
                L"                 ",
                L"   ▄█▀▀▀▀▀▀▀█▄   ",
                L"   █    ●    █   ",
                L"   █             ",
                L"   █▄▄▄▄█▀▀▀▀█   ",
                L"             █   ",
                L"  ▀█         █   ",
                L"   ▀█▄▄▄▄▄▄▄█▀   ",
            }
        };
        init();
    }
    gameName getGame(){
        setFinalState();
        switch (state){
            case 0: return LIFE;
            case 1: return SNAKE;
        }
    }
};