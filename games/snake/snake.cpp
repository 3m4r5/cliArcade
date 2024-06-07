#include "../../resolutionMenu.cpp"
enum direction {STOP, LEFT, RIGHT, UP, DOWN};
using namespace std;
struct segment{
    int x, y;
    segment(int x = -1, int y = -1): x(x), y(y){}
    friend bool operator==(segment s1, segment s2){
        return (s1.x == s2.x && s1.y == s2.y);
    }
};
class snake{
private:
    vector<vector<bool>> grid;
    direction dir = STOP;
    int gameOver = 0, score = 0, len = 0, width, height;
    segment fruit, head;
    vector<segment> tail;
    void init(){
        terminal::init();
        resolutionMenu().setResolution();
        width = terminal::columns() - 5,
        height = terminal::lines() - 6;
        switch (terminal::r){
            case two: width /= 2; break;
            case half: height *= 2; break;
            case quarter: width *= 2, height *= 2; break;
            case sixth: width *= 2, height *= 3; break;
            case eighth: width *= 2, height *= 4; break;
        }
        height = min(height, 50);
        width = min(width, 50);
        fruit = {rand() % width, rand() % height},
        head = {width / 2, height / 2},
        tail = vector<segment>(height * width);
        grid.resize(height + 2, vector<bool>(width + 2, 0));
        for (int i = 0; i <= width + 1; i++) grid[0][i] = grid[height + 1][i] = 1;
        for (int i = 0; i <= height + 1; i++) grid[i][0] = grid[i][width + 1] = 1;
    }
    void draw() {
        auto newGrid = grid;
        newGrid[head.y + 1][head.x + 1] = newGrid[fruit.y + 1][fruit.x + 1] = 1;
        for (int i = 0; i < len; i++) newGrid[tail[i].y + 1][tail[i].x + 1] = 1;
        terminal::rawBuffer = newGrid;
        terminal::print();
    }
    void input() {
        switch (terminal::uGetKey()){
            case 'A': if (dir != DOWN) dir = UP; break;
            case 'B': if (dir != UP) dir = DOWN; break;
            case 'C': if (dir != LEFT) dir = RIGHT; break;
            case 'D': if (dir != RIGHT) dir = LEFT; break;
            case ' ':
            case '\n': gameOver = 1;
        }
    }
    void update() {
        auto prev = tail[0];
        tail[0] = head;
        for (int i = 1; i < len; i++) swap(prev, tail[i]);
        switch (dir) {
            case LEFT : head.x--; break;
            case RIGHT: head.x++; break;
            case UP   : head.y--; break;
            case DOWN : head.y++; break;
        }
        if (head.x >= width) head.x = 0;
        else if (head.x < 0) head.x = width - 1;
        if (head.y >= height) head.y = 0;
        else if (head.y < 0) head.y = height - 1;
        for (int i = 0; i < len; i++) if (tail[i] == head) gameOver = 1;
        if (head == fruit) {
            score += 10;
            l: fruit.x = rand() % width;
            fruit.y = rand() % height;
            if (head == fruit) goto l;
            for (auto i: tail) if (i == fruit) goto l;
            len++;
        }
        terminal::wait(0.1);
    }
public:
    snake() {
        init();
        while (!gameOver){
            input();
            update();
            draw();
        }
        wcout << "Game over!\nScore: " << score << '\n';
    }
};