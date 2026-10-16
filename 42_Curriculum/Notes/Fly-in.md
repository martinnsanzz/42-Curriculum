# Fly-in

## Concepts

### Multi-agent pathfinding
The problem of `Multi-agent pathfinding (MAPF)` is an instance of multi-agent planning
and consist in the computation of collision-free paths for a group of agents from
their location to an assigned target.
The aim is to find those paths that optimize a given objective function (Usually
defined as the number of time steps until all agents reach their goal cells).

Several algorithms have been proposed to solve the MAPF problem. Due to the
complexity, it happens that the optimal approaches are not feasable on big environments
or with high number of agents. It is important to reach a trade-off between the
efficiency and the solution and its effectiveness.

**Problem formalization**

The elements of a classical MAPF are:
- A set of `k` agents `A = {1, 2,...., k}`
- An undirected `graph G = (V, E)` where `V` is the node set and `E` is the edge set.
The nodes represent the possible locations of the agents, while the arcs are the
possible connections between such positions.
- A map `s: A -> V` that associates each agent with its starting point.
- A map `t: A -> V` that associates each agent with its target point.

It is assumed that time is separate, and that each agent can peform one action at each
time step. There are two possible actions for an agent:
- **Wait action**, in which the agent remains in its node.
- **Move action**, in which the agent moves to an adjacent node.

An action is formalized as a function:
- `a: V -> V`, meaning that `a(v) = v'` represents the action of moving from `v to v'`
if `v'` is adjacent to `v` and different than `v'`.
- Or to stay in node `v` if `v = v'`.

The agents perform sequences of actions to go from their starting position to their
target location. A sequence of action performed by a single agent is called a plan.
A valid solution for the **MAPF** problem is a set of `k single-agent plans (one for each agent)`.

>**Note:** Once an agent has reached its target, it can either remain in the target 
>location or dissapear

**Types of conflicts**

A valid multi-agent pathfinding (MAPF) solution requires that the single-agent plans of all agents do not collide. Collisions are defined by comparing agent positions at each time step, where `πi[x]` is the position of agent `i` after `x` steps.

| Conflict | Description | Formal condition |
|---|---|---|
| **Vertex** | Two agents occupy the same location at the same time. | `πi[x] = πj[x]` |
| **Edge** | Two agents cross the same edge in the same direction at the same time. Impossible if vertex conflicts are forbidden. | `πi[x] = πj[x]` and `πi[x+1] = πj[x+1]` |
| **Following** | An agent moves into a location that another agent occupied in the previous step. | `πi[x+1] = πj[x]` |
| **Cycle** | Three or more agents rotate through each other's positions in a loop. Impossible if following conflicts are forbidden. | each agent takes the position of the next agent in the cycle |
| **Swapping** | Two agents exchange positions by passing along the same edge in opposite directions. | `πi[x+1] = πj[x]` and `πj[x+1] = πi[x]` |


There is no universal standard. Each problem definition chooses which conflicts are allowed. Vertex and edge conflicts are usually forbidden.

### Graph theory
Graph theory is the study of graphs, which are mathematical structures used to model
`pairwise relations` between object. A **graph** is made up of `vertices (nodes)` 
which are connected by `edges (arcs, links or lines)`.

**Shortest path problem**
In `graph theory` the `shortest path problem` is the problem of findinga **path**
between two **nodes** in a graph such as the sum of the `steps` of its constituend
nodes is minimized.

Two nodes are adjacent when they are both connected to a common node.

#### Shortest Path Problem Variations

The problem is also sometimes called the **single-pair shortest path problem**, to distinguish it from the following variations:

- **Single-source shortest path:** find the shortest paths from a source vertex `v` to all other vertices in the graph.
- **Single-destination shortest path:** find the shortest paths from all vertices in a directed graph to a single destination vertex `v`. This can be reduced to the single-source problem by reversing the arcs in the directed graph.
- **All-pairs shortest path:** find the shortest paths between every pair of vertices `v`, `v'` in the graph.


#### Shortest Path Algorithms

**Directed vs undirected**

- **Directed:** each edge has a direction and its own weight.
- **Undirected:** an edge works both ways with the same weight. Treat it as two directed edges, one per direction.
- **Negative weights:** a negative undirected edge is already a negative cycle (`a→b→a`), so negative weights only make sense on directed graphs.
- **Negative cycle:** a cycle whose total weight is below 0. No shortest path exists, because you can keep looping to lower the cost.

**Algorithms**

