#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


struct Monkey {
    std::vector<int> items{};
    char operationType{};
    int operand{};
    int divisibleBy{};
    int trueOutcome{};
    int falseOutcome{};
    int inspectionCounter{};
};


std::vector<Monkey> processData(const std::string& filename) {
    std::ifstream file(filename);
    
    if (!file) {
        throw std::runtime_error("Error opening the file: " + filename);
    }

    std::string line;
    std::vector<Monkey> monkeys{};

    while (getline(file, line)) {
        if (line.starts_with("  Starting items:")) {
            Monkey monkey;
            
            line = line.substr(line.find(":") + 2);
            std::erase(line, ',');
            std::istringstream iss(line);
            int number;        
            while (iss >> number) {
                monkey.items.push_back(number);
            }

            getline(file, line);
            auto operationType = std::find_if(line.rbegin(), line.rend(), [](char c) {
                return c == '*' || c == '+';
            });
            std::string operand = std::string{operationType.base() + 1, line.end()};
            monkey.operationType = (operand == "old") ? '^' : *operationType;
            monkey.operand = (operand == "old") ? 0 : std::stoi(operand);

            getline(file, line);
            auto digitPosition = std::find_if(line.begin(), line.end(), ::isdigit);
            monkey.divisibleBy = std::stoi(std::string{digitPosition, line.end()});

            getline(file, line);
            digitPosition = std::find_if(line.begin(), line.end(), ::isdigit);
            monkey.trueOutcome = std::stoi(std::string{digitPosition, line.end()});

            getline(file, line);
            digitPosition = std::find_if(line.begin(), line.end(), ::isdigit);
            monkey.falseOutcome = std::stoi(std::string{digitPosition, line.end()});

            monkeys.push_back(monkey);
        }
    }

    return monkeys;
}


int countMonkeyInteractionWithItems(std::vector<Monkey>& monkeys, const size_t& roundNumber) {
    for (size_t i{0}; i < roundNumber; ++i) {
        for (auto& monkey : monkeys) {
            for (auto& item : monkey.items) {
                switch (monkey.operationType) {
                    case '+':
                        item += monkey.operand;
                        break;
                    case '*':
                        item *= monkey.operand;
                        break;
                    case '^':
                        item *= item;
                        break;
                }
                ++monkey.inspectionCounter;
                item /= 3;
                if (item % monkey.divisibleBy == 0) {
                    monkeys[monkey.trueOutcome].items.push_back(item);
                } else {
                    monkeys[monkey.falseOutcome].items.push_back(item);
                }            
            }
            monkey.items = {};
        }
    }

    std::vector<int> inspectionCounters{};

    for (const auto& monkey : monkeys) {
        inspectionCounters.push_back(monkey.inspectionCounter);
    }

    std::sort(inspectionCounters.rbegin(), inspectionCounters.rend());
    
    return inspectionCounters[0] * inspectionCounters[1];
}


int main() {
    try {
        std::vector<Monkey> monkeyDetails = processData("../input/11.txt");
        // std::vector<Monkey> monkeyDetails = processData("../test_input/11.txt");  // 10605
        
        int result = countMonkeyInteractionWithItems(monkeyDetails, 20);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}