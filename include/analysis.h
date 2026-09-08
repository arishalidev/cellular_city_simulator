#pragma once
#include <vector>
#include <memory>
#include <utility>

#include "residential.h"
#include "commercial.h"
#include "industrial.h"
#include "pollution.h"

class Analysis {
private:
    //Saving pointers in Analysis class
    std::shared_ptr<Residential> residential;
    std::shared_ptr<Commercial> commercial;
    std::shared_ptr<Industrial> industrial;
    std::shared_ptr<Pollution> pollution;
    std::shared_ptr<int> workers;
    std::shared_ptr<int> goods;
    std::shared_ptr<bool> exitCoordLoop;

    std::shared_ptr<std::vector<std::vector<char>>> pollutionState;
    std::shared_ptr<std::vector<std::vector<char>>> regionState;
    
    std::tuple<int,int,int,int> getZone(std::pair<std::pair<int, int>, std::pair<int, int>> zone);
public:
    Analysis(std::shared_ptr<std::vector<std::vector<char>>> regionState, std::shared_ptr<std::vector<std::vector<char>>> pollutionState, std::shared_ptr<int> workers, std::shared_ptr<int> goods, std::shared_ptr<Residential> residential, std::shared_ptr<Commercial> commercial, std::shared_ptr<Industrial> industrial, std::shared_ptr<Pollution> pollution, std::shared_ptr<bool> exitCoordLoop);
    
    void printRegionState(bool printFinal = false, std::pair<std::pair<int, int>, std::pair<int, int>> zone = {{-1,-1},{-1,-1}});
    void printPollutionState(bool printFinal = false, std::pair<std::pair<int, int>, std::pair<int, int>>  zone = {{-1,-1},{-1,-1}});
    void printRegionInfo(int time = -1, bool simDone = false, std::pair<std::pair<int, int>, std::pair<int, int>>  zone = {{-1,-1},{-1,-1}});
    bool validateCoords(int coords, bool xCoord = false);
};
