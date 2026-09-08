#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "initialize.h"

config readConfigFile(std::string inputFile) {
    config configSettings;

    //Open file
    std::ifstream file(inputFile);
    if (!file.is_open()) {
        std::cerr << "Failed to open file " << inputFile << "!" << std::endl;
        exit(0);
    }

    std::string line;
    int i = 0;
    std::string delimiter = ":";
    
    //Loop though each line, store information in config
    while (std::getline(file, line)) {
        switch (i) {
            case 0:
                configSettings.regionLayoutFile = line.substr(line.find(delimiter) + delimiter.length(), line.length());
                break;
            case 1:
                configSettings.timeLimit = std::stoi(line.substr(line.find(delimiter) + delimiter.length(), line.length()));
                break;
            case 2:
                configSettings.refreshRate = std::stoi(line.substr(line.find(delimiter) + delimiter.length(), line.length()));
                break;
            default:
                break;
        }
        i++;
    }
    
    file.close();

    return configSettings;
}

std::shared_ptr<std::vector<std::vector<char>>> readRegionLayoutFile(std::string regionFile) {
    std::shared_ptr<std::vector<std::vector<char>>> initialRegionState = std::make_shared<std::vector<std::vector<char>>>();
    
    //Open file
    std::ifstream file(regionFile);
    if (!file.is_open()) {
        std::cerr << "Failed to open file " << regionFile << "!" << std::endl;
        exit(0);
    }
    
    std::string line;
    std::string delimiter = ",";
    
    //Loop though each line, storing each char in the region layout matrix
    while (std::getline(file, line)) {
        std::vector<char> newRow;
        size_t i = 0;
        
        //Loop though each item in row
        while ((i = line.find(delimiter)) != std::string::npos) {
            
            //Add new char to row
            newRow.push_back(line.substr(0, i)[0]);
            
            //Delete information already in row
            line.erase(0, i + delimiter.length());
        }

        //Add the last element
        newRow.push_back(line[0]);
        
        //Add row to matrix
        initialRegionState->push_back(newRow);
    }
    
    
    //Make region is not empty
    if(initialRegionState->empty()) {
        std::cerr << "Region state empty!" << std::endl;
        exit(0);
    }

    file.close();
    return initialRegionState;
}
