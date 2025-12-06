# RUBIK

 A program to solve arbitrary Rubik-type (twisty) puzzles starting from two text files:
 1. a textual description of the puzzle, and
 2. an also textual description of the source and target state (the task).

 The program parses these files, builds structured objects to model the puzzle, and then finds the optimal solution for the given task.
 
## Features

- **Currently implemented:** Breadth-First Search (BFS), also known as "Devil's algorithm" (guarantees shortest solution, but is brute-force).
- **Supported puzzle:** Pocket Cube (2x2x2 Rubik's Cube).
- **Finds shortest solutions** for any task on the Pocket Cube.

## Future Plans

- Implement smarter and faster algorithms, aiming for "God's algorithm" (optimal solution with minimal computation).
- Add support for more Rubik-type puzzles.
- Integrate heuristic distance measures to improve search efficiency.

## Usage

1. Prepare a puzzle description file and a task file. See examples in the repo:
   - **puzzle description:** puzzle_003.txt (base state and states after permutations)
   - **task description:** puzzle_003_task_002.txt (source state, target state and the scrambling steps performed)
2. Build and run the program. Select "Shortest Random Solve" from the main menu.
3. The program outputs the shortest solution sequence for the given task using the BFS algorithm.

---

*Note: There are several different definitions of "Devil's algorithm" and even for "God's algorithm", so don't take these terms too seriously.*

