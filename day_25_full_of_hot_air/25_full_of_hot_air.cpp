#include <cmath>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


std::vector<std::string> processData(const std::string& filename) {
    std::ifstream file(filename);

    if (!file)
        throw std::runtime_error("Error opening the file: " + filename);

    std::string line{};
    std::vector<std::string> snafuValues{};

    while (getline(file, line))
        snafuValues.push_back(line);

    return snafuValues;
}


long convertToDecimal(std::string snafuValue, const std::string& snafuSymbols) {
    std::reverse(snafuValue.begin(), snafuValue.end());
    long decimalValue{};

    for (int i{}; i < snafuValue.size(); ++i) {
        int snafuSymbolsIndex = snafuSymbols.find(snafuValue[i]);
        decimalValue += std::pow(5, i) * (snafuSymbolsIndex - 2);
    }

    return decimalValue;
}


long sumConvertedValuesToDecimal(const std::vector<std::string>& snafuValues,
                                 const std::string& snafuSymbols) {
    long sumOfDecimalValues{};

    for (const auto& snafuValue : snafuValues)
        sumOfDecimalValues += convertToDecimal(snafuValue, snafuSymbols);

    return sumOfDecimalValues;
}


std::string convertToSnafu(long decimalValue, const std::string& snafuSymbols) {
    std::string snafuValue{};

    while (decimalValue) {
        ldiv_t div = std::ldiv(decimalValue + 2, 5);
        decimalValue = div.quot;
        snafuValue = snafuSymbols[div.rem] + snafuValue;
    }

    return snafuValue;
}


int main() {
    try {
        std::vector<std::string> snafus = processData("../input/25.txt");
        // std::vector<std::string> snafus = processData("../test_input/25.txt");  // 2=-1=0

        std::string snafuSymbols{"=-012"};
        size_t decimalSum = sumConvertedValuesToDecimal(snafus, snafuSymbols);
        std::string result = convertToSnafu(decimalSum, snafuSymbols);
        std::cout << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return 0;
}