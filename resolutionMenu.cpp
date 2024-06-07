#pragma once
#include "baseMenu.cpp"

class resolutionMenu : public baseMenu{
public:
    resolutionMenu(){
        cardWidth = 11,
        cardHeight = 5,
        gridWidth = 3,
        hMargin = 2,
        title = L"How many characters should a cell take?",
        cardsContent = {{L"One"}, {L"Two"}, {L"1/2"}, {L"1/4"}, {L"1/6"}, {L"1/8"}};
        init();
    }
    void setResolution(){
        setFinalState();
        switch (state){
            using t = terminal;
            case 0: t::r = one;     break;
            case 1: t::r = two;     break;
            case 2: t::r = half;    break;
            case 3: t::r = quarter; break;
            case 4: t::r = sixth;   break;
            case 5: t::r = eighth;
        }
    }
};