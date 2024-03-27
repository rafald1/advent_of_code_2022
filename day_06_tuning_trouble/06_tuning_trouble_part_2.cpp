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

int findMarkerPosition(const std::string& datastream, const std::size_t& markerLength) {
    for (int i = 0; i < datastream.length() - markerLength; i++) {        
        std::unordered_set<char> marker(datastream.begin() + i, datastream.begin() + i + markerLength);
        
        if (marker.size() == markerLength) {
            return i + markerLength;
        }
    }

    return 0;
}

int main() {
    try {
        std::string recievedDatastream = processData("../input/06.txt");
        // std::string recievedDatastream = processData("../test_input/06.txt");  // 19

        int result = findMarkerPosition(recievedDatastream, 14);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}