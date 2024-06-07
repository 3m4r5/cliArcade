#include "../../baseMenu.cpp"
#include "fileMenu.cpp"
#include "formatMenu.cpp"

class patternMenu : public baseMenu {
public:
    patternMenu(){
        gridWidth = 2, cardHeight = 5, cardWidth = 16,
        title = L"method of importing pattern:",
        cardsContent = {{L"From a File"}, {L"Manual Entry"}};
        init();
    }
    pattern getPattern(){
        setFinalState();
        return (state ? formatMenu().getPattern(): fileMenu().getPattern());
    }
};