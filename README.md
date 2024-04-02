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

### [Day 9](https://adventofcode.com/2022/day/9)
I used an unordered set to track the unique positions of a tail. Additionally, I learned about using a struct and used it to improve code readability when accessing x and y positions of knots. In part 2, the number of knots increased from 2 to 10, which forced me to modify the way a tail followed a head. Initially, my new implementation wasn't proper as the result differed slightly from the correct answer. I then referred back to examples and drew diagrams to identify the issue with moving knots around. When refactoring the correct solution, I learned about the spaceship operator (`<=>`) but ended up not using it as it didn't provide any benefits over the working comparison in this case.
I encountered an issue when trying to use `std::unordered_set<std::pair<int, int>>`. As I didn’t know any better, I used `std::unordered_set<std::string>` and converted and combined both ints into a unique string.

### [Day 10](https://adventofcode.com/2022/day/10)
It was a straightforward puzzle. The input was easy to process. The solution didn't require anything more than I have already familiarized myself with.

### [Day 11](https://adventofcode.com/2022/day/11)
The input was difficult to process, leading me to use a struct to store input data. Throughout the challenge, there were many opportunities to learn and try different things. For the first time, I used a lambda function, `std::find_if`, and `std::erase`. In Part 2, I had to use modulo magic to keep numbers within a manageable range without influencing the result of necessary tests.

### [Day 12](https://adventofcode.com/2022/day/12)
For this puzzle, I used an unordered map to store the x, y positions of the surrounding area along with the elevation level. The next step was to build a directional graph, and finally, I applied the Breadth-First Search (BFS) algorithm to find the shortest path.

In Part 2, I found it more convenient to choose the end node as the start node and I used Dijkstra's algorithm to determine the shortest path from all shortest paths between the elevation level of 'a' and the endpoint 'E'. I used a vector to store the elevation levels of the surrounding area, which I then utilized to build an unordered map representing the directed graph. Since the graph was unweighted, there was no need to store edge weights. During the process, each node received a unique label represented by an integer value starting from 0. I simplified the Dijkstra algorithm by removing the need to track the parent node for each node. In the refactoring phase, I removed an unordered set that tracked visited nodes, as checking if the shortest path for a node is equal to infinity indicates whether the node has been visited or not.

I noticed that many nodes couldn't be reached, as their shortest path value was equal to infinity after running the algorithm. Because the graph was unweighted, the algorithm could be greatly simplified, as the first time you encounter a node, you always reach it with the minimum number of steps.

### [Day 13](https://adventofcode.com/2022/day/13)
The challenge here was to parse and compare the input properly against each other. Unfortunately, the first attempt wasn't successful, and it took a lot of time to pinpoint the issue. It was a good opportunity to use additional tests to help identify edge cases, but it seems C++ doesn't have a built-in unit test module. After reading about different frameworks, I opted for using plain asserts.

In Part 2, only one tweak was required in the comparison function used as a custom sort function with `std::sort` to sort all packets. Sorting wasn't strictly necessary to get the answer, as comparing introduced dividers against packets and counting the number of packets that were "smaller" than dividers worked better. However, it was an opportunity to see a custom sort function in action.
