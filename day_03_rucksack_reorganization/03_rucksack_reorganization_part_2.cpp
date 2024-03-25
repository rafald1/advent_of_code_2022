#include <fstream> 
#include <iostream>
#include <string>
#include <vector>


std::vector<std::string> processData(const std::string& filename) {
    std::ifstream file(filename);
    
    if (!file) {
        std::cerr << "Error opening the file: " << filename << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::string lineString;
    std::vector<std::string> rucksacks;

    while (std::getline(file, lineString)) {
        rucksacks.push_back(lineString);
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

int findCommonItem(const std::vector<std::string>& rucksacks) {
    int total = 0;
    
    if (rucksacks.size() % 3 != 0) {
        std::cerr << "Vector size is not a multiple of 3." << std::endl;
        std::exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < rucksacks.size(); i += 3) {
        const std::string& firstElfRucksack = rucksacks[i];
        const std::string& secondElfRucksack = rucksacks[i + 1];
        const std::string& thirdElfRucksack = rucksacks[i + 2];

        for (char commonItem : firstElfRucksack) {
            if (secondElfRucksack.find(commonItem) != std::string::npos &&
                thirdElfRucksack.find(commonItem) != std::string::npos) {
                total += calculateItemPriority(commonItem);
                break;
            }
        }
    }

    return total;
}

int main() {
    auto processedData = processData("../input/03.txt");
    // auto processedData = processData("../test_input/03.txt");  // 70
    
    int result = findCommonItem(processedData);
    std::cout << result << std::endl;
    return 0;
}