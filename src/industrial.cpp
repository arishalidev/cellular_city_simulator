#include <vector>
#include <set>
#include <memory>
#include <utility>
#include <map>
#include <queue>
#include <array>
#include <algorithm>

#include "tools.h"
#include "industrial.h"
#include "commercial.h"

Industrial::Industrial(std::shared_ptr<Commercial> commercial, std::shared_ptr<std::vector<std::vector<char>>> regionState, std::shared_ptr<int> workers, std::shared_ptr<int> goods) : commercial(commercial), regionState(regionState), workers(workers), goods(goods)  {
    
    //Loops though initial region state, and stores all the industrial and powerline cells
    for (int i = 0; i < this->regionState->size(); i++) {
        for (int j = 0; j < this->regionState->at(i).size(); j++) {
            if (this->regionState->at(i).at(j) == 'I') {
                this->regionInfo[{i,j}] = 0;
            } else if (this->regionState->at(i).at(j) == 'T') {
                this->powerlineInfo[{i,j}] = 0;
            }
        }
    }
    
    //Get all power lines that are adjacent to residential cells
    const std::map<std::pair<int, int>, std::pair<int, int>> relevantPowerlines = getAdjacentCoords(this->regionInfo, this->powerlineInfo, 0, 0);
    
    //Map for new information
    std::map<std::pair<int, int>, int> newPowerLines;

    //Loop though adjacent power lines, add them to new map
    for(const std::pair<std::pair<int, int>, std::pair<int, int>> powerlineCoordinates : relevantPowerlines) {
        newPowerLines[{powerlineCoordinates.second}] = 0;
    }
    
    this->powerlineInfo = newPowerLines;
}

bool Industrial::applyRules(bool changed) {
    
    //Stop region from growing if enough workers dont exist
    if(*this->workers < 2) return changed;

    //Get all cells that are adjacent to power lines and have a population of 0
    std::map<std::pair<int, int>, std::pair<int, int>> growingCells = getAdjacentCoords(this->regionInfo, this->powerlineInfo, 0, 0);

    //Add cells that are adjacent to each other, and meet criteria
    const int maxVal = 3;
    for(int i = 0; i <= maxVal - 1; i++) {
        for(int j = i; j <= maxVal; j++) {
            if(j == 0) continue;
            growingCells.merge(getAdjacentCoords(this->regionInfo, this->regionInfo, i, j));
        }
    }
    
    //Loop though growing cells, delete unqualified ones
    for (auto cell = growingCells.begin(); cell != growingCells.end(); ) {
                
        //Stop cells that have a population of 1 or more, but are not adjacent to enough populous cells to grow
        if(this->regionInfo[cell->first] >= 1) {
            int numOfAdjacent = numOfAdjacentAndSame({cell->first, this->regionInfo[cell->first]}, this->regionInfo);
            if(!(numOfAdjacent >= this->regionInfo[cell->first]*2)) {
                cell = growingCells.erase(cell);
                continue;
            }
        }
        ++cell;
    }
    
    //Map for new information
    std::map<std::pair<int, int>, int> newRegionInfo;
    
    std::priority_queue<std::array<int, 4>, std::vector<std::array<int, 4>>, SortOrder> cellGrowthQueue = getOrderOfCellGrowth(this->regionInfo, growingCells);


    //Loop though all cells that met criteria to grow
    while(!cellGrowthQueue.empty()) {
        
        std::pair<int, int> cell = {cellGrowthQueue.top()[2], cellGrowthQueue.top()[3]};
        cellGrowthQueue.pop();
                
        //Stop cell from growing if enough workers dont exist
        if(*this->workers < 2) continue;
        
        //Add cell to new list, increase population
        newRegionInfo[cell] = this->regionInfo[cell] + 1;
        
        *this->goods += 1;
        *this->workers -= 2;
        
        this->commercial->applyRules(false);
 
        changed = true;
    }
    
    //Loop though updated cells, update the information in the class and region state
    for(std::pair<std::pair<int, int>, int> cell : newRegionInfo) {
        this->regionInfo[cell.first] = cell.second;
        if(cell.second != 0) this->regionState->at(cell.first.first).at(cell.first.second) = '0' + cell.second;
    }

    return changed;
}

int Industrial::getPopulation(bool specifiedRange, int yStart, int yEnd, int xStart, int xEnd) {
    int regionPopulation = 0;
    
    if(!specifiedRange) {
        
        //Loop though every cell, count up regions population
        for(std::pair<std::pair<int, int>, int> cell : this->regionInfo)  {
            regionPopulation += cell.second;
        }
    } else {
        
        //Loop though every cell, count regions population if in bounds
        for(std::pair<std::pair<int, int>, int> cell : this->regionInfo)  {
            if(std::clamp(cell.first.first, yStart, yEnd - 1) == cell.first.first && std::clamp(cell.first.second, xStart, xEnd - 1) == cell.first.second) regionPopulation += cell.second;

        }
    }

    return regionPopulation;
}

std::map<std::pair<int, int>, int> Industrial::getRegionInfo() {
    return this->regionInfo;
}
