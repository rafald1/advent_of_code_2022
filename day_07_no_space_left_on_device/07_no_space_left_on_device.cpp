#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>


std::unordered_map<std::string, int> processData(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Error opening the file: " + filename);
    }

    std::string line{};
    std::unordered_map<std::string, int> currentDirectorySize{};
    std::vector<std::string> stack{};

    while (getline(file, line)) {
        if (std::isdigit(line[0])) {
            for (auto path : stack) {
                currentDirectorySize[path] += std::stoi(line.substr(0, line.find(' ')));
            }
        } else if (line.substr(0, 4) == "$ cd") {
            std::string directory = line.substr(5);
            
            if (directory == "..") {
                stack.pop_back();
            } else {
                stack.push_back(stack.empty() ? directory : stack.back() + "+" + directory);
            }
        }
    }

    return currentDirectorySize;
}


int countSizeOfDirectoriesSmallerThan(const std::unordered_map<std::string, int>& directorySizes,
                                      const int& maxDirectorySize) {
    int sum = 0;

    for (const auto& directorySize : directorySizes) {
        if (directorySize.second < maxDirectorySize) {
            sum += directorySize.second;
        }
    }

    return sum;
}


int main () {
    try {
        auto retrievedDirectorySizes = processData("../input/07.txt");
        // auto retrievedDirectorySizes = processData("../test_input/07.txt");  // 95437

        int result = countSizeOfDirectoriesSmallerThan(retrievedDirectorySizes, 100000);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return 0;
}