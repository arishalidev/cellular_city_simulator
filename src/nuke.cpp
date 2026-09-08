#include "nuke.h"
#include <cmath>
#include <iostream>
#include <random>

Nuke::Nuke(std::pair<int, int> nukeCoords, int nukeSize, std::shared_ptr<std::vector<std::vector<char>>> regionState) : regionState(regionState){
    this->nukeCoords = nukeCoords;
    this->nukeSize = nukeSize;
}

void Nuke::deploy() {
    int areaDestroyed = 0;

    //Blast radius in meters
    int blastRadius = 110 * std::cbrt(this->nukeSize);

    for (int i = 0; i < this->regionState->size(); i++) {
        for (int j = 0; j < this->regionState->at(i).size(); j++) {
            
            //Distance in km
            float distance = std::sqrt((i - nukeCoords.first) * (i - nukeCoords.first) + (j - nukeCoords.second) * (j - nukeCoords.second));
            
            
            if(distance < blastRadius/100) {
                this->regionState->at(i).at(j) = 'N';
                areaDestroyed += 100;
            }
        }
    }
    
    std::cout << "Area destoryed: " << areaDestroyed << "m^2" << std::endl;
}
