#include <memory>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

#include "pollution.h"
#include "tools.h"

Pollution::Pollution(std::shared_ptr<std::vector<std::vector<char>>> pollutionState, std::shared_ptr<Industrial> industrial) : industrial(industrial), pollutionState(pollutionState) {
    
    //Loop though regionState,add information to map
    for (int i = 0; i < pollutionState->size(); i++) {
        for (int j = 0; j < pollutionState->at(i).size(); j++) {
            this->regionStateMap[{i,j}] = 0;
        }
    }
}

void Pollution::calculatePollutionSpread() {
    this->totalPollution = 0;
    
    //Load data from industrial class to pollution spread
    for(const std::pair<std::pair<int, int>, int> pollutionSource : this->industrial->getRegionInfo()) {
        if(pollutionSource.second != 0) this->pollutionSpread[pollutionSource.first] = pollutionSource.second;
    }
    
    //Load data from pollution spread to region state
    for(const std::pair<std::pair<int, int>, int> regionCell : this->regionStateMap) {
        this->regionStateMap[regionCell.first] = this->pollutionSpread[regionCell.first];
    }

    //Track number of changes in pollution spread, exit loop when no more changes
    for(int i = 0; i < 2; i++) {


        //Get coordinates of cells that have pollution that can spread
        std::map<std::pair<int, int>, std::pair<int, int>> pollutedCell = getAdjacentCoords(this->regionStateMap, this->pollutionSpread, 0, 3);
        pollutedCell.merge(getAdjacentCoords(this->regionStateMap, this->pollutionSpread, 1, 3));
        pollutedCell.merge(getAdjacentCoords(this->regionStateMap, this->pollutionSpread, 0, 2));

        //Loop though all cells that had pollution spread
        for(std::pair<std::pair<int, int>, std::pair<int, int>> cell : pollutedCell) {

            //Spread the pollution
            if(this->pollutionSpread[cell.second] == 3) this->pollutionSpread[cell.first] = 2;
            if(this->pollutionSpread[cell.second] == 2) this->pollutionSpread[cell.first] = 1;
        }
    
    }
    
    //Loop though all pollution, update pollutionState
    for(const std::pair<std::pair<int, int>, int> cell : this->pollutionSpread) {
        this->totalPollution += cell.second;
        if(cell.second != 0) this->pollutionState->at(cell.first.first).at(cell.first.second) = '0' + cell.second;
    }
}

int Pollution::getTotalPollution(bool specifiedRange, int yStart, int yEnd, int xStart, int xEnd) {
    int regionPopulation = 0;
    
    if(!specifiedRange) {
        
        //Loop though every cell, count up regions population
        for(std::pair<std::pair<int, int>, int> cell : this->pollutionSpread)  {
            regionPopulation += cell.second;
        }
    } else {
        
        //Loop though every cell, count regions population if in bounds
        for(std::pair<std::pair<int, int>, int> cell : this->pollutionSpread)  {
            if(std::clamp(cell.first.first, yStart, yEnd - 1) == cell.first.first && std::clamp(cell.first.second, xStart, xEnd - 1) == cell.first.second) regionPopulation += cell.second;

        }
    }

    return regionPopulation;
}
