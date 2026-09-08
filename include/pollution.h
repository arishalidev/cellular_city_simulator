#pragma once
#include <memory>
#include <string>
#include <utility>
#include <map>

#include "industrial.h"

class Pollution {
private:
    std::shared_ptr<Industrial> industrial;
    std::shared_ptr<std::vector<std::vector<char>>> pollutionState;
    
    std::map<std::pair<int, int>, int> regionStateMap;
    std::map<std::pair<int, int>, int> pollutionSpread;

    int totalPollution;

public:
    Pollution(std::shared_ptr<std::vector<std::vector<char>>> pollutionState, std::shared_ptr<Industrial> industrial);
    void calculatePollutionSpread();
    int getTotalPollution(bool specifiedRange = false, int yStart = -1, int yEnd = -1, int xStart = -1, int xEnd = -1);
};
