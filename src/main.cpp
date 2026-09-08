#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <set>
#include <utility>
#include <chrono>
#include <thread>

#include "initialize.h"
#include "analysis.h"
#include "residential.h"
#include "commercial.h"
#include "industrial.h"
#include "pollution.h"
#include "nuke.h"

int main() {
    std::string inputFile;

    std::cout << "Please enter input file: ";
    std::cin >> inputFile;
    
    //Read in config file and initial region state
    config configSettings = readConfigFile(inputFile);
    std::shared_ptr<std::vector<std::vector<char>>> regionState = readRegionLayoutFile(configSettings.regionLayoutFile);
    
    std::shared_ptr<std::vector<std::vector<char>>> pollutionState = std::make_shared<std::vector<std::vector<char>>>(*regionState);
    
    std::shared_ptr<int> workers = std::make_shared<int>(0);
    std::shared_ptr<int> goods = std::make_shared<int>(0);
    std::shared_ptr<bool> exitCoordLoop = std::make_shared<bool>(false);

    //Create pointers for each region type, and pass them to Analysis class
    std::shared_ptr<Residential> residential = std::make_shared<Residential>(regionState, workers);
    std::shared_ptr<Commercial> commercial = std::make_shared<Commercial>(regionState, workers, goods);
    std::shared_ptr<Industrial> industrial = std::make_shared<Industrial>(commercial, regionState, workers, goods);
    std::shared_ptr<Pollution> pollution = std::make_shared<Pollution>(pollutionState, industrial);
    
    std::unique_ptr<Analysis> analyze = std::make_unique<Analysis>(regionState, pollutionState, workers, goods, residential, commercial, industrial, pollution, exitCoordLoop);
    
    bool changeInStates = true;
    int time = 0;
    
    //Loop while time limit is not reached, or if states are changing.
    while ((time < configSettings.timeLimit) && (changeInStates)) {
        changeInStates = false;
        
        //Print region info if time is not 0
        if(time != 0) analyze->printRegionInfo(time);
          
        //Print region at frequency of refresh rate
        if(time % configSettings.refreshRate == 0) {
            analyze->printRegionState();
        }
        
        std::cout << std::endl;
        
        changeInStates = commercial->applyRules(changeInStates);
        changeInStates = industrial->applyRules(changeInStates);
        changeInStates = residential->applyRules(changeInStates);

        pollution->calculatePollutionSpread();
        
        //Save commercial updating region for last as commercial.applyRules can be called multiple times
        commercial->updateRegion();
        
        time++;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

    }

    //If program exited due to time limit, int(!changeInStates) will be 0, giving the correct time
    analyze->printRegionInfo(time - int(!changeInStates), true);
    
    analyze->printRegionState(true);
    analyze->printPollutionState(true);

    
    std::pair<std::pair<int, int>, std::pair<int, int>> coordsToZoomInOn;
    
    //Loop for zooming in on coordinates
    int input = 0;
    while(true) {
        std::cout << "Enter coordinates to zoom in on (-1 to exit loop): " << std::endl;

        bool exitLoop = false;
        
        //Only run when exitCoordLoop is false (user enteres -1)
        //Ask user for coords and validate them
        while(!*exitCoordLoop) {
            std::cout << "y1: ";
            std::cin >> coordsToZoomInOn.first.first;
            
            exitLoop = analyze->validateCoords(coordsToZoomInOn.first.first, false);
            if(!exitLoop) continue; else break;
        }
        
        while(!*exitCoordLoop) {
            std::cout << "x1: ";
            std::cin >> coordsToZoomInOn.first.second;

            exitLoop = analyze->validateCoords(coordsToZoomInOn.first.second, true);
            if(!exitLoop) continue; else break;
        }
        
        while(!*exitCoordLoop) {
            std::cout << "y2: ";
            std::cin >> coordsToZoomInOn.second.first;

            exitLoop = analyze->validateCoords(coordsToZoomInOn.second.first, false);
            if(!exitLoop) continue; else break;
        }
        
        while(!*exitCoordLoop) {
            std::cout << "x2: ";
            std::cin >> coordsToZoomInOn.second.second;
            
            exitLoop = analyze->validateCoords(coordsToZoomInOn.second.second, true);
            if(!exitLoop) continue; else break;
        }

        //Exit main loop if user
        if(*exitCoordLoop) break;

        //Print region info for user defined coords
        analyze->printRegionInfo(-1, false, coordsToZoomInOn);
        analyze->printRegionState(false, coordsToZoomInOn);
        analyze->printPollutionState(false, coordsToZoomInOn);
    }
    
    //NUKE FEATURE
    
    std::cout << "Enter coordinates to drop nuke (-1 to spare lives): " << std::endl;
    
    *exitCoordLoop = false;
    std::pair<int, int> nukeCoords;
    int nukeSize;
    
    bool exitLoop = false;

    while(!*exitCoordLoop) {
        std::cout << "y: ";
        std::cin >> nukeCoords.first;
        
        exitLoop = analyze->validateCoords(nukeCoords.first, false);
        if(!exitLoop) continue; else break;
    }
    
    while(!*exitCoordLoop) {
        std::cout << "x: ";
        std::cin >> nukeCoords.second;

        exitLoop = analyze->validateCoords(nukeCoords.second, true);
        if(!exitLoop) continue; else break;
    }

    while(!*exitCoordLoop) {
        std::cout << "Enter size of nuke in kT: ";
        std::cin >> nukeSize;

        if(nukeSize <= 0) {
            std::cout << "Nuke size is too small!" << std::endl;
            continue;
        } else break;
    }
    
    if(!*exitCoordLoop) {
        Nuke nuke(nukeCoords, nukeSize, regionState);
        nuke.deploy();
        
        std::cout << "Nuke Deployed!" << std::endl << std::endl;
        analyze->printRegionState(false, coordsToZoomInOn);
    }
    
    //Loop for zooming in on coordinates
    input = 0;
    while(true) {
        std::cout << "Enter coordinates to zoom in on (-1 to exit loop): " << std::endl;

        bool exitLoop = false;
        
        //Only run when exitCoordLoop is false (user enteres -1)
        //Ask user for coords and validate them
        while(!*exitCoordLoop) {
            std::cout << "y1: ";
            std::cin >> coordsToZoomInOn.first.first;
            
            exitLoop = analyze->validateCoords(coordsToZoomInOn.first.first, false);
            if(!exitLoop) continue; else break;
        }
        
        while(!*exitCoordLoop) {
            std::cout << "x1: ";
            std::cin >> coordsToZoomInOn.first.second;

            exitLoop = analyze->validateCoords(coordsToZoomInOn.first.second, true);
            if(!exitLoop) continue; else break;
        }
        
        while(!*exitCoordLoop) {
            std::cout << "y2: ";
            std::cin >> coordsToZoomInOn.second.first;

            exitLoop = analyze->validateCoords(coordsToZoomInOn.second.first, false);
            if(!exitLoop) continue; else break;
        }
        
        while(!*exitCoordLoop) {
            std::cout << "x2: ";
            std::cin >> coordsToZoomInOn.second.second;
            
            exitLoop = analyze->validateCoords(coordsToZoomInOn.second.second, true);
            if(!exitLoop) continue; else break;
        }

        //Exit main loop if user
        if(*exitCoordLoop) break;

        //Print region info for user defined coords
        analyze->printRegionInfo(-1, false, coordsToZoomInOn);
        analyze->printRegionState(false, coordsToZoomInOn);
        analyze->printPollutionState(false, coordsToZoomInOn);
    }



    return 1;
}

