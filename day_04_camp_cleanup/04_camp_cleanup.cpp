#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <tuple>
#include <vector>


std::vector<std::tuple<int, int, int, int>> processData(const std::string& filename) {
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Error opening the file: " << filename << std::endl;
        std::exit(EXIT_FAILURE);
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

int countContainedRanges(const std::vector<std::tuple<int, int, int, int>>& rangePairs) {
    int containedRangesCounter = 0;

    for (auto rangePair : rangePairs) {
        auto [rangeAStart, rangeAEnd, rangeBStart, rangeBEnd] = rangePair;
        if ((rangeAStart <= rangeBStart && rangeAEnd >= rangeBEnd) ||
            (rangeBStart <= rangeAStart && rangeBEnd >= rangeAEnd)) {
            containedRangesCounter += 1;
        }
    }

    return containedRangesCounter;
}

int main() {
    auto processedData = processData("../input/04.txt");
    // auto processedData = processData("../test_input/04.txt");  // 2
    
    int result = countContainedRanges(processedData);
    std::cout << result << std::endl;
    return 0;
}