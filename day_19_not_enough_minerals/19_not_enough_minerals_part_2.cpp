#include <exception>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>


struct Resources {
    int ore{};
    int clay{};
    int obsidian{};
    int geode{};

    Resources operator-(const Resources& other) const {
        return {ore - other.ore, clay - other.clay, obsidian - other.obsidian, geode - other.geode};
    }

    Resources operator+(const Resources& other) const {
        return {ore + other.ore, clay + other.clay, obsidian + other.obsidian, geode + other.geode};
    }

    bool operator>=(const Resources& other) const {
        return (ore >= other.ore && clay >= other.clay & obsidian >= other.obsidian);
    }

    int& operator[](const std::string& key) {
        static const std::unordered_map<std::string, int Resources::*> resourceMap{
            {"ore", &Resources::ore},
            {"clay", &Resources::clay},
            {"obsidian", &Resources::obsidian},
            {"geode", &Resources::geode}
        };

        return this->*resourceMap.at(key);
    }
};


std::vector<std::unordered_map<std::string, Resources>> processData(const std::string& filename) {
    std::ifstream file(filename);

    if (!file)
        throw std::runtime_error("Error opening the file: " + filename);

    std::string line{};
    std::regex pattern{"[0-9]+"};
    std::vector<std::unordered_map<std::string, Resources>> blueprints{};

    while(getline(file, line)) {
        std::regex_token_iterator<std::string::iterator> matches(line.begin(), line.end(), pattern);
        std::string blueprintID = {*matches++};
        std::unordered_map<std::string, Resources> botCost{};
        botCost["ore"] = {std::stoi(*matches++)};
        botCost["clay"] = {std::stoi(*matches++)};
        botCost["obsidian"] = {std::stoi(*matches++), std::stoi(*matches++)};
        botCost["geode"] = {std::stoi(*matches++), 0, std::stoi(*matches)};

        blueprints.push_back(botCost);
        if (blueprintID == "3")
            break;
    }

    return blueprints;
}


int simulate(
    int timeLeft, 
    std::string botToBuild, 
    const std::unordered_map<std::string, Resources>& botCost, 
    Resources botCount, 
    Resources resources
) {
    bool canBotBeBuilt{false};

    if (resources >= botCost.at(botToBuild)) {
        canBotBeBuilt = true;
        resources = resources - botCost.at(botToBuild);
    }

    resources = resources + botCount;

    if (timeLeft == 1)
        return resources.geode;

    if (canBotBeBuilt) {
        ++botCount[botToBuild];
        int sim[4]{0, 0, 0, 0};
        int maxOreNeeded = std::max(botCost.at("ore").ore, std::max(botCost.at("clay").ore, 
                                    std::max(botCost.at("obsidian").ore, botCost.at("geode").ore)));
        
        if (resources.ore <= maxOreNeeded && 
            (timeLeft - 1) * botCount.ore + resources.ore < (timeLeft - 1) * maxOreNeeded)
            sim[0] = simulate(timeLeft - 1, "ore", botCost, botCount, resources);
        
        if (resources.clay <= botCost.at("obsidian").clay + 5)
            sim[1] = simulate(timeLeft - 1, "clay", botCost, botCount, resources);
        
        if (resources.obsidian < botCost.at("geode").obsidian)
            sim[2] = simulate(timeLeft - 1, "obsidian", botCost, botCount, resources);
        
        sim[3] = simulate(timeLeft - 1, "geode", botCost, botCount, resources);

        return std::max(sim[0], std::max(sim[1], std::max(sim[2], sim[3])));
    } else {
        return simulate(timeLeft - 1, botToBuild, botCost, botCount, resources);
    }
}


int useBlueprintsToSimulate(const std::vector<std::unordered_map<std::string, Resources>>& blueprints) {
    int geodeCount{1};

    for (auto& blueprint : blueprints) {
        Resources botCount{1, 0, 0, 0};
        Resources startingResources{0, 0, 0, 0};
        int sim1 = simulate(32, "ore", blueprint, botCount, startingResources);
        int sim2 = simulate(32, "clay", blueprint, botCount, startingResources);
        geodeCount *= std::max(sim1, sim2);
    }

    return geodeCount;
}


int main() {
    try {
        auto allBlueprints = processData("../input/19.txt");
        // auto allBlueprints = processData("../test_input/19.txt");  // 3472

        int result = useBlueprintsToSimulate(allBlueprints);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return 0;
}