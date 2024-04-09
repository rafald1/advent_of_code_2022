#include <complex>
#include <exception>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>
#include <unordered_set>


template<class T>
struct std::hash<std::complex<T>> {
    size_t operator()(const std::complex<T>& complex) const {
        return std::hash<T>{}(complex.real()) ^ (std::hash<T>{}(complex.imag()) << 1);
    }
};


std::unordered_set<std::complex<int>> processData(const std::string& filename) {
    std::ifstream file(filename);

    if (!file)
        throw std::runtime_error("Error opening the file: " + filename);

    std::string line{};
    std::unordered_set<std::complex<int>> elfPositions{};

    for (int y{}; getline(file, line); ++y) {
        for (int x{}; x < line.size(); ++x)
            if (line[x] == '#')
                elfPositions.insert(std::complex<int>{x, y});
    }

    return elfPositions;
}


std::complex<int> chooseDirection(
    const std::unordered_set<std::complex<int>>& elfPositions,
    const std::complex<int>& elfPosition,
    const size_t& round
) {
    std::unordered_map<std::string, std::complex<int>> directions{
        {"N", {0, -1}}, {"NE", {1, -1}}, {"E", {1, 0}}, {"SE", {1, 1}}, 
        {"S", {0, 1}}, {"SW", {-1, 1}}, {"W", {-1, 0}}, {"NW", {-1, -1}}
    };

    std::unordered_map<std::string, bool> neighbours{};

    for (const auto& [label, direction] : directions)
        neighbours[label] = elfPositions.count(elfPosition + direction);

    int neighboursCount = std::count_if(neighbours.begin(), neighbours.end(), [](const auto& pair) {
        return pair.second;
    });

    if (neighboursCount == 0)
        return std::complex<int>{0, 0};


    std::vector<char> checkOrders{'N', 'S', 'W', 'E'};

    for (size_t i{}; i < 4; ++i) {
        char currentCheck = checkOrders[(round + i) % 4];

        if (currentCheck == 'N')
            if (!neighbours["NW"] && !neighbours["N"] && !neighbours["NE"])
                return directions["N"];

        if (currentCheck == 'S')
            if (!neighbours["SW"] && !neighbours["S"] && !neighbours["SE"])
                return directions["S"];

        if (currentCheck == 'W')
            if (!neighbours["NW"] && !neighbours["W"] && !neighbours["SW"])
                return directions["W"];

        if (currentCheck == 'E')
            if (!neighbours["NE"] && !neighbours["E"] && !neighbours["SE"])
                return directions["E"];

    }

    return std::complex<int>{0, 0};
}


int calculateEmptyGround(std::unordered_set<std::complex<int>>& elfPositions) {
    int min = std::numeric_limits<int>::min();
    int max = std::numeric_limits<int>::max();
    int minX{max}, minY{max}, maxX{min}, maxY{min};

    for (const auto& elfPosition : elfPositions) {
        minX = std::min(minX, elfPosition.real());
        minY = std::min(minY, elfPosition.imag());
        maxX = std::max(maxX, elfPosition.real());
        maxY = std::max(maxY, elfPosition.imag());
    }

    return (maxX - minX + 1) * (maxY - minY + 1) - elfPositions.size();
}


int moveElves(std::unordered_set<std::complex<int>>& elfPositions, size_t numberOfRounds) {
    for (size_t round{}; round < numberOfRounds; ++round) {
        std::unordered_map<std::complex<int>, std::vector<std::complex<int>>> propossedMoves{};
        
        for (const auto& elfPosition : elfPositions) {
            std::complex<int> propossedMove = chooseDirection(elfPositions, elfPosition, round);
            if (propossedMove != std::complex<int>{0, 0})
                propossedMoves[elfPosition + propossedMove].push_back(elfPosition);
        }

        for (auto& [elfDestination, originalElfPositions] : propossedMoves) {
            if (originalElfPositions.size() == 1) {
                elfPositions.erase(originalElfPositions.front());
                elfPositions.insert(elfDestination);
            }
        } 
    }

    return calculateEmptyGround(elfPositions);
}


int main() {
    try {
        std::unordered_set<std::complex<int>> elves = processData("../input/23.txt");
        // std::unordered_set<std::complex<int>> elves = processData("../test_input/23.txt");  // 110

        int result = moveElves(elves, 10);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return 0;
}