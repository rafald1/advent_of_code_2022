#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>


struct MathMonkey {
    std::string leftMonkeyName{};
    std::string rightMonkeyName{};
    char operation{};
};


std::pair<std::unordered_map<std::string, long>, std::unordered_map<std::string, MathMonkey>>
processData(const std::string& filename) {
    std::ifstream file(filename);

    if (!file)
        throw std::runtime_error("Error opening the file: " + filename);

    std::string line{};
    std::unordered_map<std::string, long> simpleMonkeys{};
    std::unordered_map<std::string, MathMonkey> operationMonkeys{};

    while(getline(file, line)) {
        std::string monkeyName = line.substr(0, 4);
        if (isdigit(line[6])) {
            simpleMonkeys[monkeyName] = std::stol(line.substr(6));
        } else {
            std::string leftMonkayName = line.substr(6, 4);
            std::string rightMonkeyName = line.substr(13, 4);
            char operation = line.substr(11, 1).front();
            operationMonkeys[monkeyName] = {leftMonkayName, rightMonkeyName, operation};
        }
    }
    return {simpleMonkeys, operationMonkeys};
}


long calculate(
    const std::unordered_map<std::string, long>& simpleMonkeys,
    const std::unordered_map<std::string, MathMonkey>& operationMonkeys,
    const std::string& monkeyName
) {
    if (simpleMonkeys.contains(monkeyName))
        return simpleMonkeys.at(monkeyName);

    const auto& [leftMonkeyName, rightMonkeyName, operation] = operationMonkeys.at(monkeyName);
    const long leftResult = calculate(simpleMonkeys, operationMonkeys, leftMonkeyName);
    const long rightResult = calculate(simpleMonkeys, operationMonkeys, rightMonkeyName);

    switch (operation) {
        case '+':
            return leftResult + rightResult;
        case '-':
            return leftResult - rightResult;
        case '*':
            return leftResult * rightResult;
        case '/':
            return leftResult / rightResult;
        default:
            throw std::runtime_error("Unsupported operation: " + std::to_string(operation));
    }
}


int main() {
    try {
        auto [processedSimpleMonkeys, processedOperationMonkeys] = processData("../input/21.txt");
        // auto [processedSimpleMonkeys, processedOperationMonkeys] = processData("../test_input/21.txt");  // 152

        long result = calculate(processedSimpleMonkeys, processedOperationMonkeys, "root");
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return 0;
}