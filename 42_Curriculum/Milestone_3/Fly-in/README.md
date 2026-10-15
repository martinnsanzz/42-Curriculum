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
| ⬜ To do | Drone object (id, position, path, state) |
| 🔄 Doing | Shortest path for one drone (BFS, then Dijkstra) |
| ⬜ To do | Turn simulator (capacity checks, drones wait when full) |
| ⬜ To do | Multiple paths for bottlenecks |
| ⬜ To do | Output format and terminal display |
| ⬜ To do | Run all maps, mypy, flake8 |


**Currently working on the algorithms:**

## Dijkstra's algorithm

This algorithm works for `directed/undirected` but only with **positive** cost values.

Time complexity for this algorithm is `O((V+E) * log(V))` where **E** is the number of nodes
and **V** is the number of vertices.

**Data structures that it needs:**
- **Priority queue (heapq)**. It holds the frontier and always gives you the cheapest zone first. Each entry is the cost so far, with the zone name next to it. If you add the priority-zone tie-breaker, the cost becomes a tuple like (turns, -priority_zones), followed by the name.
- **Best-cost dict (dist)**. It maps a zone name to the cheapest total cost found so far. The start begins at 0, and every other zone is treated as infinity until it is reached.
- **Parent dict (came_from)**. It maps a zone to the zone you came from on the cheapest route. This is what lets you rebuild the path at the end.
- **Visited or finished set**. It marks zones whose cost is final, so you skip stale entries in the heap. A check like “popped cost is worse than dist” does the same job without a set.

**How it works:**
1. Set initial distances for all vertices: 0 for the source vertex, and infinity for all the other.
2. Choose the unvisited vertex with the shortest distance from the start to be the current vertex. So the algorithm will always start with the source as the current vertex.
3. For each of the current vertex's unvisited neighbor vertices, calculate the distance from the source and update the distance if the new, calculated, distance is lower.
4. We are now done with the current vertex, so we mark it as visited. A visited vertex is not checked again.
5. Go back to step 2 to choose a new current vertex, and keep repeating these steps until all vertices are visited.
6. In the end we are left with the shortest path from the source vertex to every other vertex in the graph.