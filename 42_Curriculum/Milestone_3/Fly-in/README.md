*This project has been created as part of the 42 curriculum by masanz-s*

[Martin GitHub](https://github.com/martinnsanzz)

# Fly-in
*Drones are interesting.*

## 📂 Description

**Fly-in** is a Python project that simulates multi-drone pathfinding and scheduling. The goal is to move a fleet of drones from a central hub `(start)` to a target hub `(end)` through a network of hubs and links, in as few turns as possible, while respecting a strict set of constraints.

### 🎯 Objective
Route every drone from `start` to `end` while **minimizing the total number of turns**. Drones move simultaneously each turn, so the challenge is not only finding short paths but also *scheduling* the fleet so that drones do not block each other.

### 🧩 Constraints
- **Zone capacity:** each hub can only hold a limited number of drones at the same time 
(`max_drones`).
- **Link capacity:** each connection can only be used by a limited number of drones per turn 
(`max_link_capacity`).
- **Hub types:**
  - `normal`: costs 1 turn to enter.
  - `priority`: costs 1 turn to enter and is preferred by the pathfinder when routes tie.
  - `restricted`: costs 2 turns to enter.
  - `blocked`: can never be entered.

## 🔷 Instructions

> **Notes:** This project uses `uv` as the package manager. Make sure it is installed, or 
>the Makefile won't let you run the project. The only runtime dependency is **Pydantic**.

### Requirements
- Python **3.13+**
- [`uv`](https://docs.astral.sh/uv/) installed and available in your `PATH`
- `make`

### Installation
Install the dependencies (creates the `.venv` and syncs it with `uv.lock`):

```bash
make install
```

You don't need to run this before the other commands. `make terminal`, `make visual` and `make debug` call it automatically.

### Execution
The program runs as a package (`python -m src`) from the project root.

| Command | Description |
|---|---|
| `make terminal` | Runs the simulation and prints every turn in the terminal. |
| `make visual` | Runs the simulation with the `--visual` flag. *(Reserved for the upcoming visualizer, not implemented yet.)* |
| `make debug` | Runs the program under `pdb`. |

You can also run it directly:

```bash
uv run python -m src --map maps/easy/01_linear_path.txt
```

### Maps
Example maps are in the `maps/` folder, grouped by difficulty: `easy`, `medium`, `hard` and `challenger`. See `maps/README.md` for the map file format.

### Tests
```bash
uv run pytest
```

### Linting and type checking
| Command | Description |
|---|---|
| `make lint` | Runs `flake8` and `mypy` with the standard flags. |
| `make lint-strict` | Runs `flake8` and `mypy --strict`. |

### Cleaning
| Command | Description |
|---|---|
| `make clean` | Removes `.mypy_cache`, `__pycache__`, `.pytest_cache` and `.pyc` files. |
| `make fclean` | Does everything `clean` does and also removes the `.venv`. Run `deactivate` first if the venv is active. |

Run `make help` to list all available commands.


## ➕ Additional requirements

In this section additional requirements mentioned on the `subject.pdf` will be added:

### ⚙️ How it works
1. **Parsing:** the map file is read line by line and validated with **Pydantic** models. 
The first setting must be `nb_drones`, followed by the `start_hub`, the `end_hub`, any 
intermediate `hub` lines and the `connection` lines. Optional `[key=value]` metadata is 
supported on hubs and connections. Invalid maps are rejected with an error message that 
includes the line number.
2. **Graph:** the validated map is converted into an undirected graph with adjacency lists 
and per-link capacities.
3. **Pathfinding:** Dijkstra's algorithm runs backwards from the `end` hub, so every 
reachable hub gets its cheapest cost to the goal. Blocked hubs are never entered but are
present on the graph for visual representation proposes.
4. **Simulation:** drones are moved turn by turn along these routes, respecting hub and link 
capacities.
5. **Output:** Depending on the `visual mode` the result might be shown on the **terminal** 
or on the **visualizer**.

### 💡 Chosen Algorithm

Pathfinding uses **Dijkstra's algorithm** with a **binary heap** as the priority queue 
(Python's `heapq`).

<u>Why Dijkstra</u>

- **Weighted costs:** entering a hub costs 1 turn (`normal`, `priority`) or 2 turns 
(`restricted`). A plain BFS counts edges, not turns, so it would treat a restricted hub like 
any other and return wrong routes.
- **Non-negative weights:** every cost is positive, which is the one condition Dijkstra 
needs to be correct. Slower algorithms that handle negative weights (such as Bellman-Ford) 
are not needed.
- **Simple and fast enough:** the maps are small graphs, so a heap-based Dijkstra is both 
easy to reason about and quick to run.

<u>How it works</u>

1. **Reverse search:** Dijkstra starts at the `end` hub (distance 0) and expands outward, not from `start`. When it finishes, every reachable hub holds its cheapest cost *to the goal*. One run gives the distance for every hub, which a per-drone search from `start` would not.
2. **Cost model:** the cost of a route is the sum of the costs of the hubs entered along it, so entering the end hub counts too (a `restricted` end hub costs 2).
3. **Heap:** the heap holds `(distance, hub)` pairs. Each step pops the closest unvisited hub and tries to improve the distance of its neighbors.
4. **Lazy deletion:** when a shorter route to a hub is found, the new pair is pushed and the old one stays in the heap. Stale entries are skipped when popped (`dist > distance[name]`), because a binary heap can't update an entry in place.
5. **Blocked hubs:** `graph.valid_neighbours` filters out `blocked` hubs, so they are never entered and hubs behind them stay unreachable.
6. **Unreachable start:** if `start` is not in the result, the map has no valid route and a `GraphError` is raised.

<u>Complexity</u>

With `V` hubs and `E` links, using adjacency lists and a binary heap:

| | Complexity |
|---|---|
| Time | `O((V + E) log V)` |
| Space | `O(V + E)` |

<u>Scheduling</u>

Dijkstra only gives the cheapest cost from each hub to the goal. Moving the whole fleet in 
the fewest turns, while respecting hub and link capacities, is handled by the simulation 
(see the **How it works** section above).

### 👁️ Visual representation
<u>Terminal</u>

The `total simulation` is displayed directly in the terminal, turn by turn. Each line is one 
turn separated with the `turn number` and lists the moves made by the drones in that turn.
The drones that move in that turn are represented in the following format:

```bash
D<ID>-<zone> 
# or 
D<ID>-<connection> # in case of drones still in flight toward restricted zones.
```
Drones that do not move during a turn are not listed.

>**Notes:** Zones will be represented with the color described in the metadata field 
>*[color=...]*. If the color is not in the supported colors a `default gray` will be given,
>if the color is `rainbow`, each character of the name will have a different RGB color.


Run it with:

```bash
make terminal
```

This will provide a **selection menu** done in curses, to choose a map from the `maps` 
folder. If `maps` is not found or empty or no levels are found an error will raise.
 
Example output (map `maps/easy/01_linear_path.txt`):

```text
------- Turn nb: 1 -------
D1-waypoint1 

------- Turn nb: 2 -------
D1-waypoint2 D2-waypoint1 

------- Turn nb: 3 -------
D1-goal D2-waypoint2 

------- Turn nb: 4 -------
D2-goal
```

The simulation ends when every drone has reached the `end` hub. The total number of turns is the number of lines printed.


<u>Visualizer</u>


## 📖 Resources
This is a list of multiple resources use throughout the life-cycle of the project

### 🐍 Python
- [Pydantic vs Dataclass](https://medium.com/@nishthakukreti.01/pydantic-vs-dataclass-640ed78a5f7c)
- [Pydantic Validation Doc's](https://pydantic.dev/docs/validation/latest/get-started/)
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
- [Dijkstra's Shortest Path Algorithm Explained](https://www.youtube.com/watch?v=bZkzH5x0SKU&t=288s)
- [Learn Adjacency Lists](https://www.youtube.com/watch?v=ee6zIj4J3-Y&t=51s)
- [Graphs: Edge List, Adjacency Matrix, Adjacency List, DFS, BFS](https://www.youtube.com/watch?v=4jyESQDrpls&t=469s)
- [OOP](https://www.geeksforgeeks.org/dsa/introduction-of-object-oriented-programming/)
- [Shortest Path using BFS](https://youcademy.org/graph-shortest-path-using-bfs/)
- [BFS for a Graph](https://www.geeksforgeeks.org/dsa/breadth-first-search-or-bfs-for-a-graph/)
- [How Dijkstra's Algorithm Works](https://www.youtube.com/watch?v=EFg3u_E6eHU)
- [Dijkstra's Algorithm how to use it](https://medium.com/@singhatul1155/dijkstras-algorithm-navigating-the-shortest-path-how-to-use-it-and-its-importance-8924295fe690)
- [DSA Dijkstra's Algorithm](https://www.w3schools.com/dsa/dsa_algo_graphs_dijkstra.php)
- [Terminal Colors](https://chrisyeh96.github.io/2020/03/28/terminal-colors.html)
- [RGB](https://rgbcolorpicker.com/)

## 🤖 AI Usage

**AI was NOT used to generate code.** All function implementations were written by Martin™.

**Where AI was used:**
- **Concept teacher** → Explaining new concepts related to the project such as Graph Theory,
path-finding algorithms, OOP concepts and standards.
- **Pytest** → Once I learned how to use pytest and the tests where getting more complicated
to formulate (Graph creation, pathfinding testing), Claude helped writing multiple test
cases to see if the code was returning the expected values.
- **Docstrings** → Write down appropriate docstrings for the classes and functions in the 
code.
- **Understanding type errors:** → Helped explain Pylance and `mypy --strict` messages, 
such as incompatible types in assignments and unresolved imports, and why each one appeared.
- **Fixing import and package structure issues:** → Helped trace errors like 
`import-not-found` and `Duplicate module named "__main__"` back to their causes: the 
`mypy_path` setting, missing `__init__.py` files, and the difference between `python -m src` 
and running a file directly.
- **README** → Helped proofread and structure this README.

**What AI was not used for**
The design of the project (parser, graph, pathfinding and simulation logic) and the 
algorithms were written by me. Every AI suggestion was read, tested and understood before 
being kept in the code.