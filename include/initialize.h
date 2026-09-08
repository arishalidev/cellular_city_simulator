#pragma once
#include <string>
#include <vector>
#include <memory>

struct config {
    std::string regionLayoutFile;
    int timeLimit = -1;
    int refreshRate = -1;
};

config readConfigFile(std::string inputFile);
std::shared_ptr<std::vector<std::vector<char>>> readRegionLayoutFile(std::string regionFile);
