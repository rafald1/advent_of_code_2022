#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


struct IndexedValue {
    int index{};
    int value{};
};


std::vector<IndexedValue> processData(const std::string& filename) {
    std::ifstream file(filename);

    if (!file)
        throw std::runtime_error("Error opening the file: " + filename);

    std::string line{};
    std::vector<IndexedValue> numbers{};
    int originalIndex{};

    while(getline(file, line)) {
        numbers.push_back({originalIndex, std::stoi(line)});
        ++originalIndex;
    }

    return numbers;
}


size_t calculateNewPositon(
    const std::ptrdiff_t& originalPosition, 
    const int& shiftValue, 
    const size_t& numberOfElements
) {
    if (originalPosition < 0)
        throw std::runtime_error(
            "There is en error in distance calculation. Distance cannot be nagative: "
            + std::to_string(originalPosition)
        );
    
    if (shiftValue < 0)
        return (numberOfElements + originalPosition - 
        std::abs(shiftValue) % numberOfElements) % numberOfElements;
    else
        return (originalPosition + shiftValue) % numberOfElements;
}


void decrypt(std::vector<IndexedValue>& numbers) {
    size_t numSize{numbers.size()};

    for (size_t i{}; i < numSize; ++i) {
        std::vector<IndexedValue>::iterator it = std::find_if(
            numbers.begin(), 
            numbers.end(), 
            [i](const IndexedValue& indexedValue
        ) {
            return indexedValue.index == i;
        });

        if (it != numbers.end()) {
            std::ptrdiff_t position = std::distance(numbers.begin(), it);
            IndexedValue removedElement = std::move(*it);
            numbers.erase(it);
            size_t newPosition = calculateNewPositon(position, removedElement.value, numSize - 1);
            numbers.insert(numbers.begin() + newPosition, std::move(removedElement));
        } else {
            throw std::runtime_error("Index of IndexedValue not found.");
        }
    }
}


int calculateGroveCoordinates(const std::vector<IndexedValue>& numbers) {
    size_t numSize{numbers.size()};

    std::vector<IndexedValue>::const_iterator it = std::find_if(
        numbers.begin(), 
        numbers.end(), 
        [](const IndexedValue& indexedValue
    ) {
        return indexedValue.value == 0;
    });

    if (it != numbers.end()) {
        int groveCoordinatesSum{};
        int shifts[3] = {1000, 2000, 3000};
        std::ptrdiff_t position = std::distance(numbers.begin(), it);

        for (auto& shift : shifts) {
            auto temp = numbers.begin() + (position + shift) % numSize;
            groveCoordinatesSum += temp->value;
        }

        return groveCoordinatesSum;
    } else {
        throw std::runtime_error("Value not found.");
    }
}


int main() {
    try {
        auto recievedNumbers = processData("../input/20.txt");
        // auto recievedNumbers = processData("../test_input/20.txt");  // 3

        decrypt(recievedNumbers);
        int result = calculateGroveCoordinates(recievedNumbers);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return 0;
}