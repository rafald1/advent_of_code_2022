#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <utility>


std::vector<std::string> processData(const std::string& filename) {
    std::ifstream file(filename);
    
    if (!file) {
        throw std::runtime_error("Error opening the file: " + filename);
    }

    std::string line{};
    std::vector<std::string> forest{};

    while (getline(file, line)) {
        forest.push_back(line);
    }

    return forest;
}


int findHighestScenicScore(std::vector<std::string>& forest) {
    size_t forestHeight{forest.size()};
    size_t forestWidth{forest[0].size()};
    int highestScenicScore{};
    std::vector<std::pair<int, int>> directions{{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

    for (int y = 1; y < forestHeight - 1; y++) {
        for (int x = 1; x < forestWidth - 1; x++) {
            int currentScenicScore{1};
            
            for (auto& direction : directions) {
                auto [dy, dx] = direction;
                int i{1};
                int partialScenicScore{0};
                
                while (y + i * dy >= 0 && 
                       y + i * dy < forestHeight &&
                       x + i * dx >= 0 &&
                       x + i * dx < forestWidth) {
                    partialScenicScore++;
                    
                    if (forest[y][x] <= forest[y + i * dy][x + i * dx]) {
                        break;
                    }

                    i++;
                }

                currentScenicScore *= partialScenicScore;
            }

            highestScenicScore = std::max(highestScenicScore, currentScenicScore);
        }
    }    

    return highestScenicScore;
}


int main() {
    try {
        std::vector<std::string> forestPlan = processData("../input/08.txt");
        // std::vector<std::string> forestPlan = processData("../test_input/08.txt");  // 8

        int result = findHighestScenicScore(forestPlan);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return 0;
}