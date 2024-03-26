#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <tuple>
#include <vector>


std::vector<std::tuple<int, int, int, int>> readRangesFromFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Error opening the file: " + filename);
    }

    std::regex pattern("(\\d+)-(\\d+),(\\d+)-(\\d+)");
    std::smatch matches;
    std::vector<std::tuple<int, int, int, int>> rangePairs;
    std::string line;

    while (std::getline(file, line)) {
        if (std::regex_match(line, matches, pattern)) {
            int rangeAStart = std::stoi(matches[1].str());
            int rangeAEnd = std::stoi(matches[2].str());
            int rangeBStart = std::stoi(matches[3].str());
            int rangeBEnd = std::stoi(matches[4].str());
        
            rangePairs.emplace_back(rangeAStart, rangeAEnd, rangeBStart, rangeBEnd);
        }
    }

    return rangePairs;
}

int countOverlappedRanges(const std::vector<std::tuple<int, int, int, int>>& rangePairs) {
    int overlappedRangesCounter = 0;

    for (auto& [rangeAStart, rangeAEnd, rangeBStart, rangeBEnd] : rangePairs) {
        if (rangeAEnd < rangeBStart || rangeBEnd < rangeAStart) {
            continue;
        } else {
            overlappedRangesCounter += 1;
        }
    }

    return overlappedRangesCounter;
}

int main() {
    try {
    auto processedData = readRangesFromFile("../input/04.txt");
    // auto processedData = readRangesFromFile("../test_input/04.txt");  // 4
    
    int result = countOverlappedRanges(processedData);
    std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return 0;
}