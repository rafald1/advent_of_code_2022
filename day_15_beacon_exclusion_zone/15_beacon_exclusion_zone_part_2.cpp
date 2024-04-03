#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>


std::vector<std::tuple<int, int, int, int>> processData(const std::string& filename) {
    std::ifstream file(filename);

    if (!file)
        throw std::runtime_error("Error opening the file: " + filename);

    std::string line{};
    std::vector<std::tuple<int, int, int, int>> sensorsAndBeacons{};

    while(getline(file, line)) {
        int sensorX{}, sensorY{}, beaconX{}, beaconY{};

        if (std::sscanf(line.c_str(), "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d",
            &sensorX, &sensorY, &beaconX, &beaconY) == 4) {
            sensorsAndBeacons.emplace_back(sensorX, sensorY, beaconX, beaconY);
        } else
            throw std::runtime_error("Failed to extract a sensor and a beacon from the processed line:\n" + line);
    }

    return sensorsAndBeacons;
}


int checkCombinedRangeForGap(std::vector<std::pair<int, int>>& ranges) {
    std::sort(ranges.begin(), ranges.end());
    std::vector<std::pair<int, int>> combinedRanges{};

    for (const auto& range : ranges) {
        if (!combinedRanges.empty() && range.first <= combinedRanges.back().second) {
            combinedRanges.back().second = std::max(combinedRanges.back().second, range.second);
        } else {
            combinedRanges.push_back(range);
        }
    }
    
    if (combinedRanges.size() == 1)
        return 0;
    
    return combinedRanges[0].second + 1;
}


long findDistressBeaconFrequency(
    const std::vector<std::tuple<int, int, int, int>>& sensorsAndBeacons,
    const std::pair<int, int>& distressBeaconSearchArea
) {
    auto& [areaFrom, areaTo] = distressBeaconSearchArea;
    std::vector<std::pair<int, int>> ranges{};
    
    for (int distressBeaconY{areaFrom}; distressBeaconY < areaTo; ++distressBeaconY) {
        for (const auto& [sensorX, sensorY, beaconX, beaconY] : sensorsAndBeacons) {
            int distance{std::abs(sensorX - beaconX) + std::abs(sensorY - beaconY)};
            int remainingDistance{distance - std::abs(sensorY - distressBeaconY)};
            
            if (remainingDistance > 0) {
                ranges.emplace_back(
                    sensorX - remainingDistance < areaFrom ? areaFrom : sensorX - remainingDistance, 
                    sensorX + remainingDistance > areaTo ? areaTo : sensorX + remainingDistance
                );
            }
        }

        int distressBeaconX = checkCombinedRangeForGap(ranges);
        if (distressBeaconX != 0)
            return 4000000L * distressBeaconX + distressBeaconY;
        ranges.clear();
    }

    throw std::runtime_error("Failed to find a distress beacon.");
}


int main() {
    try {
        auto discoveredSensorsAndBeacons = processData("../input/15.txt");
        std::pair<int, int> distressBeaconArea{0, 4000000};
        // auto discoveredSensorsAndBeacons = processData("../test_input/15.txt");  // 56000011
        // std::pair<int, int> distressBeaconArea{0, 20};

        auto result = findDistressBeaconFrequency(discoveredSensorsAndBeacons, distressBeaconArea);
        std::cout <<  result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}