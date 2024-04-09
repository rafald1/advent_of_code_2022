#include <exception>
#include <fstream>
#include <iostream>
#include <limits>
#include <regex>
#include <string>
#include <unordered_map>


template<class A, class B>
struct std::hash<std::pair<A, B>> {
	size_t operator() (const std::pair<A, B>& pair) const {
		return std::rotl(hash<A>{}(pair.first), 1) ^ hash<B>{}(pair.second);
	}
};


enum class Command { rotateLeft, rotateRight, moveForward };


struct Instruction {
    Command commamd{};
    int value{};
};


std::pair<std::unordered_map<std::pair<int, int>, int>, std::vector<Instruction>>
processData(const std::string& filename) {
    std::ifstream file(filename);

    if (!file)
        throw std::runtime_error("Error opening the file: " + filename);

    std::string line{};
    std::unordered_map<std::pair<int, int>, int> monkeyMap{};

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


void expandMapByMinMaxValues(std::unordered_map<std::pair<int, int>, int>& monkeyMap) {
    int min = std::numeric_limits<int>::min();
    int max = std::numeric_limits<int>::max();
    std::unordered_map<std::pair<int, int>, int> minMaxMapValues{};
    
    for (auto& [key, _] : monkeyMap) {
        auto [x, y] = key;

        if (minMaxMapValues.count({x, min}))
            minMaxMapValues[{x, min}] = std::min(y, minMaxMapValues[{x, min}]);
        else
            minMaxMapValues[{x, min}] = y;

        if (minMaxMapValues.count({x, max}))
            minMaxMapValues[{x, max}] = std::max(y, minMaxMapValues[{x, max}]);
        else
            minMaxMapValues[{x, max}] = y;

        if (minMaxMapValues.count({min, y}))
            minMaxMapValues[{min, y}] = std::min(x, minMaxMapValues[{min, y}]);
        else
            minMaxMapValues[{min, y}] = x;

        if (minMaxMapValues.count({max, y}))
            minMaxMapValues[{max, y}] = std::max(x, minMaxMapValues[{max, y}]);
        else
            minMaxMapValues[{max, y}] = x;
    }

    monkeyMap.insert(minMaxMapValues.begin(), minMaxMapValues.end());
}


std::pair<int, int> wrapAround(
    const std::unordered_map<std::pair<int, int>, int>& monkeyMap,
    const std::pair<int, int>& currentPosition,
    const std::pair<int, int>& direction
) {
    int min = std::numeric_limits<int>::min();
    int max = std::numeric_limits<int>::max();
    auto [x, y] = currentPosition;
    auto [dx, dy] = direction;

    if (dx == 0) {
        if (dy == -1)
            y = monkeyMap.at({x, max});
        else if (dy == 1)
            y = monkeyMap.at({x, min});
    } else if (dy == 0) {
        if (dx == -1)
            x = monkeyMap.at({max, y});
        else if (dx == 1)
            x = monkeyMap.at({min, y});
    }

    return {x, y};
}


std::pair<int, int> rotateRight(const std::pair<int, int>& direction) {
    std::vector<std::pair<int, int>> directions{{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
    auto it = std::find(directions.begin(), directions.end(), direction);
    size_t nextIndex = (std::distance(directions.begin(), it) + 1) % directions.size();
    
    return directions[nextIndex];
}


std::pair<int, int> rotateLeft(const std::pair<int, int>& direction) {
    std::vector<std::pair<int, int>> directions{{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
    auto it = std::find(directions.begin(), directions.end(), direction);
    size_t previousIndex = (directions.size() + std::distance(directions.begin(), it) - 1) % directions.size();
    
    return directions[previousIndex];
}


int calculateDirectionValue(const std::pair<int, int>& direction) {
    auto& [dx, dy] = direction;
    return (dx < 0) ? 2 : (dx > 0) ? 0 : (dy < 0) ? 3 : 1;
}


int traverse(
    const std::unordered_map<std::pair<int, int>, int>& monkeyMap,
    const std::vector<Instruction>& instructions,
    const std::pair<int, int>& startingPosition
) {
    std::pair<int, int> direction{1, 0};
    auto [x, y] = startingPosition;

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
                    auto [dx, dy] = direction;
                    std::pair<int, int> nextPosition{x + dx, y + dy};
                    
                    if (!monkeyMap.contains(nextPosition))
                        nextPosition = wrapAround(monkeyMap, {x, y}, direction);
                    
                    if (monkeyMap.at(nextPosition) == '#')
                        break;

                    x = nextPosition.first;
                    y = nextPosition.second;
                }

                break;
        }
    }

    return 1000 * y + 4 * x + calculateDirectionValue(direction);
}


int main() {
    try {
        auto [receivedMap, receivedInstructions] = processData("../input/22.txt");
        // auto [receivedMap, receivedInstructions] = processData("../test_input/22.txt");  // 6032

        expandMapByMinMaxValues(receivedMap);
        std::pair<int, int> startingPosition{receivedMap.at({std::numeric_limits<int>::min(), 1}), 1};
        int result = traverse(receivedMap, receivedInstructions, startingPosition);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return 0;
}