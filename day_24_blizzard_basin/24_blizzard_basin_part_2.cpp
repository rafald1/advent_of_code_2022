#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>


struct Position {
    int row{};
    int column{};

    bool operator==(Position other) const {
        return row == other.row && column == other.column;
    }
};


template <>
struct std::hash<Position> {
    size_t operator()(const Position& pos) const {
        return std::hash<int>{}(pos.row) ^ (std::hash<int>{}(pos.column) << 1);
    }
};


std::vector<std::string> processData(const std::string& filename) {
    std::ifstream file(filename);

    if (!file)
        throw std::runtime_error("Error opening the file: " + filename);

    std::string line{};
    std::vector<std::string> grid{};
    std::getline(file, line);

    while (getline(file, line))
        grid.push_back(line.substr(1, line.size() - 2));

    grid.pop_back();

    return grid;
}


bool checkIfInBounds(const Position& next, const size_t& height, const size_t& width) {
    if (next.row < 0 || next.row >= height || next.column < 0 || next.column >= width)
        return 0;
    return 1;
}


bool checkIfNoBlizardDetected(
    const size_t& rewindCounter, 
    const Position& next, 
    const std::vector<std::string>& grid, 
    const size_t& height, 
    const size_t& width
) {
    if (grid[next.row][(next.column + rewindCounter) % width] == '<' ||
        grid[next.row][(width + next.column - rewindCounter % width) % width] == '>' ||
        grid[(next.row + rewindCounter) % height][next.column] == '^' ||
        grid[(height + next.row - rewindCounter % height) % height][next.column] == 'v')
        return 0;
    return 1;
}


size_t findShortestPath(const std::vector<std::string>& grid, const Position& start, const Position& end, size_t timeCounter) {
    size_t height{grid.size()};
    size_t width{grid[0].size()};
    Position directions[5] = {{0, 0}, {-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    std::unordered_set<Position> possibleLocations{{start}};

    while (1) {
        std::unordered_set<Position> futurePossibleLocation{};
        for (auto& current : possibleLocations) {           
            if (current == end)
                return timeCounter;

            for (auto& direction : directions) {
                Position next{current.row + direction.row, current.column + direction.column};

                if (next == start || next == end || checkIfInBounds(next, height, width) && 
                    checkIfNoBlizardDetected(timeCounter + 1, next, grid, height, width)) {
                    futurePossibleLocation.insert(next);
                }
            }
        }

        possibleLocations = futurePossibleLocation;
        ++timeCounter;
    }

    throw std::runtime_error("Failed to find shortest path.");
}


int main() {
    try {
        std::vector<std::string> processedGrid = processData("../input/24.txt");
        // std::vector<std::string> processedGrid = processData("../test_input/24.txt");  // 54
        
        size_t height{processedGrid.size()};
        size_t width{processedGrid[0].size()};
        Position start{-1, 0};
        Position end{static_cast<int>(height), static_cast<int>(width) - 1};
        size_t elapsedTime{};
        elapsedTime = findShortestPath(processedGrid, start, end, elapsedTime);
        elapsedTime = findShortestPath(processedGrid, end, start, elapsedTime);
        elapsedTime = findShortestPath(processedGrid, start, end, elapsedTime);
        std::cout << elapsedTime << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return 0;
}