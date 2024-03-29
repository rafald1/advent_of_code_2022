#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>


std::vector<std::pair<char, int>> processData(const std::string& filename) {
    std::ifstream file(filename);

    if (!file) {
        throw std::runtime_error("Error opening the file: " + filename);
    }

    std::string line{};
    std::vector<std::pair<char, int>> motions{};
    
    while (getline(file, line)) {
        motions.push_back({line[0], std::stoi(line.substr(2))});
    }

    return motions;
}


int countTailUniquePositions(const std::vector<std::pair<char, int>>& motions) {
    struct Position {
        int x;
        int y;
    };

    Position head{0, 0};
    Position tail{0, 0};
    std::unordered_set<std::string> tailPositions{std::to_string(tail.x) + "+" + std::to_string(tail.y)};
    std::unordered_map<char, std::pair<int, int>> directions{
        {'U', {0, 1}},
        {'R', {1, 0}},
        {'D', {0, -1}},
        {'L', {-1, 0}}
    };

    for (const auto& motion : motions) {
        auto [direction, stepNumber] = motion;
        auto [dx, dy] = directions[direction];

        for (int i = 0; i < stepNumber; i++) {
            head.x += dx;
            head.y += dy;
            if (std::abs(head.x - tail.x) > 1 || std::abs(head.y - tail.y) > 1) {
                tail.x = head.x - dx;
                tail.y = head.y - dy;
                tailPositions.insert(std::to_string(tail.x) + "+" + std::to_string(tail.y));
            }
        }
    }

    return tailPositions.size();
}


int main() {
    try {
        auto seriesOfMotions = processData("../input/09.txt");
        // auto seriesOfMotions = processData("../test_input/09_1.txt");  // 13

        int result = countTailUniquePositions(seriesOfMotions);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}