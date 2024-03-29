#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>


std::vector<std::pair<std::string, int>> processData(const std::string& filename) {
    std::ifstream file(filename);

    if (!file) {
        throw std::runtime_error("Error opening the file: " + filename);
    }

    std::string line{};
    std::vector<std::pair<std::string, int>> instructions{};

    while (getline(file, line)) {
        std::string command{line.substr(0, 4)};
        int value = (command == "addx") ? std::stoi(line.substr(5)) : 0;
        instructions.emplace_back(command, value);
    }

    return instructions;
}


int calculateSignalStrengths(const std::vector<std::pair<std::string, int>>& instructions) {
    int cycle{};
    int registerX{1};
    std::vector<int> signalStrengths;

    for (const auto& [command, value] : instructions) {
        cycle++;

        if ((cycle - 20) % 40 == 0)
            signalStrengths.push_back(cycle * registerX);

        if (command == "addx") {
            cycle++;
            
            if ((cycle - 20) % 40 == 0)
                signalStrengths.push_back(cycle * registerX);
            
            registerX += value;
        }
    }
    
    return std::accumulate(signalStrengths.begin(), signalStrengths.end(), 0);
}


int main () {
    try {
        auto recievedInstructions = processData("../input/10.txt");
        // auto recievedInstructions = processData("../test_input/10.txt");  // 13140

        int result = calculateSignalStrengths(recievedInstructions);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}