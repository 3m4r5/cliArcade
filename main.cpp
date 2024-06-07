#include "mainMenu.cpp"
#include "games.h"
int main() {
    terminal::init();
    switch (mainMenu().getGame()){
        case LIFE: gameOfLife();
        case SNAKE: snake();
    }
}