*This project has been created as part of the 42 curriculum by masanz-s*

[Martin GitHub](https://github.com/martinnsanzz)

# Fly-in
*Drones are interesting.*


## 📂 Description


## 🔷 Instructions


## ➕ Additional requirements

### 🐨 How it works?


## 📖 Resources
This is a list of multiple resources use through out the life-cycle of the project

### 🐍 Python
- [Pydantic vs Dataclass](https://medium.com/@nishthakukreti.01/pydantic-vs-dataclass-640ed78a5f7c)
- [Pydanctic Validation Doc's](https://pydantic.dev/docs/validation/latest/get-started/)
- [How to represent graphs in python](https://medium.com/@arjunprakash027/how-to-represent-graphs-in-python-e1bfc2fee8cb)
- [Pytest Docs](https://docs.pytest.org/en/stable/)
- [Class Methods](https://realpython.com/ref/builtin-functions/classmethod/)
- [Hashable](https://realpython.com/ref/glossary/hashable/)
- [Queue in Python](https://www.geeksforgeeks.org/python/queue-in-python/)
- [Priority Queue](https://www.geeksforgeeks.org/python/priority-queue-in-python/)
- [Argparse Docs](https://docs.python.org/3/library/argparse.html)
- [Find files, folders in your directory with Python](https://richard-taujenis.medium.com/find-files-folders-in-your-directory-with-python-2ff11081e373)
- [Curses Menu docs](https://curses-menu.readthedocs.io/en/latest/usage.html)

### 📋 Other Stuff
- [Multi-Agent Path Finding with Capacity Constraints](https://arxiv.org/pdf/1907.12648)
- [Multi-agent pathfinding](https://en.wikipedia.org/wiki/Multi-agent_pathfinding)
- [Graph Theory](https://en.wikipedia.org/wiki/Graph_theory)
- [Shortest Path Problem](https://en.wikipedia.org/wiki/Shortest_path_problem)
- [Introduction to Graph Theory](https://www.youtube.com/watch?v=LFKZLXVO-Dg)
- [Dijkstras Shortest Path Alghoritm Explained](https://www.youtube.com/watch?v=bZkzH5x0SKU&t=288s)
- [Learn Adjacency Lists](https://www.youtube.com/watch?v=ee6zIj4J3-Y&t=51s)
- [Graphs: Edge List, Adjacency Matrix, Adjacency List, DFS, BFS](https://www.youtube.com/watch?v=4jyESQDrpls&t=469s)
- [OOP](https://www.geeksforgeeks.org/dsa/introduction-of-object-oriented-programming/)
- [Shortest Path using BFS](https://youcademy.org/graph-shortest-path-using-bfs/)
- [BFS for a Graph](https://www.geeksforgeeks.org/dsa/breadth-first-search-or-bfs-for-a-graph/)
- [How Dijkstra's Algorithm Works](https://www.youtube.com/watch?v=EFg3u_E6eHU)
- [Dijsktras Algorithm how to use it](https://medium.com/@singhatul1155/dijkstras-algorithm-navigating-the-shortest-path-how-to-use-it-and-its-importance-8924295fe690)
- [DSA Dijkstra's Algorithm](https://www.w3schools.com/dsa/dsa_algo_graphs_dijkstra.php)
- [Terminal Colors](https://chrisyeh96.github.io/2020/03/28/terminal-colors.html)
- [RGB](https://rgbcolorpicker.com/)

## 🤖 AI Usage

**AI was NOT used to generate code.** All function implementations were written by Martin™.

**Where AI was used:**


This README.md file was done by **human fingers, sweat and tears**. No AI wrote a single
line of text :)


# Project Guide (To be deleted)

| Status | Task |
|---|---|
| ✅ Done | Loader, models, `parse_map`, parser and `DroneMap` tests |
| 🔄 Doing | Read the subject's movement rules |
| ✅ Done | Graph object (neighbours, zone costs, capacities, blocked zones) |
| 🔄 Doing | Drone object (id, position, path, state) |
| ✅ Done | Shortest path for one drone (BFS, then Dijkstra) |
| ⬜ To do | Turn simulator (capacity checks, drones wait when full) |
| ⬜ To do | Multiple paths for bottlenecks |
| ⬜ To do | Output format and terminal display |
| ⬜ To do | Run all maps, mypy, flake8 |


## Subject Requirements
Implementation must hablde:
1. Distribution of drones across multiple paths
2. Strategic waiting when movement isn't possible
3. Avoidance of path conflicts and deadlocks

**Zone Occupance Rules**
- By default, a zone may contain at most one drone at any given simulation turn.
- Zones with max_drones=N metadata can contain up to N drones simultaneously.
- The only special exceptions to occupancy rules are:
    ◦ The start zone: all drones begin here and may share the space initially.
    ◦ The end zone: multiple drones can arrive here and are considered delivered.
- Two drones may not enter the same zone on the same turn unless the zone’s capacity
allows it.
- A drone may not move into a zone that would exceed its maximum capacity.
- Connection capacity (max_link_capacity) defined on connections limits how many
drones can traverse the same connection simultaneously.
- Drones may move simultaneously, as long as all capacity constraints are respected.

**Movement rules from the subject**
The simulation runs in discrete turns. At each turn every drone may:
- Move to an adjacent neighbour (if capacity allows)
- Move to a connection towards a restricted zone (That requires 2 turns to be reached).
In this case, the drone MUST reach its destination during the next turn. It can't wait extra turns on the connection
- Stay in place (To wait or if movement is blocked)

Simulation must prevent conflicts and ensure valid movement scheduling based on 
turn-by-turn state evaluation:
- Drones moving out of a zone free up capacity for the same turn
- A zone must have available capacity for a drone to move into it (After all drones 
moving out have freed up space)

**Algorithm needs to understand**
- Path lengths including movement cost associated with zone types
- Turn scheduling, to prevent drones from colliding or blocking each other
- Graph structure, to determine available disjoint or overlapping paths
- Zone capacity constrains (max_drones) and connections capacity (ma_link_capacity)

> Algorithm must be adaptable: Different maps require different routing strategies

