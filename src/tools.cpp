#include <map>
#include <utility>
#include <vector>
#include <queue>
#include <set>

#include "tools.h"

std::map<std::pair<int, int>, std::pair<int, int>>  getAdjacentCoords(std::map<std::pair<int, int>, int> r1, std::map<std::pair<int, int>, int> r2, int con1, int con2) {
    
    //Create a new map to store new pairs in
    std::map<std::pair<int, int>, std::pair<int, int>> adjacentCoordsMap;

    //Loop though region we are searching for
    for(const std::pair<std::pair<int, int>, int> cell : r1) {
        
        //For every cell in region we are searching for, loop though all possible neighbors of cell
        for (const std::pair<int, int> offset : adjacentCoordsOffset) {
            
            //Define neighbor as coordinates of cell + offset
            const std::pair<int, int> neighbor = {cell.first.first + offset.first, cell.first.second + offset.second};
            
            //See if neighbor cell exists and is adjacent
            const std::map<std::pair<int, int>, int>::iterator neighborCell = r2.find(neighbor);
            if (neighborCell != r2.end()) {
                
                //Test to see if cells meet conditions
                if(cell.second == con1 && neighborCell->second == con2) {
                    adjacentCoordsMap[cell.first] = neighborCell->first;
                }
            }
        }
    }
    return adjacentCoordsMap;
}


std::priority_queue<std::array<int, 4>, std::vector<std::array<int, 4>>, SortOrder> getOrderOfCellGrowth(std::map<std::pair<int, int>, int> regionInfo, std::map<std::pair<int, int>, std::pair<int, int>> cells) {
    
    //Create prioroity queue to prefer larger populations and adjacency, and lower Y and X coordinates
    std::priority_queue<std::array<int, 4>, std::vector<std::array<int, 4>>, SortOrder>  cellGrowthQueue;
    
    std::array<int, 4> growingCell;
    
    //Loop though cells, sort and add them to priority queue
    for(std::pair<std::pair<int, int>, std::pair<int, int>> cell : cells) {
        
        growingCell[0] = regionInfo[cell.first];
        growingCell[1] = numOfAdjacentAndSame({cell.first, regionInfo[cell.first]}, regionInfo, true);
        growingCell[2] = cell.first.first;
        growingCell[3] = cell.first.second;
        cellGrowthQueue.push(growingCell);
    }
    
    return cellGrowthQueue;
}
int numOfAdjacentAndSame(std::pair<std::pair<int, int>, int> cell, std::map<std::pair<int, int>, int> region, bool returnTotalPop) {
    
    //Find the cell in we are looking for in the region
    const std::map<std::pair<int, int>, int>::iterator cellInRegion = region.find(cell.first);
    int num = 0;
    
    //Make sure the cell is actually in the region
    if (cellInRegion == region.end()) return num;
    
    //For every cell in region we are searching for, loop though all possible neighbors of cell
    for (const std::pair<int, int> offset : adjacentCoordsOffset) {
        
        //Define neighbor as coordinates of cell + offset
        const std::pair<int, int> neighbor = {cell.first.first + offset.first, cell.first.second + offset.second};
        
        //Find the neighbor cells coordinates in region to get its population
        const std::map<std::pair<int, int>, int>::iterator neighborCellCoords = region.find(neighbor);
        
        //Test to see if neighbor cell is in region, and if cell is adjacent to cell with greater or equal population
        if(neighborCellCoords != region.end() && cell.second <= neighborCellCoords->second) {
            
            if(returnTotalPop){
                num = neighborCellCoords->second + num;
            } else {
                num++;
            }
        }
        
    }

    
    return num;
}
