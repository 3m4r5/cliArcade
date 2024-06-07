#include "../../resolutionMenu.cpp"
#include "patternMenu.cpp"
using namespace std;

class gameOfLife{
public:
    gameOfLife(){
        init();
        update();
        while (terminal::uGetKey() != ' '){
            terminal::print();
            update();
            terminal::wait(0.01);
        }
        terminal::exit();
    }
private:
    vector<int> birth, survive;
    int height, width;
    vector<vector<bool>> grid;
    void init(){
        pattern p = patternMenu().getPattern();
        grid = p.grid, birth = p.birth, survive = p.survive;
        resolutionMenu().setResolution();
    }
    void update(){
        auto tmp = grid;
        terminal::rawBuffer = grid;
        int c, x, y;
        for (int i = 0, height = grid.size(); i < height; i++){
            for (int j = 0, width = grid[0].size(); j < width; j++){
                c = -tmp[i][j];
                for (int dy = -1; dy < 2; dy++){
                    for (int dx = -1; dx < 2; dx++){
                        y = i + dy, x = j + dx;
                        if (y < 0 || x < 0 || y == height || x == width) continue;
                        c += tmp[y][x];
                    }
                }
                grid[i][j] = grid[i][j]
                ? find(survive.begin(), survive.end(), c) != survive.end()
                : find(birth.begin(), birth.end(), c) != birth.end();
            }
        }
        for (int i = 0, width = grid[0].size(); i < width; i++){
           if (grid[0][i] || grid[1][i]){
                grid.emplace(grid.begin(), vector<bool>(grid[0].size()));
                break;
            }
        }
        for (int i = 0, height = grid.size(); i < height ; i++){
            if (grid[i][0] || grid[i][1]){
                for (auto &j: grid) j.emplace(j.begin(), 0);
                break;
            }
        }
        for (int i = 0, width = grid[0].size(), last = grid.size() - 1; i < width; i++){
            if (grid[last][i] || grid[last - 1][i]){
                grid.emplace_back(vector<bool>(grid[0].size()));
                break;
            }
        }
        for (int i = 0, height = grid.size(), last = grid[0].size() - 1; i < height ; i++){
            if (grid[i][last] || grid[i][last - 1]){
                for (auto &j: grid) j.emplace_back(0);
                break;
            }
        }
    }
};