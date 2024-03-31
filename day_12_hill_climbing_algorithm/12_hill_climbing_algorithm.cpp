#include <deque>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>


template<class A,class B>
struct std::hash<std::pair<A, B>> {
	size_t operator() (const std::pair<A, B>& pair) const {
		return std::rotl(hash<A>{}(pair.first), 1) ^ hash<B>{}(pair.second);
	}
};


std::tuple<std::unordered_map<std::pair<size_t, size_t>, char>, std::pair<size_t, size_t>, std::pair<size_t, size_t>>
processData(const std::string& filename) {
    std::ifstream file(filename);

    if (!file) {
        throw std::runtime_error("Error opening the file: " + filename);
    }

    std::string line{};
    size_t y{0};
    std::unordered_map<std::pair<size_t, size_t>, char> heightMap{};
    std::pair<size_t, size_t> start{};
    std::pair<size_t, size_t> end{};

    while (getline(file, line)) {
        for (size_t x{0}; x < line.size(); ++x) {
            if (line[x] == 'S') {
                start = {x, y};
                heightMap[{x, y}] = 'a';
            } else if (line[x] == 'E') {
                end = {x, y};
                heightMap[{x, y}] = 'z';
            } else {
                heightMap[{x, y}] = line[x];
            }
        }

        ++y;
    }

    return {heightMap, start, end};
}


std::unordered_map<std::pair<size_t, size_t>, std::vector<std::pair<size_t, size_t>>> 
buildGraph(std::unordered_map<std::pair<size_t, size_t>, char>& heightMap) {
    std::vector<std::pair<int, int>> directions{{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    std::unordered_map<std::pair<size_t, size_t>, std::vector<std::pair<size_t, size_t>>> graph{};

    for (const auto& [position, height] : heightMap) {
        auto& [x, y] = position;
        graph[{x, y}] = {};
        
        for (auto& [dx, dy] : directions) {
            std::pair<size_t, size_t> neighbour{x + dx, y + dy};
            if (heightMap.contains(neighbour)) {
                if ((heightMap.at(neighbour) - heightMap.at(position)) <= 1) {
                    graph[{x, y}].push_back(neighbour);
                }
            }
        }
    }

    return graph;
}


size_t calculateShortestPath(
        const std::unordered_map<std::pair<size_t, size_t>, std::pair<size_t, size_t>>& predecessors,
        const std::pair<size_t, size_t>& start,
        const std::pair<size_t, size_t>& end
    ) {
    size_t counter{0};
    std::pair<size_t, size_t> current{end};
    
    while (current != start) {
        current = predecessors.at(current);
        ++counter;
    }

    return counter;    
}


size_t findPath(const auto& graph, const auto& start, const auto& end) {
    std::unordered_set<std::pair<size_t, size_t>> visited{start};
    std::deque<std::pair<size_t, size_t>> queue{start};
    std::unordered_map<std::pair<size_t, size_t>, std::pair<size_t, size_t>> predecessors{};

    while (!queue.empty()) {
        auto currentNode = queue.front();
        
        if (currentNode == end) {
            return calculateShortestPath(predecessors, start, end);
        }

        queue.pop_front();

        for (auto neighbour : graph.at(currentNode)) {
            if (!visited.contains(neighbour)) {
                queue.push_back(neighbour);
                visited.insert(neighbour);
                predecessors[neighbour] = {currentNode};
            }
        }
    }

    return EXIT_FAILURE;
}


int main() {
    try {
        auto [processedHeightMap, startNode, endNode] = processData("../input/12.txt");
        // auto [processedHeightMap, startNode, endNode] = processData("../test_input/12.txt");  // 31
        
        auto builtGraph = buildGraph(processedHeightMap);
        size_t result = findPath(builtGraph, startNode, endNode);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE; 
    }
    
    return 0;
}