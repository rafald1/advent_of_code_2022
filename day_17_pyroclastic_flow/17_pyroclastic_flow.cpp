#include <complex>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>


template<class T>
struct std::hash<std::complex<T>> {
    size_t operator()(const std::complex<T>& c) const {
        size_t hash1 = std::hash<T>{}(c.real());
        size_t hash2 = std::hash<T>{}(c.imag());
        return hash1 ^ (hash2 << 1);
    }
};


std::string processData(const std::string& filename) {
    std::ifstream file(filename);

    if (!file)
        throw std::runtime_error("Error opening the file: " + filename);

    std::string line{};
    getline(file, line);

    return line;
}


int getJetDirection(const std::string& pattern) {
    static size_t currentIndex = 0;
    int direction = pattern[currentIndex] - '<' - 1;
    currentIndex = (currentIndex + 1) % pattern.size();
    return direction;
}


std::vector<std::unordered_set<std::complex<int>>> constructRocks() {
    std::unordered_set<std::complex<int>> rock1{{0, 0}, {1, 0}, {2, 0}, {3, 0}};  // -
    std::unordered_set<std::complex<int>> rock2{{1, 0}, {0, 1}, {1, 1}, {2, 1}, {1, 2}};  // +
    std::unordered_set<std::complex<int>> rock3{{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}};  // _|
    std::unordered_set<std::complex<int>> rock4{{0, 0}, {0, 1}, {0, 2}, {0, 3}};  // |
    std::unordered_set<std::complex<int>> rock5{{0, 0}, {1, 0}, {0, 1}, {1, 1}};  // ■
    std::vector<std::unordered_set<std::complex<int>>> rocks{rock1, rock2, rock3, rock4, rock5};
    return rocks;
}


std::unordered_set<std::complex<int>> getNextRock(
    const std::vector<std::unordered_set<std::complex<int>>>& rocks
) {
    static size_t currentIndex = 0;
    std::unordered_set<std::complex<int>> rock = rocks[currentIndex];
    currentIndex = (currentIndex + 1) % rocks.size();
    return rock;
}


bool checkIfRockMovementIsPossible(
    const std::complex<int>& rockPosition, 
    const std::complex<int>& rockMovement, 
    const std::unordered_set<std::complex<int>>& rock,
    const std::unordered_set<std::complex<int>>& tower
) {
    for (auto rockElement : rock) {
        std::complex<int> newPos{rockPosition + rockMovement + rockElement};
        if (newPos.real() < 0 || newPos.real() > 6 || newPos.imag() == 0 || tower.contains(newPos))
            return false;
    }
    
    return true;
}


int calculateTowerHeight(
    const std::vector<std::unordered_set<std::complex<int>>>& rocks, 
    const std::string& jetPattern
) {
    std::unordered_set<std::complex<int>> tower{};
    int towerHeight{};
    std::vector<int> rockHeights{1, 3, 3, 4, 2};  // -, +, _|, |, ■

    for (size_t rockCounter{0}; rockCounter < 2022; ++rockCounter) {
        std::complex<int> rockPosition{2, towerHeight + 4};
        std::unordered_set<std::complex<int>> rock = getNextRock(rocks);

        while (1) {
            int direction = getJetDirection(jetPattern);
            std::complex<int> rockMovement{direction, 0};

            if (checkIfRockMovementIsPossible(rockPosition, rockMovement, rock, tower))
                rockPosition = rockPosition + rockMovement;

            rockMovement = {0, -1};

            if (checkIfRockMovementIsPossible(rockPosition, rockMovement, rock, tower))
                rockPosition = rockPosition + rockMovement;
            else
                break;
        }

        for (auto& rockElement : rock)
            tower.insert(rockElement + rockPosition);

        towerHeight = std::max(towerHeight, rockPosition.imag() + rockHeights[rockCounter % 5] - 1);
    }

    return towerHeight;
}


int main() {    
    try {
        std::string jetPattern = processData("../input/17.txt");
        // std::string jetPattern = processData("../test_input/17.txt");  // 3068

        std::vector<std::unordered_set<std::complex<int>>> rocks = constructRocks();
        int result = calculateTowerHeight(rocks, jetPattern);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return 0;
}