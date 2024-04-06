#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>


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
            throw std::runtime_error("Failed to extract coordinates of a cube from the processed line:\n" + line);
    }

    return lavaDroplets;
}


size_t countNotConnectedSides(const std::unordered_set<Cube>& lavaDroplets) {
    std::unordered_set<Cube> adjacencies{{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};
    size_t freeSidesCounter{lavaDroplets.size() * 6};

    for (auto& lavaDroplet : lavaDroplets) {
        for (auto& adjacency : adjacencies) {
            Cube neighbour{lavaDroplet + adjacency};
            if (lavaDroplets.contains(neighbour))
                --freeSidesCounter;
        }
    }

    return freeSidesCounter;
}


int main() {
    try {
        std::unordered_set<Cube> lavaDroplets = processData("../input/18.txt");
        // std::unordered_set<Cube> lavaDroplets = processData("../test_input/18.txt");  // 64

        size_t result = countNotConnectedSides(lavaDroplets);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return 0;
}