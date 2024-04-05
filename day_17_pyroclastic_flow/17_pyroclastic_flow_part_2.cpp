#include <complex>
#include <cstdlib>
#include <exception>
#include <format>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>


template<class T>
struct std::hash<std::complex<T>> {
    size_t operator()(const std::complex<T>& c) const {
        size_t hash1 = std::hash<T>{}(c.real());
        size_t hash2 = std::hash<T>{}(c.imag());
        return hash1 ^ (hash2 << 1);
    }
};


template<class A, class B>
struct std::hash<std::pair<A, B>> {
   size_t operator() (const std::pair<A, B>& pair) const {
       return std::rotl(hash<A>{}(pair.first), 1) ^ hash<B>{}(pair.second);
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


int getJetDirection(const std::string& pattern, int& jetIndex) {
    int direction = pattern[jetIndex] - '<' - 1;
    jetIndex = (jetIndex + 1) % pattern.size();
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
    const std::vector<std::unordered_set<std::complex<int>>>& rocks,
    int& rockIndex
) {
    std::unordered_set<std::complex<int>> rock = rocks[rockIndex];
    rockIndex = (rockIndex + 1) % rocks.size();
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


long calculateTowerHeight(
    const std::vector<std::unordered_set<std::complex<int>>>& rocks,
    const std::string& jetPattern
) {
    std::unordered_set<std::complex<int>> tower{};
    int towerHeight{};
    std::vector<int> rockHeights{1, 3, 3, 4, 2};  // -, +, _|, |, ■
    long numberOfRocks{1'000'000'000'000};
    int rockCounter{};
    int rockIndex{};
    int jetIndex{};
    std::unordered_map<std::pair<int, int>, std::pair<int, int>> memo{};

    while(1) {
        std::pair<int, int> key{rockIndex, jetIndex};

        if (memo.count(key)) {
            auto [storedRockCounter, storedTowerHeight] = memo.at(key);
            std::ldiv_t result = std::div(numberOfRocks - rockCounter, rockCounter * 1L - storedRockCounter);
            
            if (result.rem == 0)
                return towerHeight + (towerHeight - storedTowerHeight) * result.quot;
        } else
            memo[key] = {rockCounter, towerHeight};

        std::complex<int> rockPosition{2, towerHeight + 4};
        std::unordered_set<std::complex<int>> rock = getNextRock(rocks, rockIndex);

        while (1) {
            int direction = getJetDirection(jetPattern, jetIndex);
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

        towerHeight = std::max(towerHeight, rockPosition.imag() + rockHeights[(rockIndex + 4) % 5] - 1);
        ++rockCounter;

        if (rockCounter == std::numeric_limits<int>::max()) {
            throw std::runtime_error(std::format("Failed to find cycle after {} rocks.", rockCounter));
        }
    }
}


int main() {    
    try {
        std::string jetPattern = processData("../input/17.txt");
        // std::string jetPattern = processData("../test_input/17.txt");  // 1514285714288

        std::vector<std::unordered_set<std::complex<int>>> rocks = constructRocks();
        long result = calculateTowerHeight(rocks, jetPattern);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return 0;
}