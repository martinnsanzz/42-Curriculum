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
| ✅ Doing | Graph object (neighbours, zone costs, capacities, blocked zones) |
| ⬜ To do | Drone object (id, position, path, state) |
| ⬜ To do | Shortest path for one drone (BFS, then Dijkstra) |
| ⬜ To do | Turn simulator (capacity checks, drones wait when full) |
| ⬜ To do | Multiple paths for bottlenecks |
| ⬜ To do | Output format and terminal display |
| ⬜ To do | Run all maps, mypy, flake8 |


**Currently working on the graph object:**
- link_capacity(a, b)
Returns how many drones can use the link between a and b. Build frozenset({a, b}) and look it up in the link capacity dict. The order of a and b doesn’t matter. If the two zones aren’t connected, the key is missing, so decide whether that raises a clear error or returns 0. Raising is safer, because asking about a link that doesn’t exist is usually a bug.