#pragma once
#include <vector>
#include <memory>
#include <utility>
#include <map>
#include <set>

#include "commercial.h"

class Industrial {
private:
    std::shared_ptr<std::vector<std::vector<char>>> regionState;
    std::shared_ptr<Commercial> commercial;
    std::shared_ptr<int> workers;
    std::shared_ptr<int> goods;

    //{{y,x},population}
    std::map<std::pair<int, int>, int> regionInfo;
    std::map<std::pair<int, int>, int> powerlineInfo;
    
public:
    Industrial(std::shared_ptr<Commercial> commercial, std::shared_ptr<std::vector<std::vector<char>>> regionState, std::shared_ptr<int> workers, std::shared_ptr<int> goods);
    bool applyRules(bool changed);
    int getPopulation(bool specifiedRange = false, int yStart = -1, int yEnd = -1, int xStart = -1, int xEnd = -1);
    std::map<std::pair<int, int>, int> getRegionInfo();
};
