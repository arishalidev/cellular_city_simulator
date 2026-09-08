#include <vector>
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>

#include "analysis.h"

//Move pointers into analysis class
Analysis::Analysis(std::shared_ptr<std::vector<std::vector<char>>> regionState, std::shared_ptr<std::vector<std::vector<char>>> pollutionState, std::shared_ptr<int> workers, std::shared_ptr<int> goods, std::shared_ptr<Residential> residential, std::shared_ptr<Commercial> commercial, std::shared_ptr<Industrial> industrial, std::shared_ptr<Pollution> pollution, std::shared_ptr<bool> exitCoordLoop) : regionState(regionState), pollutionState(pollutionState), workers(workers), goods(goods), residential(residential), commercial(commercial), industrial(industrial), pollution(pollution), exitCoordLoop(exitCoordLoop) {}

std::tuple<int,int,int,int> Analysis::getZone(std::pair<std::pair<int, int>, std::pair<int, int>> zone) {
    size_t yStart;
    size_t yEnd;
    
    size_t xStart;
    size_t xEnd;
    
    if(zone.first.first == -1) {
        
        //Define max bounds for printing region state
        yStart = 0;
        yEnd = this->regionState->size();
        
        xStart = 0;
        xEnd = this->regionState->at(0).size();
    } else {
        
        //Define users inputed bounds for printing region state
        yStart = std::min(zone.first.first, zone.second.first);
        yEnd = yStart + std::abs(zone.first.first - zone.second.first) + 1;
        
        xStart = std::min(zone.first.second, zone.second.second);
        xEnd = xStart + std::abs(zone.first.second - zone.second.second) + 1;

    }
    
    //Return values in tuple for ease
    return std::make_tuple(yStart, yEnd ,xStart ,xEnd);
}

void Analysis::printRegionState(bool printFinal, std::pair<std::pair<int, int>, std::pair<int, int>> zone) {
    
    auto [yStart, yEnd ,xStart ,xEnd] = this->getZone(zone);

    if(printFinal) {
        std::cout << "Final ";
    }
    std::cout << "Region State:" << std::endl;
    
    
    //Loop and print region state
    for (size_t i = yStart; i < yEnd; i++) {
        for (size_t j = xStart; j < xEnd; j++) {
            std::cout << this->regionState->at(i).at(j) << "    ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Analysis::printPollutionState(bool printFinal, std::pair<std::pair<int, int>, std::pair<int, int>> zone) {
    
    auto [yStart, yEnd ,xStart ,xEnd] = this->getZone(zone);

    if(printFinal) {
        std::cout << "Final ";
    }
    std::cout << "Pollution State:" << std::endl;
    

    //Loop and print region state
    for (size_t i = yStart; i < yEnd; i++) {
        for (size_t j = xStart; j < xEnd; j++) {

                char regionCell = this->regionState->at(i).at(j);
                char pollutionCell = this->pollutionState->at(i).at(j);
                
                //If region state cell is empty
                if(regionCell == ' ') {
                    std::cout << pollutionCell << "    ";
                    
                //If region state cell is the same as the pollution cell
                } else if (regionCell == pollutionCell) {
                    std::cout << regionCell << "    ";

                //If cell is polluted.
                } else if (std::isdigit(pollutionCell)) {
                    std::cout << regionCell << "(" << pollutionCell << ") ";

                //All other cases
                } else {
                    std::cout << regionCell << "    ";
                }
                
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    
}

void Analysis::printRegionInfo(int time, bool simDone, std::pair<std::pair<int, int>, std::pair<int, int>> zone) {
    
    auto [yStart, yEnd ,xStart ,xEnd] = this->getZone(zone);
    bool specifiedRange = false;
    
    //If no user defined zone
    if(zone.first.first == -1) {
        if(simDone) {
            std::cout << "↓ Final Region Information" << std::endl;
            std::cout << "Total time taken: " << time << std::endl;
            std::cout << "Final workers available: " << *this->workers << std::endl;
            std::cout << "Final goods available: " << *this->goods << std::endl;


        } else {
            std::cout << "↓ Region Information for Timestamp #" << time << std::endl;
            std::cout << "Total workers available: " << *this->workers << std::endl;
            std::cout << "Total goods available: " << *this->goods << std::endl;

        }
    } else {
        specifiedRange = true;
        std::cout << std::endl;
        std::cout << "↓ Information for Defined Bounds (" <<
        yStart << ", " << xStart << ") to (" << yEnd - 1 << ", " << xEnd - 1  << "): " << std::endl;
    }
    
    std::cout << "Total residential population: " << residential->getPopulation(specifiedRange, yStart, yEnd ,xStart ,xEnd) << std::endl;
    std::cout << "Total industrial population: " << industrial->getPopulation(specifiedRange, yStart, yEnd ,xStart ,xEnd) << std::endl;
    std::cout << "Total commercial population: " << commercial->getPopulation(specifiedRange, yStart, yEnd ,xStart ,xEnd) << std::endl;
    std::cout << "Total region pollution: " << pollution->getTotalPollution(specifiedRange, yStart, yEnd ,xStart ,xEnd) << std::endl;
}


bool Analysis::validateCoords(int coords, bool xCoord) {
    
    //If coords are negative
    if(coords < 0) {
        *this->exitCoordLoop = true;
        return true;
    }
        
    //If coords are refering to the x axis
    if(xCoord) {
        
        //Make sure coordinates are in bounds of region
        if(coords >= this->regionState->at(0).size()) {
            std::cout << "Coordinates out of bounds! " << std::endl;
            return false;
        } else return true;
        
    //If coords are refering to the y axis
    } else {
        
        //Make sure coordinates are in bounds of region
        if(coords >= this->regionState->size()) {
            std::cout << "Coordinates out of bounds! " << std::endl;
            return false;
        } else return true;
    }
    
    return true;
}
