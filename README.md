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

### [Day 14](https://adventofcode.com/2022/day/14)
I utilized regex to parse the input data. Based on the input, I constructed a cave represented by an unordered set. The trickiest part was correctly implementing the behavior of pouring sand, which took a few tries to get right. In Part 2, I had to modify the pouring sand behavior to address changing conditions.

Along the way, I learned about templates and how to compute a hash for a pair when using unordered sets or unordered maps.

### [Day 15](https://adventofcode.com/2022/day/15)
I learned about `sscanf` as another way to process input data. I chose a straightforward approach to solving this puzzle. Unfortunately, the solution for part 2 is quite slow.

### [Day 16](https://adventofcode.com/2022/day/16)
This puzzle was very challenging. Starting with input processing, I had the opportunity to learn more about regex. Here's how I tackled it:
- I used an unordered map to store all distances between nodes and applied the Floyd–Warshall algorithm to calculate them.
- I used only valves capable of releasing pressure and the starting valve.
- I implemented a recursive function that tried all possible combinations of valves that could be opened during the allotted time.
- I utilized another unordered map to speed things up by memorizing the results of the recursive function.

Part 2 made things even more complicated. The final solution is very slow and takes almost 2 minutes to find the answer. At this point, I'm just happy it works.

### [Day 17](https://adventofcode.com/2022/day/17)
For this puzzle, I decided to use complex numbers to store x and y positions, as I had never tried this approach before. It made position-related calculations straightforward. Along the way, I learned about the `static` keyword and its use in functions to produce different outcomes based on subsequent calls.

