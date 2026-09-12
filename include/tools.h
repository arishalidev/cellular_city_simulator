#pragma once
#include <map>
#include <utility>
#include <vector>
#include <queue>

struct SortOrder {
    bool operator()(const std::array<int, 4>& a, const std::array<int, 4>& b) const {
        if (a[0] != b[0]) return a[0] < b[0];
        if (a[1] != b[1]) return a[1] < b[1];
        if (a[2] != b[2]) return a[2] > b[2];
        return a[3] > b[3];
    }
};

const std::vector<std::pair<int,int>> adjacentCoordsOffset = {
    {0, 1}, {1, 0}, {0, -1}, {-1, 0},
    {1, 1}, {-1, -1}, {-1, 1}, {1, -1}
};


std::map<std::pair<int, int>, std::pair<int, int>>  getAdjacentCoords(std::map<std::pair<int, int>, int>& r1, std::map<std::pair<int, int>, int>& r2, int con1, int con2);
std::priority_queue<std::array<int, 4>, std::vector<std::array<int, 4>>, SortOrder> getOrderOfCellGrowth(std::map<std::pair<int, int>, int> region, std::map<std::pair<int, int>, std::pair<int, int>> cells);
int numOfAdjacentAndSame(std::pair<std::pair<int, int>, int> cell, std::map<std::pair<int, int>, int> region, bool returnTotalPop = false);
