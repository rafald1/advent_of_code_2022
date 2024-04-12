#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <limits>
#include <regex>
#include <string>
#include <unordered_map>


struct Position {
    int x{};
    int y{};

    Position operator+(const Position& other) const {
        return {x + other.x, y + other.y};
    }

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};


template<>
struct std::hash<Position> {
	size_t operator() (const Position& position) const {
		return std::rotl(hash<int>{}(position.x), 1) ^ hash<int>{}(position.y);
	}
};


template <typename T>
struct std::hash<std::pair<T, Position>> {
    std::size_t operator()(const std::pair<T, Position>& pair) const {
        return std::hash<T>{}(pair.first) ^ (std::hash<Position>{}(pair.second) << 1);
    }
};


enum class Command { rotateLeft, rotateRight, moveForward };


struct Instruction {
    Command commamd{};
    int value{};
};


std::pair<std::unordered_map<Position, int>, std::vector<Instruction>> processData(const std::string& filename) {
    std::ifstream file(filename);

    if (!file)
        throw std::runtime_error("Error opening the file: " + filename);

    std::string line{};
    std::unordered_map<Position, int> monkeyMap{};

    for (int y{}; getline(file, line); ++y) {
        if (line.empty())
            break;
        
        for (int x{}; x < line.size(); ++x) {
            char currentChar = line[x];
            
            if (currentChar != ' ')
                monkeyMap[{x + 1, y + 1}] = currentChar;
        }
    }

    getline(file, line);
    std::regex pattern("\\d+|[LR]");
    std::regex_iterator<std::string::iterator> it(line.begin(), line.end(), pattern);
    std::regex_iterator<std::string::iterator> end{};
    std::vector<Instruction> instructions{};

    while (it != end) {
        std::string value = it->str();

        if (value == "L")
            instructions.push_back({Command::rotateLeft});
        else if (value == "R")
            instructions.push_back({Command::rotateRight});
        else
            instructions.push_back({Command::moveForward, std::stoi(value)});

        ++it;
    }

    return {monkeyMap, instructions};
}


Position findStartingPosition(const std::unordered_map<Position, int>& monkeyMap) {
    auto smallestX = std::min_element(
        monkeyMap.begin(), monkeyMap.end(),
        [](const auto& lhsPair, const auto& rhsPair) {
            return lhsPair.first.y == 1 && lhsPair.first.x < rhsPair.first.x;
        }
    );

    return {smallestX->first.x, 1};
}


int rotateRight(const int& direction) {
    return (direction + 1) % 4;
}


int rotateLeft(const int& direction) {
    return (4 + direction - 1) % 4;
}


