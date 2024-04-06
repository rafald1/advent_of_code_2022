#include <climits>
#include <exception>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <utility>


struct Cube {
    int x{};
    int y{};
    int z{};

    bool operator==(const Cube& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    Cube operator+(const Cube& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }
};


template <>
struct std::hash<Cube> {
    size_t operator()(const Cube& c) const {
        size_t hash_x = std::hash<int>{}(c.x);
        size_t hash_y = std::hash<int>{}(c.y);
        size_t hash_z = std::hash<int>{}(c.z);
        return hash_x ^ (hash_y << 1) ^ (hash_z << 2);
    }
};


std::unordered_set<Cube> processData(const std::string& filename) {
    std::ifstream file(filename);

    if (!file)
        throw std::runtime_error("Error opening the file: " + filename);

    std::string line{};
    std::unordered_set<Cube> lavaDroplets{};

    while(getline(file, line)) {
        int x{}, y{}, z{};

        if (std::sscanf(line.c_str(), "%d,%d,%d", &x, &y, &z) == 3)
            lavaDroplets.insert(Cube{x, y, z});
        else
            throw std::runtime_error("Failed to extract coordinates of a droplet from the processed line:\n" + line);
    }

    return lavaDroplets;
}


std::pair<Cube, Cube> findMinimumAndMaximum(const std::unordered_set<Cube>& lavaDroplets) {
    Cube minPos{INT_MAX, INT_MAX, INT_MAX};
    Cube maxPos{INT_MIN, INT_MIN, INT_MIN};

    for (auto& lavaDroplet : lavaDroplets) {
        minPos = {std::min(minPos.x, lavaDroplet.x), 
                  std::min(minPos.y, lavaDroplet.y), 
                  std::min(minPos.z, lavaDroplet.z)};
        maxPos = {std::max(maxPos.x, lavaDroplet.x), 
                  std::max(maxPos.y, lavaDroplet.y), 
                  std::max(maxPos.z, lavaDroplet.z)};
    }

    return {minPos, maxPos};
}


std::unordered_set<Cube> buildOutsideMap(const std::unordered_set<Cube>& lavaDroplets) {
    auto [minPos, maxPos] = findMinimumAndMaximum(lavaDroplets);
    std::unordered_set<Cube> outside{};
    std::queue<Cube> queue{{minPos + Cube{-1, 0, 0}}};
    std::unordered_set<Cube> adjacencies{{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};

    while(!queue.empty()) {
        Cube currentCube = queue.front();
        queue.pop();

        for (auto& adjacency : adjacencies) {
            Cube neighbour{currentCube + adjacency};

            if (neighbour.x < minPos.x - 1 || neighbour.x > maxPos.x + 1 ||
                neighbour.y < minPos.y - 1 || neighbour.y > maxPos.y + 1 ||
                neighbour.z < minPos.z - 1 || neighbour.z > maxPos.z + 1 ||
                lavaDroplets.contains(neighbour) || outside.contains(neighbour)) {
                continue;
            }

            queue.push(neighbour);
            outside.insert(neighbour);
        }
    }

    return outside;
}


size_t countExternalSides(const std::unordered_set<Cube>& lavaDroplets, const std::unordered_set<Cube>& outside) {
    std::unordered_set<Cube> adjacencies{{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};
    size_t counter{};

    for (auto& lavaDroplet : lavaDroplets) {
        for (auto& adjacency : adjacencies) {
            Cube neighbour{lavaDroplet + adjacency};
            if (outside.contains(neighbour))
                ++counter;
        }
    }

    return counter;
}


int main() {
    try {
        std::unordered_set<Cube> allLavaDroplets = processData("../input/18.txt");
        // std::unordered_set<Cube> allLavaDroplets = processData("../test_input/18.txt");  // 58

        std::unordered_set<Cube> outsideMap = buildOutsideMap(allLavaDroplets);
        size_t result = countExternalSides(allLavaDroplets, outsideMap);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return 0;
}