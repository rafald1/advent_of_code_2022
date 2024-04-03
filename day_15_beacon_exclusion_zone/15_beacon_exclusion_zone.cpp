#include <algorithm>
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


std::vector<std::pair<int, int>> calculateRangesWhereBeaconCannotExist(
    const std::vector<std::tuple<int, int, int, int>>& sensorsAndBeacons,
    const int& row
) {
    std::vector<std::pair<int, int>> ranges{};
    
    for (const auto& [sensorX, sensorY, beaconX, beaconY] : sensorsAndBeacons) {
        int distance{std::abs(sensorX - beaconX) + std::abs(sensorY - beaconY)};
        int remainingDistance{distance - std::abs(sensorY - row)};
        
        if (remainingDistance > 0)
            ranges.emplace_back(sensorX - remainingDistance, sensorX + remainingDistance);
    }

    return ranges;
}


std::vector<std::pair<int, int>> combineRangesWhereBeaconCannotExist(
    std::vector<std::pair<int, int>>& ranges
) {
    std::sort(ranges.begin(), ranges.end());
    std::vector<std::pair<int, int>> combinedRanges{};

    for (const auto& range : ranges) {
        if (!combinedRanges.empty() && range.first <= combinedRanges.back().second) {
            combinedRanges.back().second = std::max(combinedRanges.back().second, range.second);
        } else {
            combinedRanges.push_back(range);
        }
    }

    return combinedRanges;
}


size_t calculateNumberOfNotPresentPositions(const std::vector<std::pair<int, int>>& ranges) {
    size_t counter{};

    for (const auto& range : ranges) {
        counter += range.second - range.first + 1;
    }

    return counter;
}


size_t calculateNumberOfPositionOccupiedByKnownSensorsAndBeacons(
    const std::vector<std::tuple<int, int, int, int>>& sensorsAndBeacons, 
    const int& rowNumber
) {
    std::unordered_set<int> uniqueSensorsAndBeacons{};

    for (const auto& [sensorX, sensorY, beaconX, beaconY] : sensorsAndBeacons) {
        if (sensorY == rowNumber)
            uniqueSensorsAndBeacons.insert(sensorX);
        if (beaconY == rowNumber)
            uniqueSensorsAndBeacons.insert(beaconX);
    }

    return uniqueSensorsAndBeacons.size();
}


int main() {
    try {
        auto discoveredSensorsAndBeacons = processData("../input/15.txt");
        int row{2000000};
        // auto discoveredSensorsAndBeacons = processData("../test_input/15.txt");  // 26
        // int row{10};

        auto allRanges = calculateRangesWhereBeaconCannotExist(discoveredSensorsAndBeacons, row);
        auto allCombinedRanges = combineRangesWhereBeaconCannotExist(allRanges);
        size_t notPresent = calculateNumberOfNotPresentPositions(allCombinedRanges);
        size_t occupied = calculateNumberOfPositionOccupiedByKnownSensorsAndBeacons(discoveredSensorsAndBeacons, row);
        std::cout << notPresent - occupied << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}