In Part 2, I was forced to rethink my approach, and the `static` keyword disappeared from functions. There were many difficult parts in this puzzle. Initially, I struggled with properly moving rocks and calculating the height of the tower created by rocks. In Part 2, the challenge was predicting the height of the tower after 1 trillion (or 1 billion, depending on where you're from) of rocks.

### [Day 18](https://adventofcode.com/2022/day/18)
In Part 1 I used a straighforward approach to solve it. I used struct to represent lava dropletes as cubes. I overloaded equality operator so I can use this struct as a type in unordered set. I added operator+ as well to make calculating x, y, z position of 6 adjecent cubes easier.
Part 2 required to only count external surface. This was accomplished by using the unorderd set to map the surrounding cubes and finally checking if every lava droplet has any adjecent cube in this map of surrounding cubes indicating that this side of lava droplet is an external side.

### [Day 18](https://adventofcode.com/2022/day/18)
In Part 1, I used a straightforward approach to solve it. I represented lava droplets as cubes using a struct. To facilitate comparisons, I overloaded the equality operator for this struct and added the operator+ to simplify the calculation of the positions of the six adjacent cubes.

For Part 2, the task was to count only the external surface of the lava droplets. I achieved this by using an unordered set to map the surrounding cubes and then checked if every lava droplet had any adjacent cube in this set of surrounding cubes. This indicated that this side of the lava droplet was an external side.

### [Day 19](https://adventofcode.com/2022/day/19)
For Part 1, I created a struct and overloaded the +, -, >=, and [] operators to keep the required changes easy to follow. Initially, the solution was slow, taking 616 seconds to complete. To optimize it, I added memoization, which reduced the time needed to 144 seconds. However, due to the nature of the problem, it was easy to identify cases that were not worth pursuing, allowing for pruning, which further reduced the time to 24 seconds. Continuing to refine the pruning conditions eventually brought the time down to 0.9 seconds. At this point, removing memoization reduced the time to 0.3 seconds.

Part 2 proved that you can easily prune the correct branch. I had to make my pruning conditions less strict, which resulted in a 2.1-second solution. I tried with memoization as well, but it only increased the time, so I ultimately removed it.

Returning to Part 1, I loosened the pruning conditions, resulting in a 0.4 second solution.

### [Day 20](https://adventofcode.com/2022/day/20)
This puzzle was a source of confusion for me as I failed to understand why my formula for calculating the new position for negative numbers was producing the wrong value. I spent some time troubleshooting the issue, only to realize that `a + b % a` isn't the same as `a - abs(b) % a` when 'b' is a negative integer and 'a' is of type `size_t`, with the expected result also being `size_t`. In the end, I moved the calculation of the new position to a function and overdid it just for the sake of learning. Part 2 required small changes to get it working.

### [Day 21](https://adventofcode.com/2022/day/21)
Part 1 was straightforward to solve, and the recursive function proved to be a good choice for solving this problem.
Part 2 became complicated due to changes in conditions and required an additional recursive function. The first step was to attempt to calculate the left and right values of the root operation using the recursive function from Part 1. However, only one of these values could be calculated in Part 2. This value had to be used to calculate backward until the missing value was determined.

### [Day 22](https://adventofcode.com/2022/day/22)
This is an interesting puzzle. For Part 1, I decided to build a map using an unordered map. Once an edge of the area is reached, you were supposed to appear on the other side of the current row or column, depending on the direction you were moving. To accomplish that, I extended the map by including information about the minimum and maximum index numbers of columns for each row and the minimum and maximum index numbers of rows for each column. This allowed for easy wrapping around based on position and direction of movement.

I decided to skip Part 2 as I would like to give it more thought on how to move around edges when you are on the surface of a cube.

Update: After spending a lot of time studing cubes, I have learnt that there are 11 distinctive 2d layouts of a cube and each of them can have diffrent orientation. Which made the approach of recognizing the layout not feasible. There are 14 edges that need to be connected to create 7 pairs. Obviously, if an external angle between edges is 90 degrees you can connect these edges. Depending on a layout you can easily create 2-4 obvious pairs. The difficult part was to connect all remaining edges. I almost gave up and hardcoded how traverse between edges. Luckily, I have found a tip that made this solution possible. The tip was: when you connect two edges (external angle between edges is 90 degrees) you remove them and you change the direction of (rotate) each edge after deleted ones and you carry on with the process until all edges are connected.

It took a lot of work, but final solution is working properly. Step by step process below.
1. Create an edge list that contains direction, x, and y by moving around the edges of a map in clockwise motion. I have started at the top left corner of a map with direction indicating east. I moved in this direction as long as needed and changed the direction to follow the map edges all the way to the starting position.
2. Calculate the real edge length, which is the minimum edge length amongs all the edges.
3. Split the list you created in step 1 knowing the real edge length. I guess I could have divided by 14 and skip the step 2. ;) The important thing is that list is created in such manner that you don't loose the original directions of each edge in the next step.
4. Find edge pairs by following the tip. Connect two edges that have external angle between them equal to 90 degrees, remove them, change the direction of each edge after deleted ones, contiue with the process until all edges are connected.
5. Build the proper wrap map by joining every edge point on paired edges. I connected points in original order with points in reversed order. Additionaly, I included the direction in this wrap map. The way to get proper directions is to rotate (original direction from step 1) left to get the direction that you will have when leaving this cube face and to rotate right to get the direction you will have when entering this cube face.
That is it. All this work to just build a proper wrap map.

### [Day 23](https://adventofcode.com/2022/day/23)
Once again, I used complex numbers to represent x, y positions. This puzzle resembled Conway's Game of Life, but with elves moving around according to their own rules. In Part 1, the task was to simulate 10 rounds and calculate the empty space in the smallest rectangle that contained all elves. 

For Part 2, I had to simulate until the elves didn't need to move anymore according to the rules. In this part, I improved on elves collision management, realizing that only two elves can try to occupy the same spot at the same time. This optimization sped things up a little bit.
I also realized that the way you calculate the hash can significantly impact performance. I observed differences in execution time, taking 6.1, 4.3, and 2.2 seconds to find the solution based on the way the hash was calculated.

### [Day 24](https://adventofcode.com/2022/day/24)
I changed my approach to solving this puzzle a few times. Initially, I started with calculating and storing blizzard locations every round. However, I ended up storing only the original blizzard locations and "rewinding time". The location would be blizzard-free if in the other four locations there were no blizzards with the correct directions back in the past. Additionally, I changed my approach from using a queue to find the path to storing all possible locations that can be reached every turn. The puzzle required determining the fastest time the exit can be reached, and the knowledge of the exact path that has to be taken wasn't required. For Part 2, I made small changes to allow for additional travel back to the start and again to the end location.

### [Day 25](https://adventofcode.com/2022/day/25)
With a little bit of math, I achieved a straightforward solution for converting snafu values to decimal and decimal to snafu.
