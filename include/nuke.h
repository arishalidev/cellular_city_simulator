#pragma once
#include <utility>
#include <vector>
#include <memory>

class Nuke {
private:
    std::shared_ptr<std::vector<std::vector<char>>> regionState;
    
    std::pair<int, int> nukeCoords;
    int nukeSize;
public:
    Nuke(std::pair<int, int> nukeCoords, int nukeSize, std::shared_ptr<std::vector<std::vector<char>>> regionState);
    void deploy();
};
