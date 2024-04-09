#include <complex>
#include <exception>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_set>


template<class T>
struct std::hash<std::complex<T>> {
    size_t operator()(const std::complex<T>& complex) const {
        return std::hash<T>{}(complex.real()) ^ (std::hash<T>{}(complex.imag()) << sizeof(T));
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
    enum Direction { N = 0, NE = 1, E = 2, SE = 3, S = 4, SW = 5, W = 6, NW = 7 };

    std::complex<int> directions[8]{{0, -1}, {1, -1}, {1, 0}, {1, 1}, 
                                    {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}};

    bool neighbours[8]{};

    int neighboursCounter{};

    for (size_t i{}; i < 8; ++i) {
        if (elfPositions.count(elfPosition + directions[i])) {
            neighbours[i] = 1;
            ++neighboursCounter;
        }
    }

    if (neighboursCounter == 0)
        return std::complex<int>{0, 0};

    int checkOrders[4]{N, S, W, E};

    for (size_t i{}; i < 4; ++i) {
        char currentCheck = checkOrders[(round + i) % 4];

        if (currentCheck == N)
            if (!neighbours[NW] && !neighbours[N] && !neighbours[NE])
                return directions[N];

        if (currentCheck == S)
            if (!neighbours[SE] && !neighbours[S] && !neighbours[SW])
                return directions[S];

        if (currentCheck == W)
            if (!neighbours[SW] && !neighbours[W] && !neighbours[NW])
                return directions[W];

        if (currentCheck == E)
            if (!neighbours[NE] && !neighbours[E] && !neighbours[SE])
                return directions[E];
    }

    return std::complex<int>{0, 0};
}


int moveElves(std::unordered_set<std::complex<int>>& elfPositions) {
    size_t round{};
    
    while(1) {
        std::unordered_map<std::complex<int>, std::complex<int>> propossedMoves{};
        
        for (const auto& elfPosition : elfPositions) {
            std::complex<int> propossedMove = chooseDirection(elfPositions, elfPosition, round);
            if (propossedMove != std::complex<int>{0, 0}) {
                if (propossedMoves.count(elfPosition + propossedMove))
                    propossedMoves.erase(elfPosition + propossedMove);
                else
                    propossedMoves[elfPosition + propossedMove] = elfPosition;
            }
        }

        if (propossedMoves.size() == 0)
            return round + 1;

        for (auto& [elfDestination, originalElfPositions] : propossedMoves) {
            elfPositions.erase(originalElfPositions);
            elfPositions.insert(elfDestination);
        } 
    
        ++round;
    }
}


int main() {
    try {
        std::unordered_set<std::complex<int>> elves = processData("../input/23.txt");
        // std::unordered_set<std::complex<int>> elves = processData("../test_input/23.txt");  // 20

        int result = moveElves(elves);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return 0;
}