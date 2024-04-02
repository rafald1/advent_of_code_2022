#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <regex>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>


template<class A,class B>
struct std::hash<std::pair<A, B>> {
	size_t operator() (const std::pair<A, B>& pair) const {
		return std::rotl(hash<A>{}(pair.first), 1) ^ hash<B>{}(pair.second);
	}
};


std::vector<std::vector<std::pair<int, int>>> processData(const std::string& filename) {
    std::ifstream file(filename);

    if (!file) {
        throw std::runtime_error("Error opening the file: " + filename);
    }

    std::string line{};
    std::vector<std::vector<std::pair<int, int>>> paths{};
    std::regex pattern{"(\\d+),(\\d+)"};
    std::smatch matches{};

    while (getline(file, line)) {
        auto iter = line.cbegin();
        std::vector<std::pair<int, int>> ranges{};
        
        while (std::regex_search(iter, line.cend(), matches, pattern)) {
            int x{std::stoi(matches[1])};
            int y{std::stoi(matches[2])};
            ranges.emplace_back(x, y);
            iter = matches.suffix().first;
        }

        paths.push_back(ranges);
    }

    return paths;
}


std::unordered_set<std::pair<int, int>> constructCave(const std::vector<std::vector<std::pair<int, int>>>& caveScan) {
    std::unordered_set<std::pair<int, int>> cave{};

    for (auto path : caveScan) {
        auto [pathFromX, pathFromY] = path[0];
        cave.emplace(pathFromX, pathFromY);

        for (auto it = std::next(path.begin()); it != path.end(); ++it) {
            auto [pathToX, pathToY] = *it;
            int dx{pathToX - pathFromX};
            int dy{pathToY - pathFromY};
            int stepX = (dx > 0) ? 1 : (dx < 0) ? -1 : 0;
            int stepY = (dy > 0) ? 1 : (dy < 0) ? -1 : 0;

            while (pathFromX != pathToX || pathFromY != pathToY) {
                pathFromX += stepX;
                pathFromY += stepY;
                cave.emplace(pathFromX, pathFromY);
            }
        }
    }

    return cave;
}


size_t pourSand(std::unordered_set<std::pair<int, int>>& cave) {
    size_t caveDepth{};

    for (const auto& rockPosition : cave) {
        if (rockPosition.second > caveDepth)
            caveDepth = rockPosition.second;
    }

    size_t numberOfRocks{cave.size()};
    std::pair<int, int> sandPosition{500, 0};

    while (sandPosition.second < caveDepth) {
        if (!cave.contains({sandPosition.first, sandPosition.second + 1})) {
            ++sandPosition.second;
        } else if (!cave.contains({sandPosition.first - 1, sandPosition.second + 1})) {
            --sandPosition.first;
            ++sandPosition.second;
        } else if (!cave.contains({sandPosition.first + 1, sandPosition.second + 1})) {
            ++sandPosition.first;
            ++sandPosition.second;
        } else {
            cave.insert(sandPosition);
            sandPosition = {500, 0};
        }
    }

    size_t numberOfRocksAndSand{cave.size()};
    return numberOfRocksAndSand - numberOfRocks;
}


int main() {
    try {
        std::vector<std::vector<std::pair<int, int>>> allPaths = processData("../input/14.txt");
        // std::vector<std::vector<std::pair<int, int>>> allPaths = processData("../test_input/14.txt");  // 24

        std::unordered_set<std::pair<int, int>> constructedCave = constructCave(allPaths);
        size_t result = pourSand(constructedCave);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}