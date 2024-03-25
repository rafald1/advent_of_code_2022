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


int getPlayerSymbolValue(int opponentValue, int roundOutcome) {
    if (roundOutcome == 0) {       
        return (opponentValue - 1) ?  opponentValue - 1 : 3;
    } else if (roundOutcome == 6) {
        return (opponentValue % 3) ? opponentValue + 1 : 1;
    } else if (roundOutcome == 3) {
        return opponentValue;
    }
    
    std::cerr << "Wrong round outcome: " << roundOutcome << std::endl;
    std::exit(EXIT_FAILURE);
}


int calculateScore(const std::vector<std::pair<unsigned char, unsigned char>>& encryptedStrategyGuide) {
    enum class Choice {Rock = 1, Paper = 2, Scissors = 3};
    std::unordered_map<unsigned char, Choice> shapeScore = {
        {'A', Choice::Rock},
        {'B', Choice::Paper},
        {'C', Choice::Scissors}
    };
    enum class Outcome {Lose = 0, Draw = 3, Win = 6};
    std::unordered_map<unsigned char, Outcome> roundOutcomes = {
        {'X', Outcome::Lose},
        {'Y', Outcome::Draw},
        {'Z', Outcome::Win}
    };
    int total = 0;

    for (const auto& pair : encryptedStrategyGuide) {
        auto [opponentChoice, outcome] = pair;
        total += static_cast<int>(roundOutcomes[outcome]) + 
                 getPlayerSymbolValue(static_cast<int>(shapeScore[opponentChoice]), 
                                      static_cast<int>(roundOutcomes[outcome]));
    }
    
    return total;
}


int main() {
    auto processedData = processData("../input/02.txt");
    // auto processedData = processData("../test_input/02.txt");  // 12

    int result = calculateScore(processedData);
    std::cout << result << std::endl;
    return 0;
}