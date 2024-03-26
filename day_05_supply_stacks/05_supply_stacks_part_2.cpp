#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>


auto processData(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Error opening the file: " + filename);
    }

    std::string line;
    std::unordered_map<int, std::string> stacksOfCrates;

    while (std::getline(file, line)) {
        if (line.empty()) {
            break;
        }

        for (auto it = line.begin(); it != line.end(); ++it) {
            if (std::isalpha(*it)) {
                int stackNumber = (std::distance(line.begin(), it) + 3) / 4;
                stacksOfCrates[stackNumber].insert(stacksOfCrates[stackNumber].begin(), *it);
            }
        }
    }

    std::regex pattern("move (\\d+) from (\\d+) to (\\d+)");
    std::smatch matches;
    std::vector<std::tuple<int, int, int>> rearrangementProcedures;

    while (std::getline(file, line)) {
        if (std::regex_match(line, matches, pattern)) {
            int numberOfCrates = std::stoi(matches[1].str());
            int fromStackNumber= std::stoi(matches[2].str());
            int toStackNumber= std::stoi(matches[3].str());
            rearrangementProcedures.emplace_back(numberOfCrates, fromStackNumber, toStackNumber);
        }
    }
    
    return std::make_tuple(stacksOfCrates, rearrangementProcedures);
}


std::string rearrangeCrates(std::unordered_map<int, std::string>& stacksOfCrates,
                            const std::vector<std::tuple<int, int, int>>& rearrangementProcedures) {
    for (const auto& [numberOfCrates, fromStackNumber, toStackNumber] : rearrangementProcedures) {
        std::size_t position = stacksOfCrates[fromStackNumber].length() - numberOfCrates;
        std::string removedCrates = stacksOfCrates[fromStackNumber].substr(position);
        stacksOfCrates[fromStackNumber].erase(position);
        stacksOfCrates[toStackNumber] += removedCrates;
    }

    std::string topOfCrateStacks;

    for (int i = 1; i <= stacksOfCrates.size(); i++) {
        topOfCrateStacks.push_back(stacksOfCrates[i].back());
    }

    return topOfCrateStacks;
}


int main() {
    try {
        auto [crates, procedures] = processData("../input/05.txt");
        // auto [crates, procedures] = processData("../test_input/05.txt");  // MCD

        std::string result = rearrangeCrates(crates, procedures);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}