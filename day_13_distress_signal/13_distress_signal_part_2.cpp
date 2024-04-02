#include <algorithm>
#include <cassert>
#include <cctype>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


std::vector<std::string> processData(const std::string& filename) {
    std::ifstream file(filename);

    if (!file) {
        throw std::runtime_error("Error opening the file: " + filename);
    }

    std::string line{};
    std::vector<std::string> packets{};

    while(getline(file, line)) {
        if (!line.empty()) packets.push_back(line);
    }

    return packets;
}


bool comparePackets(const std::string& leftPacket, const std::string& rightPacket) {
    size_t leftIndex{};
    size_t rightIndex{};
    int depth{};

    while (leftIndex < leftPacket.size() && rightIndex < rightPacket.size()) {        
        if (isdigit(leftPacket[leftIndex]) && isdigit(rightPacket[rightIndex])) {
            int leftNumber{};
            for (; isdigit(leftPacket[leftIndex]); ++leftIndex) {
                leftNumber = leftNumber * 10 + leftPacket[leftIndex] - '0';
            }

            int rightNumber{};
            for (; isdigit(rightPacket[rightIndex]); ++rightIndex) {
                rightNumber = rightNumber * 10 + rightPacket[rightIndex] - '0';
            }

            if (leftNumber < rightNumber) return true;
            
            if (leftNumber > rightNumber) return false;

            while (depth > 0) {
                --depth;
                if (rightPacket[++rightIndex] != ']') return true;
            }

            while (depth < 0) {
                ++depth;
                if (leftPacket[++leftIndex] != ']') return false;
            }
        } else if (leftPacket[leftIndex] == rightPacket[rightIndex]) {
            ++leftIndex;
            ++rightIndex;
        } else if (leftPacket[leftIndex] == '[' && isdigit(rightPacket[rightIndex])) {
            --depth;
            ++leftIndex;
        } else if (rightPacket[rightIndex] == '[' && isdigit(leftPacket[leftIndex])) {
            ++depth;
            ++rightIndex;
        } else if (leftPacket[leftIndex] == ']') {
            return true;
        } else if (rightPacket[rightIndex] == ']') {
            return false;
        }
    }

    return false;  // strings are equal
}


void runTest(const std::string& str1, const std::string& str2, bool expected) {
    bool result = comparePackets(str1, str2);
    std::cout << "Checking if " << str1 << " < " << str2 << ". Expecting: "
        << std::boolalpha << expected << ". Result: " << result << std::endl;
    assert(result == expected);
}


void comparePacketsAsserts() {
    runTest("[1,1,3,1,1]", "[1,1,5,1,1]", true);
    runTest("[[1],[2,3,4]]", "[[1],4]", true);
    runTest("[9]", "[[8,7,6]]", false);
    runTest("[[4,4],4,4]", "[[4,4],4,4,4]", true);
    runTest("[7,7,7,7]", "[7,7,7]", false);
    runTest("[]", "[3]", true);
    runTest("[[[]]]", "[[]]", false);
    runTest("[1,[2,[3,[4,[5,6,7]]]],8,9]", "[1,[2,[3,[4,[5,6,0]]]],8,9]", false);
    runTest("[[1],4]", "[[2]]", true);
    runTest("[3]", "[[2]]", false);
    runTest("[[[[1]]]]", "[[1,2]]", true);
    runTest("[[[],6,[],1]]", "[[7]]", true);
    runTest("[[7,4,3],[[7],7]]", "[[[[7,0,3],3,[2,6]]]]", true);
    runTest("[3]", "[3]", false);
    std::cout << "All tests passed." << std::endl;
}


void sortPacketsWithDiviversInPlace(std::vector<std::string>& packets, const std::vector<std::string>& dividers) {
    packets.insert(packets.end(), dividers.begin(), dividers.end());
    std::sort(packets.begin(), packets.end(), comparePackets);
}


size_t calculateDecoderKey(const std::vector<std::string>& packets, const std::vector<std::string>& dividers) {
    size_t decoderKey{1};

    for (const auto& divider : dividers) {
        auto it = std::find(packets.begin(), packets.end(), divider);
        if (it != packets.end()) {
            decoderKey *= std::distance(packets.begin(), it) + 1;
        }
    }

    return decoderKey;
}


int main() {
    try {
        std::vector<std::string> allPackets = processData("../input/13.txt");
        // std::vector<std::string> allPackets = processData("../test_input/13.txt");  // 140

        // comparePacketsAsserts();
        std::vector<std::string> allDividers{"[[2]]", "[[6]]"};
        sortPacketsWithDiviversInPlace(allPackets, allDividers);
        size_t result = calculateDecoderKey(allPackets, allDividers);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}