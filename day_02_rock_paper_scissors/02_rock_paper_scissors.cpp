#include <fstream> 
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>


std::vector<std::pair<unsigned char, unsigned char>> processData(const std::string& filename) {
    std::ifstream file(filename);
    
    if (!file) {
        std::cerr << "Error opening the file: " << filename << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::string lineString;
    std::vector<std::pair<unsigned char, unsigned char>> encryptedStrategyGuide;

    while (std::getline(file, lineString)) {
        if (!lineString.empty()) {
            encryptedStrategyGuide.push_back(std::make_pair(lineString[0], lineString[2]));
        }
    }

    return encryptedStrategyGuide;
}


int calculateScore(const std::vector<std::pair<unsigned char, unsigned char>>& encryptedStrategyGuide) {
    enum class Choice {Rock = 1, Paper = 2, Scissors = 3};
    std::unordered_map<unsigned char, Choice> shapeScore = {
        {'A', Choice::Rock},
        {'B', Choice::Paper},
        {'C', Choice::Scissors},
        {'X', Choice::Rock},
        {'Y', Choice::Paper},
        {'Z', Choice::Scissors}
    };
    int total = 0;

    for (const auto& pair : encryptedStrategyGuide) {
        auto [opponentChoice, playerChoice] = pair;
        std::array<int, 3> scoreResults = {3, 6, 0};
        total += static_cast<int>(shapeScore[playerChoice]) + 
                 scoreResults[(3 + static_cast<int>(shapeScore[playerChoice]) - 
                              static_cast<int>(shapeScore[opponentChoice])) % 3];
    }
    
    return total;
}


int main() {
    auto processedData = processData("../input/02.txt");
    // auto processedData = processData("../test_input/02.txt");  // 15

    int result = calculateScore(processedData);
    std::cout << result << std::endl;
    return 0;
}