#pragma once
#include "terminal.cpp"
#include <math.h>
using namespace std;

class baseMenu{
protected:
    int titleHeight = 1,
    state = 0,
    cardWidth = 2, cardHeight = 2,
    gridWidth = 1,
    hMargin = 1, vMargin = 1,
    hPad = 1, vPad = 1;
    wstring title;
    vector<vector<wstring>> cardsContent;
    vector<wstring> emptyMenu;
    void init(){
        createCards();
        int menuWidth = (cardWidth + hMargin) * gridWidth + hMargin;
        emptyMenu.emplace_back(pad(title, menuWidth));
        for (int i = 1; i < vMargin; i++) emptyMenu.emplace_back(repeat(L" ", menuWidth));
        for (int i = 0, len = ceil(1.0 * cards.size() / gridWidth); i < len; i++){
            for (auto j: row(i)) emptyMenu.emplace_back(j);
            for (int i = 0; i < vMargin; i++) emptyMenu.emplace_back(repeat(L" ", menuWidth));
        }
    }
    void setFinalState(){
        while (1){
            state = max(0, min(state, int(cardsContent.size() - 1)));
            draw();
            switch (terminal::getKey()){
                case 'A': state -= gridWidth; break;
                case 'B': state += gridWidth; break;
                case 'C': state++;            break;
                case 'D': state--;            break;
                case ' ':
                case '\r': return;
                case 'x':
                case 'c': terminal::exit();
            }
        }
    }
private:
    vector<vector<wstring>> cards;
    wstring repeat(wstring s, int times){
        wstring r;
        while (times--) r += s;
        return r;
    }
    wstring pad(wstring s, int width){
        int padding = width - s.size();
        return repeat(L" ", padding / 2) + s + repeat(L" ", ceil(padding / 2.0));
    }
    void draw(){
        terminal::printBuffer = emptyMenu;
        int left = hMargin + (state % gridWidth) * (cardWidth + hMargin), top = titleHeight + (state / gridWidth) * (cardHeight + vMargin);
        terminal::highlightArea(left, left + cardWidth - 1, top, top + cardHeight - 1);
        terminal::print();
    }
    void createCards(){
        for (int j = 0, len = cardsContent.size(); j < len; j++){
            vector<wstring> card;
            card.emplace_back(L"╭" + repeat(L"─", cardWidth - 2) + L"╮");
            for (int i = 0; i < vPad; i++) card.emplace_back(L"│" + repeat(L" ", cardWidth - 2) + L"│");
            for (auto i: cardsContent[j])card.emplace_back(L"│" + pad(i, cardWidth - 2) + L"│");
            for (int i = 0; i < vPad; i++) card.emplace_back(L"│" + repeat(L" ", cardWidth - 2) + L"│");
            card.emplace_back(L"╰" + repeat(L"─", cardWidth - 2) + L"╯");
            cards.emplace_back(card);
        }
    }
    vector<wstring> row(int i){
        vector<wstring> cardRow;
        int firstCard = i * gridWidth, lastCard = min(firstCard + gridWidth, int(cardsContent.size()));
        for (int i = 0; i < cardHeight; i++){
            cardRow.emplace_back(repeat(L" ", hMargin));
            for (int j = firstCard; j < lastCard; j++){
                cardRow.back() += cards[j][i] + repeat(L" ", hMargin);
            }
        }
        return cardRow;
    }
};