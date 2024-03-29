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


std::string determinePixelStates(const std::vector<std::pair<std::string, int>>& instructions) {
    int cycle{-1};
    int registerX{1};
    std::string pixels(240, ' ');

    for (const auto& [command, value] : instructions) {
        cycle++;
        pixels[cycle] = (cycle % 40 >= registerX - 1 && cycle % 40 <= registerX + 1) ? '#' : '.';

        if (command == "addx") {
            cycle++;
            pixels[cycle] = (cycle % 40 >= registerX - 1 && cycle % 40 <= registerX + 1) ? '#' : '.';
            registerX += value;
        }
    }

    return pixels;
}


void drawPixels(const std::string& pixels) {
    for (int i = 0; i < pixels.length(); i++) {
        std::cout << pixels[i];
        
        if ((i + 1) % 40 == 0)
            std::cout << std::endl;
    }    
}


int main () {
    try {
        auto recievedInstructions = processData("../input/10.txt");
        // auto recievedInstructions = processData("../test_input/10.txt");
        // ##..##..##..##..##..##..##..##..##..##..
        // ###...###...###...###...###...###...###.
        // ####....####....####....####....####....
        // #####.....#####.....#####.....#####.....
        // ######......######......######......####
        // #######.......#######.......#######.....

        std::string pixelStates = determinePixelStates(recievedInstructions);
        drawPixels(pixelStates);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}