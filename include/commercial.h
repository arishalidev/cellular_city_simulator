#pragma once

#include <vector>
#include <utility>
#include <map>
#include <memory>
#include <set>

class Commercial {
private:
    std::shared_ptr<std::vector<std::vector<char>>> regionState;
    std::shared_ptr<int> workers;
    std::shared_ptr<int> goods;

    //{{y,x},population}
    std::map<std::pair<int, int>, int> regionInfo;
    std::map<std::pair<int, int>, int> powerlineInfo;
    
    //Set for tracking which cells have been updated
    std::set<std::pair<int, int>> updatedCells;
    
    //Map for storing new information
    std::map<std::pair<int, int>, int> newRegionInfo;
        
public:
    Commercial(std::shared_ptr<std::vector<std::vector<char>>> regionState, std::shared_ptr<int> workers, std::shared_ptr<int> goods);
    bool applyRules(bool changed);
    void updateRegion();
    int getPopulation(bool specifiedRange = false, int yStart = -1, int yEnd = -1, int xStart = -1, int xEnd = -1);
};