std::vector<std::pair<int, Position>> buildEdgeList(
    const std::unordered_map<Position, int>& monkeyMap, 
    const Position& start
) {
    std::vector<Position> directions{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    Position currentPos{start};
    int direction{0};
    std::vector<std::pair<int, Position>> edgePointsWithDirection{{direction, currentPos}};

    do {
        Position nextForward{currentPos + directions[direction]};
        Position nextDiagonally{nextForward + directions[rotateLeft(direction)]};

        if (monkeyMap.contains(nextForward)) {
            if (!monkeyMap.contains(nextDiagonally)) {
                currentPos = nextForward;
            } else {
                direction = rotateLeft(direction);
                currentPos = nextDiagonally;
            }
        } else {
            direction = rotateRight(direction);
        }

        edgePointsWithDirection.emplace_back(direction, currentPos);
    }
    while (currentPos != start);

    return edgePointsWithDirection;
}


size_t calculateMinimumEdgeLength(const std::vector<std::pair<int, Position>>& edgeList) {
    size_t edgeLengthCounter{};
    int prevDirection{edgeList.front().first};
    size_t minEdgeLength{std::numeric_limits<size_t>::max()};
    
    for (auto& [direction, _] : edgeList) {
        if (direction == prevDirection) {
            ++edgeLengthCounter;
        } else {
            minEdgeLength = std::min(minEdgeLength, edgeLengthCounter);
            edgeLengthCounter = 1;
        }

        prevDirection = direction;
    }

    return minEdgeLength;
}


std::vector<std::pair<int, std::vector<std::pair<int, Position>>>> splitIntoSeparateEdges(
    const std::vector<std::pair<int, Position>>& edgeMap, 
    const int& edgeLength
) {
    std::vector<std::pair<int, std::vector<std::pair<int, Position>>>> separateEdges{};

    for (size_t i{}; i < edgeMap.size() / edgeLength; ++i) {
        auto& [direction, _] = edgeMap[i * edgeLength];
        std::pair<int, std::vector<std::pair<int, Position>>> separateEdge{direction, {}};

        for (size_t j{}; j < edgeLength; ++j) {
            auto& [_, position] = edgeMap[i * edgeLength + j];
            separateEdge.second.emplace_back(direction, position);
        }

        separateEdges.push_back(separateEdge);
    }

    return separateEdges;
}


std::vector<std::pair<std::vector<std::pair<int, Position>>, std::vector<std::pair<int, Position>>>>
findEdgePairs(std::vector<std::pair<int, std::vector<std::pair<int, Position>>>> separateEdges) {
    std::vector<std::pair<std::vector<std::pair<int, Position>>, std::vector<std::pair<int, Position>>>> edgePairs{};
    size_t i{};

    while (!separateEdges.empty()) {
        auto& [direction, edgePointsWithDirection] = separateEdges[i];
        auto& [otherDirection, otherEdgePointsWithDirection] = separateEdges[i + 1];
        
        if ((4 + direction - otherDirection) % 4 == 1) {
            edgePairs.emplace_back(edgePointsWithDirection, otherEdgePointsWithDirection);
            separateEdges.erase(separateEdges.begin() + i, separateEdges.begin() + i + 2);
                            
            for (size_t j{i}; j < separateEdges.size(); ++j) {
                auto& [direction, _] = separateEdges[j];
                direction = rotateLeft(direction);
            }
        } else {
            ++i;
        }

        if (i > separateEdges.size() - 2)
            i = 0;
    }   

    return edgePairs;
}


std::unordered_map<std::pair<int, Position>, std::pair<int, Position>> joinMatchingEdgePointsOnPairedEdges(
    const std::vector<std::pair<std::vector<std::pair<int, Position>>, std::vector<std::pair<int, Position>>>>& edgePairs
) {
    std::unordered_map<std::pair<int, Position>, std::pair<int, Position>> wrapMap{};

    for (const auto& [pair, otherPair] : edgePairs) {
        size_t edgeLength{pair.size()};

        for (size_t i{}; i < edgeLength; ++i) {
            const auto& [direction, position] = pair.at(i);
            const auto& [otherDirection, otherPosition] = otherPair.at(edgeLength - 1 - i);
            wrapMap[{rotateLeft(direction), position}] = {rotateRight(otherDirection), otherPosition};
            wrapMap[{rotateLeft(otherDirection), otherPosition}] = {rotateRight(direction), position};
        }
    }

    return wrapMap;
}


int traverse(
    const std::unordered_map<Position, int>& monkeyMap,
    const std::vector<Instruction>& instructions,
    const Position& startingPosition,
    const std::unordered_map<std::pair<int, Position>, std::pair<int, Position>> wrapMap
) {
    std::vector<Position> directions{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
    int direction{0};
    Position currentPos = startingPosition;

    for (auto& instruction : instructions) {
        auto [command, value] = instruction;
        
        switch (command) {
            case Command::rotateLeft:
                direction = rotateLeft(direction);
                break;
            case Command::rotateRight:
                direction = rotateRight(direction);
                break;
            case Command::moveForward:
                for (int i{}; i < value; ++i) {
                    Position nextPosition{currentPos + directions[direction]};
                    int nextDirection{direction};
                    
                    if (wrapMap.contains({direction, currentPos}))
                        std::tie(nextDirection, nextPosition) = wrapMap.at({direction, currentPos});

                    if (monkeyMap.at(nextPosition) == '#')
                        break;

                    currentPos = nextPosition;
                    direction = nextDirection;
                }

                // break;
        }
    }

    return 1000 * currentPos.y + 4 * currentPos.x + direction;
}


int main() {
    try {
        auto [receivedMap, receivedInstructions] = processData("../input/22.txt");
        // auto [receivedMap, receivedInstructions] = processData("../test_input/22.txt");  // 5031

        Position startingPosition{findStartingPosition(receivedMap)};

        auto edgeList = buildEdgeList(receivedMap, startingPosition);
        size_t edgeLength = calculateMinimumEdgeLength(edgeList);
        auto separatedEdges = splitIntoSeparateEdges(edgeList, edgeLength);
        auto pairedEdges = findEdgePairs(separatedEdges);
        auto wrapMap = joinMatchingEdgePointsOnPairedEdges(pairedEdges);

        int result = traverse(receivedMap, receivedInstructions, startingPosition, wrapMap);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return 0;
}