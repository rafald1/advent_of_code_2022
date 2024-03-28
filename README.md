## Advent of Code 2022 Solutions (work in progress)
This repository contains my solutions for the [Advent of Code 2022](https://adventofcode.com/2022) challenge, implemented in C++20 (hopefully). The purpose of this project is to:
- learn C++,
- showcase my problem-solving approach,
- practice writing clear and readable code.

### Directory Structure
- 'day_xx_puzzle_name/': Contains C++ scripts for each day's challenge, where 'xx' is the day number and 'puzzle_name' is the name of the puzzle.
- 'input/': Directory where input files are expected to be placed in format: xx.txt. Input files are not included.

### Error Handling
Please note that while the solutions aim to solve the given puzzles accurately, they may not handle unexpected input gracefully. The code assumes input files are formatted as specified in each day's puzzle description.

### Preparations
- **Choosing IDE:** After my prior experience with PyCharm and IntelliJ, I considered CLion. However, due to the absence of a community edition, I explored other options such as Xcode and Visual Studio Code (VS Code). 
- **Decision Process:** I experimented with both Xcode and VS Code, switching between them a few times. Ultimately, I chose to proceed with VS Code due to its flexibility and configurability.
- **Project Structure:** I decided to organize my solutions by splitting each day's solution into two separate files while keeping all the puzzle solutions within one project. This structure posed challenges in Xcode but was successfully implemented in VS Code after some configuration.

### [Day 1](https://adventofcode.com/2022/day/1)
I began by learning how to open files and read them line by line. Additionally, Part 2 required me to start familiarizing myself with vectors: inserting elements, sorting them, and adding up their elements.

### [Day 2](https://adventofcode.com/2022/day/2)
I practiced with vectors, learned about pairs, unordered maps, enum classes, and static_cast.

### [Day 3](https://adventofcode.com/2022/day/3)
I used sets in Part 1. In Part 2, I opted to use a for loop to find a common character in strings.

### [Day 4](https://adventofcode.com/2022/day/4)
I used regex to familiarize myself with the way it works in C++, learned about tuples, and gained further understanding of vectors.

### [Day 5](https://adventofcode.com/2022/day/5)
Processing the input had to be split into two steps. I used regex for step two. I learned about vectors' emplace_back method and looping through characters in a string. The solution to both part 1 and part 2 of this puzzle was straightforward after dealing with the input.

### [Day 6](https://adventofcode.com/2022/day/6)
This puzzle was straightforward. I used sets to assess uniqueness of characters in strings.

### [Day 7](https://adventofcode.com/2022/day/7)
It took some time to figure out the proper way to process the input and then refactor it. I used a stack (vector) to track directory structures and an unordered map to store their sizes. Once accomplished, solving both part 1 and part 2 was straightforward. At this point, I learned that '{}' can be used to initialize variables.

### [Day 8](https://adventofcode.com/2022/day/8)
I struggled a lot with figuring out how to solve this puzzle with my limited knowledge of C++. I decided to assess the visibility of trees by traversing row by row and column by column instead of exploring in four directions for each individual tree. This approach worked fine for part 1. However, for part 2, I had to start over and calculate the scenic score for each individual tree.
