#pragma once
#include <vector>
#include <utility>
#include <map>
#include <memory>

class Residential {
private:
    std::shared_ptr<std::vector<std::vector<char>>> regionState;
    std::shared_ptr<int> workers;
    
    //{{y,x},population}
    std::map<std::pair<int, int>, int> powerlineInfo;
    
public:
    std::map<std::pair<int, int>, int> regionInfo;

    Residential(std::shared_ptr<std::vector<std::vector<char>>> regionState, std::shared_ptr<int> workers);
    bool applyRules(bool changeInStates);
    int getPopulation(bool specifiedRange = false, int yStart = -1, int yEnd = -1, int xStart = -1, int xEnd = -1);
};
