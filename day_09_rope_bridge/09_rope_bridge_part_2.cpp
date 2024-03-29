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

    Position knot{0, 0};
    std::vector<Position> knots{10, knot};
    std::unordered_set<std::string> tailPositions{"0+0"};
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
            knots[0].x += dx;
            knots[0].y += dy;
     
            for (int i = 1; i < knots.size(); i++) {
                if (std::abs(knots[i - 1].x - knots[i].x) > 1 || std::abs(knots[i - 1].y - knots[i].y) > 1) {
                    int xComparison = (knots[i - 1].x - knots[i].x);
                    int yComparison = (knots[i - 1].y - knots[i].y);

                    if (xComparison < 0)
                        knots[i].x--;
                    else if (xComparison > 0)
                        knots[i].x++;

                    if (yComparison < 0)
                        knots[i].y--;
                    else if (yComparison > 0)
                        knots[i].y++;
                }
            }

            std::string unorderedSetHash = std::to_string(knots.back().x) + "+" + std::to_string(knots.back().y);
            
            if (tailPositions.find(unorderedSetHash) == tailPositions.end()) {
                tailPositions.insert(unorderedSetHash);
            }
        }
    }

    return tailPositions.size();
}


int main() {
    try {
        auto seriesOfMotions = processData("../input/09.txt");
        // auto seriesOfMotions = processData("../test_input/09_2.txt");  // 36
        
        int result = countTailUniquePositions(seriesOfMotions);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}