#include "../../baseMenu.cpp"
#include "decode.cpp"
#include <algorithm>
#include <dirent.h>
#include <fstream>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

class fileMenu : public baseMenu {
public:
    vector<string> filePaths;
    fileMenu(){
        int maxLen = 0;
        addFiles(".", 5);
        if (!filePaths.size()){
            char c = '\0';
            terminal::clear();
            terminal::cursorShow();
            wcout << "No supported pattern files detected in the current directory!\
            \nDo want to explore some pattern files [y/n]? ";
            cin >> c;
            if (tolower(c) == 'y') system("xdg-open https://copy.sh/life/examples");
            terminal::exit();
        }
        for (string name: filePaths) {
            wstring wName(name.begin(), name.end());
            vector<wstring> tmp;
            tmp.emplace_back(wName.substr(wName.rfind('/') + 1));
            maxLen = max(maxLen, int(tmp.back().size()));
            cardsContent.emplace_back(tmp);
        }
        cardWidth = maxLen + 4,
        cardHeight = 5,
        gridWidth = max(2, min(3, 40 / maxLen)),
        title = L"Choose a file:";
        init();
    }
    pattern getPattern(){
        setFinalState();
        string fileName = filePaths[state], line , content = "",
        fileExtension = fileName.substr(fileName.rfind('.') + 1);
        ifstream file(fileName);
        pattern p;
        while (getline(file, line)) content += '\n' + line;
        content = content.substr(1);
        if (fileExtension == "rle") p = decode::rle(content);
        else if (fileExtension == "cells") p = decode::cells(content);
        return p;
    };
private:
    void addFiles(string path, int depth){
        if (!depth) return;
        string extensions[] = {"rle", "cells"};
        DIR *dir;
        dirent *entry;
        dir = opendir(path.c_str());
        while ((entry = readdir(dir)) != nullptr) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
            string fullPath = path + "/" + entry->d_name;
            struct stat entryInfo;
            if (stat(fullPath.c_str(), &entryInfo) == 0 && S_ISDIR(entryInfo.st_mode)) addFiles(fullPath, depth - 1);
            else {
                int dotIndex = fullPath.rfind('.');
                if (dotIndex > 1 && find(extensions, end(extensions), fullPath.substr(dotIndex + 1)) != end(extensions))
                    filePaths.emplace_back(fullPath);
            }
        }
        closedir(dir);
    }
};