| Algorithm | Problem | Weights | Time | Notes |
|---|---|---|---|---|
| **BFS** | Single-source | Unweighted (all equal) | `O(V + E)` | Fewest edges |
| **0-1 BFS** | Single-source | Only 0 or 1 | `O(V + E)` | BFS with a deque |
| **Dijkstra** (array) | Single-source | Non-negative | `O(V²)` | Fine for dense graphs |
| **Dijkstra** (binary heap) | Single-source | Non-negative | `O((V + E) log V)` | The usual choice (`heapq`) |
| **Dijkstra** (Fibonacci heap) | Single-source | Non-negative | `O(E + V log V)` | Best bound, rarely worth it |
| **Dial's** | Single-source | Small non-negative integers | `O(E + L·V)` | Bucket queue, `L` = max weight |
| **Bellman-Ford** | Single-source | Any, negatives allowed | `O(V·E)` | Detects negative cycles |
| **DAG relaxation** | Single-source | Any | `O(V + E)` | Process in topological order |
| **A\*** | Single-pair | Non-negative | depends on heuristic | Dijkstra guided by a heuristic |
| **Floyd-Warshall** | All-pairs | Any, no negative cycles | `O(V³)` | Simple, good for small dense graphs |
| **Johnson's** | All-pairs | Any, no negative cycles | `O(V² log V + V·E)` | Better on sparse graphs |

<u>Dijkstra</u>

1. Set `dist[start] = 0` and every other distance to infinity.
2. Push `(0, start)` onto a min-heap.
3. Pop the cheapest entry. Skip it if its cost is worse than `dist` (stale entry).
4. For each neighbour, compute `new = dist[u] + weight`. If `new < dist[neighbour]`, update `dist`, set `came_from[neighbour] = u`, and push it.
5. Stop at the goal, or when the heap is empty (goal unreachable).
6. Rebuild the path by walking back through `came_from`, then reverse it.

**Why it needs non-negative weights:** once a node is popped, its distance is final, because no later path can be cheaper. A negative edge breaks that guarantee.

<u>Bellman-Ford</u>

- Relax every edge `V - 1` times: `if dist[u] + w < dist[v]: dist[v] = dist[u] + w`.
- Run one more pass. If any distance still improves, there is a negative cycle reachable from the source.
- Slower than Dijkstra, so use it only when negative weights exist.

<u>A\*</u>

- Same as Dijkstra, but the heap is ordered by `g + h`:
  - `g` = cost so far
  - `h` = heuristic estimate of the cost to the goal
- **Admissible:** `h` never overestimates, so the result is optimal.
- **Consistent:** `h(u) <= w(u, v) + h(v)`, so a node never needs to be reopened.
- With `h = 0` it is exactly Dijkstra.

<u>Floyd-Warshall and Johnson's</u>

- **Floyd-Warshall:** a 3-nested loop over `k, i, j` with `dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])`.
- **Johnson's:** run Bellman-Ford once to reweight the edges so none are negative, then run Dijkstra from every vertex. Faster than Floyd-Warshall on sparse graphs.

**Choosing one**

| Situation | Use |
|---|---|
| All edges cost the same | BFS |
| Costs are 0 or 1 | 0-1 BFS |
| Non-negative weights, one source | Dijkstra with a heap |
| One start and one goal, with a good heuristic | A\* |
| Negative weights | Bellman-Ford |
| Directed acyclic graph | Relax in topological order |
| Distances between every pair | Floyd-Warshall, or Johnson's if sparse |


### Representing a graph in Python
In simple terms a graph is a map of connections, where nodes represent entities and
edges represent the relationship between them.

For a graph like this:

```txt
a ---- b
|      |
|      |
c ---- d ---- e
```

- **Nodes:** `a`, `b`, `c`, `d`, `e`
- **Edges:** `a-b`, `a-c`, `b-d`, `c-d`, `d-e`

The graph is undirected, so `a-b` and `b-a` are the same edge.

In an undirected graph, an edge works both ways. **a-b** means you can go from a to b
and from b to a. In a directed graph, each edge has a direction. **a → b** lets you go
from a to b but not back, unless there is also a separate **b → a** edge.

```txt
Undirected:   a ---- b      (a→b and b→a both allowed)
Directed:     a ---> b      (only a→b)
```

In code, an undirected edge is stored twice, once in each direction:

```python
graph = {"a": ["b"], "b": ["a"]}   # undirected a-b
graph = {"a": ["b"], "b": []}      # directed a→b
```
