#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>


std::string processData(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Error opening the file: " + filename);
    }

    std::string line;
    std::getline(file, line);

    return line;
}

int findMarkerPosition(const std::string& datastream) {
    for (int i = 3; i < datastream.length(); i++) {        
        std::unordered_set<char> marker(datastream.begin() + i - 3, datastream.begin() + i + 1);
        
        if (marker.size() == 4) {
            return i + 1;
        }
    }

    return 0;
}

int main() {
    try {
        std::string recievedDatastream = processData("../input/06.txt");
        // std::string recievedDatastream = processData("../test_input/06.txt");  // 7

        int result = findMarkerPosition(recievedDatastream);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return 0;
}