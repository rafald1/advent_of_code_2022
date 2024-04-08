#include <exception>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>


struct MathMonkey {
    long value;
    std::string leftMonkeyName{};
    std::string rightMonkeyName{};
    char operation{};
};


std::unordered_map<std::string, MathMonkey> processData(const std::string& filename) {
    std::ifstream file(filename);

    if (!file)
        throw std::runtime_error("Error opening the file: " + filename);

    std::string line{};
    std::unordered_map<std::string, MathMonkey> monkeys{};

    while(getline(file, line)) {
        std::string monkeyName = line.substr(0, 4);
        if (isdigit(line[6])) {
            monkeys[monkeyName] = {std::stol(line.substr(6))};
        } else {
            std::string leftMonkayName = line.substr(6, 4);
            std::string rightMonkeyName = line.substr(13, 4);
            char operation = line.substr(11, 1).front();
            monkeys[monkeyName] = {std::numeric_limits<long>::min(), leftMonkayName, rightMonkeyName, operation};
        }
    }

    return monkeys;
}


long calculate(
    std::unordered_map<std::string, MathMonkey>& monkeys,
    const std::string& monkeyName
) {
    long noValue = std::numeric_limits<long>::min();

    if (monkeyName == "humn")
        return noValue;

    const auto& [value, leftMonkeyName, rightMonkeyName, operation] = monkeys.at(monkeyName);

    if (value != noValue)
        return value;

    const long leftResult = calculate(monkeys, leftMonkeyName);
    const long rightResult = calculate(monkeys, rightMonkeyName);

    if (leftResult == noValue || rightResult == noValue)
        return noValue;

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


long predict(std::unordered_map<std::string, MathMonkey>& monkeys, std::string monkeyName, long predictedValue) {
    if (monkeyName == "humn")
        return predictedValue;

    long noValue = std::numeric_limits<long>::min();
    const auto& [value, leftMonkeyName, rightMonkeyName, operation] = monkeys.at(monkeyName);

    if (value != noValue)
        return value;

    long leftHandValue = calculate(monkeys, leftMonkeyName);

    if (leftHandValue == noValue) {
        long rightHandValue = calculate(monkeys, rightMonkeyName);

        switch (operation) {
            case '+':
                return predict(monkeys, leftMonkeyName, predictedValue - rightHandValue);
            case '-':
                return predict(monkeys, leftMonkeyName, predictedValue + rightHandValue);
            case '*':
                return predict(monkeys, leftMonkeyName, predictedValue / rightHandValue);
            case '/':
                return predict(monkeys, leftMonkeyName, predictedValue * rightHandValue);
            default:
                throw std::runtime_error("Unsupported operation: " + std::to_string(operation));     
        }   
    } else {
        switch (operation) {
            case '+':
                return predict(monkeys, rightMonkeyName, predictedValue - leftHandValue);
            case '-':
                return predict(monkeys, rightMonkeyName, leftHandValue - predictedValue);
            case '*':
                return predict(monkeys, rightMonkeyName, predictedValue / leftHandValue);
            case '/':
                return predict(monkeys, rightMonkeyName, leftHandValue / predictedValue);
            default:
                throw std::runtime_error("Unsupported operation: " + std::to_string(operation));     
        }   
    }
}


int main() {
    try {
        auto processedMonkeys = processData("../input/21.txt");
        // auto processedMonkeys = processData("../test_input/21.txt");  // 301

        processedMonkeys["root"].operation = '-';
        long result = predict(processedMonkeys, "root", 0);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return 0;
}