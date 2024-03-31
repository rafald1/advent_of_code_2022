#include <deque>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>


std::tuple<std::vector<char>, int, int> processData(const std::string& filename) {
    std::ifstream file(filename);

    if (!file) {
        throw std::runtime_error("Error opening the file: " + filename);
    }

    std::string line{};
    std::vector<char> heightMap{};
    int rowLength{-1};
    int startNode{};
    size_t y{};

    while (getline(file, line)) {
        if (rowLength == -1) rowLength = line.size();

        for (size_t x{}; x < line.size(); ++x) {
            char currentChar = line[x];
            
            if (currentChar == 'S') {
                heightMap.push_back('a');
            } else if (currentChar == 'E') {
                heightMap.push_back('z');
                startNode = x + rowLength * y;
            } else {
                heightMap.push_back(currentChar);
            }
        }

        ++y;
    }

    return {heightMap, rowLength, startNode};
}


std::unordered_map<int, std::vector<int>> buildGraph(const std::vector<char>& heightMap, const int& rowLength) {
    std::vector<std::pair<int, int>> directions{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    std::unordered_map<int, std::vector<int>> graph{};

    for (int node{}; node < heightMap.size(); ++node) {
        graph[node] = {};

        for (auto& [dx, dy] : directions) {
            int neighbour{node + dx + dy * rowLength};
            if (neighbour >= 0 && neighbour < heightMap.size()) {
                if ((heightMap.at(node) - heightMap.at(neighbour)) <= 1) {
                    graph[node].push_back(neighbour);
                }
            }
        }
    }

    return graph;
}


std::vector<int> simplifiedDijkstra(const std::unordered_map<int, std::vector<int>>& graph, const int& startNode) {
    int inf{std::numeric_limits<int>::max()};
    std::vector<int> shortestPaths(graph.size(), inf);
    shortestPaths[startNode] = 0;
    std::deque<int> queue{startNode};

    while (!queue.empty()) {
        auto currentNode = queue.front();
        queue.pop_front();

        for (auto neighbour : graph.at(currentNode)) {
            if (shortestPaths[neighbour] == inf) {
                shortestPaths[neighbour] = shortestPaths[currentNode] + 1;
                queue.push_back(neighbour);
            }
        }
    }
    
    return shortestPaths;
}


int findShortestPathTo(const std::vector<int>& shortestPaths, const std::vector<char>& heightMap, 
                       const char& elevationLevel) {
    int minShortestPath{std::numeric_limits<int>::max()};

    for (size_t i{}; i < heightMap.size(); ++i) {
        if (heightMap[i] == elevationLevel) {
            if (shortestPaths.at(i) < minShortestPath) {
                minShortestPath = shortestPaths.at(i);
            }
        }
    }

    return minShortestPath;
}


int main() {
    try {
        auto [processedHeightMap, processedRowLength, processedStartNode] = processData("../input/12.txt");
        // auto [processedHeightMap, processedRowLength, processedStartNode] = processData("../test_input/12.txt");  // 29

        auto builtGraph = buildGraph(processedHeightMap, processedRowLength);
        auto allShortestPaths = simplifiedDijkstra(builtGraph, processedStartNode);
        int result = findShortestPathTo(allShortestPaths, processedHeightMap, 'a');
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE; 
    }

    return 0;
}