#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>


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


size_t countVisibleTrees(std::vector<std::string>& forest) {
    size_t forestHeight{forest.size()};
    size_t forestWidth{forest[0].size()};
    size_t visibleEdgeTrees{2 * forestHeight + 2 * forestWidth  - 4};
    std::vector<int> visibilityTracker(forestHeight * forestWidth, 0);

    for (int rowIndex = 1; rowIndex < forestHeight - 1; rowIndex++) {
        char maxLeftTreeHeight{forest[rowIndex][0]};
        char maxRightTreeHeight{forest[rowIndex][forestWidth - 1]};

        for (int columnIndex = 1; columnIndex < forestWidth - 1; columnIndex++) {
            if (maxLeftTreeHeight < forest[rowIndex][columnIndex]) {
                maxLeftTreeHeight = forest[rowIndex][columnIndex];
                visibilityTracker[rowIndex * forestWidth + columnIndex] = 1;
            }
            if (maxRightTreeHeight < forest[rowIndex][forestWidth - 1 - columnIndex]) {
                maxRightTreeHeight = forest[rowIndex][forestWidth - 1 - columnIndex];
                visibilityTracker[rowIndex * forestWidth + forestWidth - 1 - columnIndex] = 1;
            }
        }
    }    

    for (int columnIndex = 1; columnIndex < forestWidth - 1; columnIndex++) {
        char maxTopTreeHeight{forest[0][columnIndex]};
        char maxBottomTreeHeight{forest[forestHeight - 1][columnIndex]};

        for (int rowIndex = 1; rowIndex < forestHeight - 1; rowIndex++) {
            if (maxTopTreeHeight < forest[rowIndex][columnIndex]) {
                maxTopTreeHeight = forest[rowIndex][columnIndex];
                visibilityTracker[rowIndex * forestWidth + columnIndex] = 1;
            }
            if (maxBottomTreeHeight < forest[forestHeight - 1 - rowIndex][columnIndex]) {
                maxBottomTreeHeight = forest[forestHeight - 1 - rowIndex][columnIndex];
                visibilityTracker[(forestHeight - 1 - rowIndex) * forestWidth + columnIndex] = 1;
            }
        }
    }

    return std::accumulate(visibilityTracker.begin(), visibilityTracker.end(), 0) + visibleEdgeTrees;
}

int main() {
    try {
        std::vector<std::string> forestPlan = processData("../input/08.txt");
        // std::vector<std::string> forestPlan = processData("../test_input/08.txt");  // 21

        size_t result = countVisibleTrees(forestPlan);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return 0;
}