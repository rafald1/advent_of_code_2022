#include <algorithm>
#include <iostream>
#include <fstream>
#include <numeric>
#include <string>
#include <vector>


std::ifstream openFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Error opening the file: " + filename);
    }
    return file;
}

int findMaxCalories(std::ifstream& inputFile) {
    std::string lineString;
    
    std::vector<unsigned int> maxCalories;
    unsigned int currentCalories = 0;
    
    while (std::getline(inputFile, lineString)) {
        if (lineString == "") {
            maxCalories.push_back(currentCalories);
            currentCalories = 0;
        } else {
            currentCalories += std::stoi(lineString);
        }
    }

    maxCalories.push_back(currentCalories);
    std::sort(maxCalories.rbegin(), maxCalories.rend());

    return std::accumulate(maxCalories.begin(), maxCalories.begin() + 3, 0);
}

int main() {
    std::ifstream inputFile = openFile("../input/01.txt");
    // std::ifstream inputFile = openFile("../test_input/01.txt");  // 45000
    
    int result = findMaxCalories(inputFile);
    inputFile.close();
    std::cout << result << std::endl;

    return 0;
}