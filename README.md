# Maze Solver Visualizer

A single-file C++ console application for generating mazes, building graph representation, visualizing traversal paths, comparing BFS and DFS, and managing maze records (`MazeRecord`).

## Features

* **Maze Generation & Visualization:** Generates mazes using recursive path carving (`createPaths`) with optional extra loops, bounded by `maxRows = 30` and `maxCols = 50` (`Wall = 0`, `Open = 1`).
* **Grid Legend (`show()`):**
  * `S` — Start cell
  * `E` — Goal cell
  * `|` / `-` — Wall cells
  * `:` — Visited cells
  * `@` — Solution path
    
* **Graph Traversal & Comparison:**
  * Maps open grid cells to node IDs (`r * cols + c + 1`) and builds an adjacency list (`buildGraph`).
  * **DFS (`stack<int>`):** Traverses depth-first and tracks visited cell count (`visitedOut`) and path edge count.
  * **BFS (`queue<int>`):** Traverses breadth-first and tracks visited cell count (`visitedOut`) and shortest path edge count.
  * **Compare BFS vs. DFS:** Runs both algorithms on the current maze and prints visited nodes and path edges for each.
* **Sample Data & Record Management (`maxRecords = 40`):**
  * Loads 15 preloaded sample records (`Maze1` through `Maze15`) via `loadSample`.
  * Supports **View**, **Add** (using unique auto-incrementing `nextId` starting at `16`), **Edit**, and **Delete** operations.
  * Supports **Search by Name** (exact match), **Show Rows >=**, **Sort by Rows** (bubble sort), and **Reset & Load Sample** (restoring sample records, resetting `nextId = 16`, and reinitializing the maze).

## Menu Options

1. **New Maze** — Generates a new maze, rebuilds the graph, and displays it.
2. **Show Maze** — Displays the current maze state.
3. **DFS (Stack)** — Runs DFS, displays visited/path markers, and prints visited count and edges.
4. **BFS (Queue)** — Runs BFS, displays visited/path markers, and prints visited count and edges.
5. **Compare BFS vs DFS** — Runs both traversals and prints their visited counts and edge counts.
6. **Reset & Load Sample** — Reloads the 15 sample records, resets `nextId` to `16`, and resets the maze.
7. **Records (View/Add/Edit/Delete)** — Manage `MazeRecord` entries.
8. **Search / Filter / Sort Records** — Search by exact name, filter by minimum rows (`>=`), or sort ascending by row count.
9. **Exit** — Exits the program.
