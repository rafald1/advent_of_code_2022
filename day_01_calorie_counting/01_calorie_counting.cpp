#include <iostream>
#include <fstream>
#include <string>


std::ifstream openFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Error opening the file: " + filename);
    }
    return file;
}

int findMaxCalories(std::ifstream& inputFile) {
    std::string lineString;
    
    unsigned int maxCalories = 0;
    unsigned int currentCalories = 0;
    
    while (std::getline(inputFile, lineString)) {
        if (lineString == "") {
            if (currentCalories > maxCalories) maxCalories = currentCalories;
            currentCalories = 0;
        } else {
            currentCalories += std::stoi(lineString);
        }
    }

    if (currentCalories > maxCalories) maxCalories = currentCalories;

    return maxCalories;
}

int main() {
    std::ifstream inputFile = openFile("../input/01.txt");
    // std::ifstream inputFile = openFile("../test_input/01.txt");  // 24000
    
    int result = findMaxCalories(inputFile);
    inputFile.close();
    std::cout << result << std::endl;

    return 0;
}