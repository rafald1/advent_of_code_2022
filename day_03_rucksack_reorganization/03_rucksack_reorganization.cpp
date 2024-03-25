#include <fstream> 
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>


std::vector<std::pair<std::string, std::string>> processData(const std::string& filename) {
    std::ifstream file(filename);
    
    if (!file) {
        std::cerr << "Error opening the file: " << filename << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::string lineString;
    std::vector<std::pair<std::string, std::string>> rucksacks;

    while (std::getline(file, lineString)) {
        int rucksackSize = lineString.length();
        if (rucksackSize % 2 != 0) {
            std::cerr << "Error: Line does not have an even length." << std::endl;
            continue;
        }
        std::string firstCompartment = lineString.substr(0, rucksackSize / 2);
        std::string secondCompartment = lineString.substr(rucksackSize / 2);
        rucksacks.push_back(std::pair(firstCompartment, secondCompartment));
    }

    return rucksacks;
}

int calculateItemPriority(char item) {
    if (std::isupper(item)) {
        return item - 'A' + 27;
    } else {
        return item - 'a' + 1;
    }
}

int compareCompartments(const std::vector<std::pair<std::string, std::string>>& rucksacks) {
    int total = 0;
    
    for (auto rucksack : rucksacks) {
        auto [firstCompartment, secondCompartment] = rucksack;
        std::set<char> firstCompartmentItems(firstCompartment.begin(), firstCompartment.end());
        std::set<char> secondCompartmentItems(secondCompartment.begin(), secondCompartment.end());
        char commonItem = 0;
        std::set_intersection(
            firstCompartmentItems.begin(), firstCompartmentItems.end(),
            secondCompartmentItems.begin(), secondCompartmentItems.end(),
            &commonItem
        );
        total += calculateItemPriority(commonItem);
    }

    return total;
}

int main() {
    auto processedData = processData("../input/03.txt");
    // auto processedData = processData("../test_input/03.txt");  // 157
    
    int result = compareCompartments(processedData);
    std::cout << result << std::endl;
    return 0;
}