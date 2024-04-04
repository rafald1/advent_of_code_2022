#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>


template<class A, class B>
struct std::hash<std::pair<A, B>> {
	size_t operator() (const std::pair<A, B>& pair) const {
		return std::rotl(hash<A>{}(pair.first), 1) ^ hash<B>{}(pair.second);
	}
};


void processData(
    const std::string& filename, 
    std::vector<std::string>& valves,
    std::unordered_map<std::string, int>& flowRates, 
    std::unordered_map<std::string, std::vector<std::string>>& neighbours
) {
    std::ifstream file(filename);

    if (!file)
        throw std::runtime_error("Error opening the file: " + filename);

    std::string line{};
    std::regex pattern{"[A-Z]{2}|\\d+"};
    std::unordered_map<std::pair<std::string, int>, std::vector<std::string>> graph{};

    while(getline(file, line)) {
        std::vector<std::string> reachableValves{};
        std::sregex_iterator iter(line.begin(), line.end(), pattern);
        std::sregex_iterator end;

        valves.push_back(iter->str());
        ++iter;
        flowRates[valves.back()] = std::stoi(iter->str());
        ++iter;

        while (iter != end) {
            reachableValves.push_back(iter->str());
            ++iter;
        }

        neighbours[valves.back()] = reachableValves;
    }
}


std::unordered_map<std::pair<std::string, std::string>, int> calculateShortestPathsBetweenNodes(
    const std::vector<std::string>& valves,
    const std::unordered_map<std::string, std::vector<std::string>>& neighbours
) {  // Floyd–Warshall algorithm
    std::unordered_map<std::pair<std::string, std::string>, int> distances;
    
    for (auto& valve : valves) {
        for (auto& otherValve : valves) {
            auto it = std::find(neighbours.at(valve).begin(), neighbours.at(valve).end(), otherValve);
            
            if (it != neighbours.at(valve).end())
                distances[{valve, otherValve}] = 1;
            else
                distances[{valve, otherValve}] = (valve == otherValve) ? 0 : 999999;
        }
    }

    for (auto& valveK : valves) {
        for (auto& valveI : valves) {
            for (auto& valveJ : valves) {
                distances[{valveI, valveJ}] = std::min(
                    distances.at({valveI, valveJ}),
                    distances.at({valveI, valveK}) + distances.at({valveK, valveJ})
                );
            }
        }
    }

    return distances;
}


std::vector<std::string> findValvesAbleToReleasePressure(const std::unordered_map<std::string, int>& flowRates) {
    std::vector<std::string> reducedValves{};

    for (auto& [valve, flowRate] : flowRates) {
        if (flowRate > 0 || valve == "AA")
            reducedValves.push_back(valve);
    }

    std::sort(reducedValves.begin(), reducedValves.end());
    return reducedValves;
}


void openCurrentValveByUpdatingValveStatuses(
    const std::vector<std::string>& valves, 
    const std::string& currentValve, 
    std::string& valveStatuses
) {
    for (size_t j{}; j < valves.size(); ++j) {
        if (valves[j] == currentValve) {
            valveStatuses[j] = '1';
            break;
        }
    }    
}


bool checkIfCurrentValveIsAlreadyOpened(
    const std::vector<std::string>& valves, 
    const std::string& currentValve, 
    const std::string& valveStatuses
) {
    for (size_t i{}; i < valves.size(); ++i) {
        if (valves[i] == currentValve) {
            if (valveStatuses[i] == '1')
                return true;
            break;
        }
    }

    return false;
}


int findMaxFlow(
    const std::vector<std::string>& valves,
    const std::unordered_map<std::string, int>& flowRates,
    const std::unordered_map<std::pair<std::string, std::string>, int>& distances,
    std::unordered_map<std::string, int>& memo,
    std::string currentValveA,
    int timeLeftA,
    std::string currentValveB,
    int timeLeftB,
    std::string valveStatuses
) {
    if (timeLeftA <= 1 || timeLeftB <= 1 ||
        checkIfCurrentValveIsAlreadyOpened(valves, currentValveA, valveStatuses)) {
        return 0;
    }

    if (timeLeftA < timeLeftB) {
        std::swap(timeLeftA, timeLeftB);
        std::swap(currentValveA, currentValveB);
    }

    std::string memoID = std::to_string(timeLeftA) + currentValveA +
                         std::to_string(timeLeftB) + currentValveB + valveStatuses;

    if (memo.find(memoID) != memo.end()) {
        return memo.at(memoID);
    }

    int pressureReleased{};

    for (size_t i{1}; i < valves.size(); ++i) {
        if (currentValveA == valves[i] || currentValveB == valves[i])
            continue;

        int timeUsed{};

        if (currentValveA != "AA") {
            openCurrentValveByUpdatingValveStatuses(valves, currentValveA, valveStatuses);
            timeUsed = 1;
        } 
            
        pressureReleased = std::max(
            pressureReleased,
            findMaxFlow(valves, flowRates, distances, memo,
                        valves[i], timeLeftA - timeUsed - distances.at({currentValveA, valves[i]}),
                        currentValveB, timeLeftB, valveStatuses)
        );
    }

    if (pressureReleased == 0)
        pressureReleased = (timeLeftB - 1) * flowRates.at(currentValveB);

    memo[memoID] = pressureReleased + (timeLeftA - 1) * flowRates.at(currentValveA);
    return memo.at(memoID);
}


int main() {
    try {
        std::vector<std::string> valves{};
        std::unordered_map<std::string, int> flowRates{};
        std::unordered_map<std::string, std::vector<std::string>> neighbours{};

        processData("../input/16.txt", valves, flowRates, neighbours);
        // processData("../test_input/16.txt", valves, flowRates, neighbours);  // 1707

        auto distances = calculateShortestPathsBetweenNodes(valves, neighbours);
        valves.clear();
        valves = findValvesAbleToReleasePressure(flowRates);
        std::unordered_map<std::string, int> memo{};
        auto result = findMaxFlow(valves, flowRates, distances, memo, "AA", 26, "AA", 26, std::string(valves.size(), '0'));
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return 0;
}