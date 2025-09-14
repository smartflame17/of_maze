# Maze Generator and Player

This project implements a maze generator and player using C++ and OpenFrameworks.

## Algorithm Overview

The core algorithm for maze generation is based on a randomized version of **Eller's algorithm**, which efficiently generates perfect mazes one row at a time. The maze is represented as a grid, and the algorithm ensures that there is one unique path between any two cells (i.e., the maze is a spanning tree).

### Maze Generation Steps

1. **Initialization**:  
   - Each cell in the first row is given a unique set identifier.
   - Arrays are allocated:  
     - `MazeRow`: Holds the set index for each cell in a row.
     - `RowConnect`: Marks horizontal connections (removal of vertical walls between adjacent cells in the same row).
     - `ColConnect`: Marks vertical connections (removal of horizontal walls below each cell).

2. **Row Processing**:
   - For each row (except the last):
     - **Horizontal Connections**: Randomly merge adjacent cells (by removing vertical walls) and update set indices so merged cells share the same set.
     - **Vertical Connections**: For each set, ensure at least one cell creates a passage downward (to prevent isolated regions). Vertical connections are chosen randomly with a bias to create more vertical corridors.
     - **Set Propagation**: Cells not connected downward are given new set identifiers in the next row.

3. **Final Row**:
   - All adjacent cells with different set identifiers are merged to ensure the maze is fully connected.

4. **Output**:
   - The maze is saved to a file (`maze.maz`) using ASCII art, with "+" for corners, "-" for horizontal walls, and "|" for vertical walls.

### Player Logic

- The player starts at the top-left corner of the maze.
- Movement is handled with boundary checks and the `MazeCell` connectivity (left, right, up, down).
- The player's position is updated based on input and cell connectivity, ensuring movement only occurs through open paths.

## Data Structures Used

- **Arrays**:  
  - `MazeRow[]`: Stores the set indices for each cell in the current row.
  - `RowConnect[]`: Boolean array for horizontal wall removal between cells.
  - `ColConnect[]`: Boolean array for vertical wall removal (passage to the next row).
- **Set Indices (Cell Indexing)**:  
  - Each cell's set index is used to manage connected regions, enabling efficient merging and tracking for the maze generation process.
- **Randomization**:  
  - Functions like `RandBool()` and `RandBoolWithBias()` introduce randomness into the maze structure, ensuring each generated maze is unique.

## Key Files

- `src/GenerateMaze.cpp`: Implementation of the maze generation algorithm.
- `src/Mazeplayer.cpp`: Handles player movement logic and interaction with the maze.
- `src/main.cpp`: Application entry point and OpenFrameworks setup.

## Summary

This project demonstrates the use of Eller’s algorithm for efficient maze generation and manages maze state using arrays representing sets and connectivity. Player movement is restricted to valid paths based on the maze's generated structure.
