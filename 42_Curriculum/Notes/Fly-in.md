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

### Color in Terminal
Terminal traditionally take an input of bytes and display them as white text on a
black background. If the input contain specific **control characters**, then the
terminal may alter certain display properties of the text (Color or font).
Old terminal could only display a max of `8 colors`. However modern terminals can 
display `24-bit RGB color`.

<u>ASCII Escape Character</u>

The **ANSI ASCII standard** represents the escape `ESC` char by the decimal `27 (33 in octal, 1B in hex)`. This char is part of the *control characters (0-31 and 127)* not 
one of the printables *(32-126)*.

Most shells recognize the `Escape Key` as a control character (Usually fo keyboard 
shortcut). So, each programing language has its own method of representing the escape
char within a string

| | Bash | C | Python 3 |
|---|---|---|---|
| Literal | \e, \E | \e (non-standard) | Python 3.3+ only: \N{ESCAPE}, \N{escape}, \N{ESC}, \N{esc} |
| Octal | \033 | \33, \033 | \33, \033 |
| Hex | \x1b | \x1b | \x1b |
| Unicode | \u1b, \U1b | - | \u001b, \U0000001b |

>**Notes:** The one used the most is the `octal` representation `\033`

<u>ANSI Escape Codes for Terminal Graphics</u>

The `ANSI escape code standard` defines a series of control sequences. Each control
sequence begins with a **Control Sequence Inducer (CSI)**, define as `ESC[`. This is 
then followed by a number of `parameter byestes` (ASCII 0-9:;<=>?) then the leter m
which forms a control sequence known as **Select Graphic Rendition (SGR)**. If no
parameter is given, then 0 will be selected which will reset all SGR effects to their
default. 

>**Notes:** SGR parameter can be chained together with a semicolon `;` as the delimeter.

Some common SGR parameters are shown below:
| Parameter	| Effect |
|---|---|
0 |	reset all SGR effects to their default |
1 |	bold or increased intensity |
2 |	faint or decreased intensity |
4 |	singly underlined |
5 |	slow blink |
30-37 |	foreground color (8 colors) |
38;5;x |	foreground color (256 colors, non-standard) |
38;2;r;g;b |	foreground color (RGB, non-standard) |
40-47 |	background color (8 colors) |
48;5;x |	background color (256 colors, non-standard) |
48;2;r;g;b |	background color (RGB, non-standard) |
90-97 |	bright foreground color (non-standard) |
100-107 |	bright background color (non-standard) |

**Can all terminal display color?**
Unfortunately not all terminals can display colors. Modern terminals generally support
8, 16 or 256 different colors, and many support `True Color (24-bit RGB). This depends
on the specific terminal emulators, its config and the underlying system.

Legacy and basic terminals may only support 8 colors or no colors at old. Too check
if a terminal can support colors the following command can be used"

```bash
tput colors
```


**What is RGB?**
RGB stand for Red, Green and Blue and is a type of color model widely used in digital
marketing and computer graphics. It combines different intensities of these three 
colors that create a total of `255^3` possible outcomes. The intensity of each color
is represented with a value of 0-255, where 0 is the lowest and 255 is the highest
intensity.

When all three intensities are (255 each) it produces white and when theyre all (0) it
produces black. When all three values are equal within the range it produces gray with
different shades.

RGB has a variant called `RGBA` where A stand for **ALPHA**. The alpha channel 
represents the opacity of transparency of a color.

### HTTP

**HTTP** stand for `Hyper Text Transfer Protocol` which are a set of rules that tell 2
computers how they're supposed to "talk" to one another. We have 3 components:
- **Client or Local machine** → This is the computer you work at.
- **Server or the remote machine** → This houses the website files that you try to access.
- **The router** → This is the machine that connects both and "talks" to each other.

The way this connection works is as follows:
1. The *client machine* does a request to the server.
2. The *server machine* does a response that sends an answer to the client.

These 2 things always happen over **HTTP**. When the request happens, the server will ask
itself if it has the files being asks for. If it has them, it will return the in the
response, else it will respond with an error or a status code.

<u>Status Codes</u>

These are the status code a server can respond with (Always 3-digit numbers):
* 100–199 → Informational, indicate that the server has received the request and is 
continuing to process it.
* 200–299 → Success *E.g., 200 (Success)*
* 300–399: → Redirection, the client must take additional action to complete the request, 
often involving a different URL.
* 400–499 → Client Error *E.g.,404 (Page Not Found)*
* 500–599 → Server error due to an internal problem.

<u>HTTP Request</u>

A `HTTP request` is a message sent by a client (typically a browser or mobile app) to a 
server, asking for a specific resource or action. When you hit an endpoint like 
`GET /api/users`, your client sends an HTTP request to the server—and that request contains 
multiple layers of important information.

**The 4 Main Parts of an HTTP Request**

**1. Request Line**

This is the first line of the HTTP request, and it is used to initialize an action. This line
indicates what kind of method, request target and version of HTTP protocol the client is
using. This includes:
- **Method:** `Get`, `POST`, `PUT`, `DELETE`...
- **Request:** The path or endpoint *(e.g. /api/users)*
- **HTTP Version:** Usually `HTTP/1.1` or `HTTP/2`

```Example
GET /api/users HTTP/1.1
```

**2. Headers**

Headers carry `metadata` about the request, this includes. The server leverages this 
information to identify the browser and OS version of the client. HTTP request headers are 
*case-insensitive, followed by a colon (:) and a value.* 

Common request headers include:
- **Host:** The domain name of the server
- **Content-Type:** Type of the request body (e.g. application/json)
- **Authorization:** Token or credentials
- **User-Agent:** Info about the client software and OS
- **Accept:** This give Info about the kind of content the client can understand.
- **Connection:** This suggests the connection between client and server should be kept open 
for subsequent requests.
- **Accept-Language:** Preferred language of the client

```Example:
Host: api.example.com
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:125.0) Gecko/20100101 Firefox/125.0
Content-Type: application/json
Connection: keep-alive
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Authorization: Bearer <token>
```

**3. Body (Optional)**
The body contains data sent to the server.  It's often `JSON, form data, or raw text`. The 
format of the data in the body is specified by the Content-Type header in the request. This 
is usually done with `POST`, `PUT`, or `PATCH` requests.

```Example
{
  "name": "Elijah",
  "email": "elijah@example.com"
}

// in the header

Content-Type: aapplication/json
```

**4. Query Parameters (Optional)**
These are `key-value pairs` appended to the URL to `filter` or `sort` data. If you have 
multiple parameters, they're usually separated by a `ampersand (&)`.

```Example
GET /api/products/search?q=coffee+beans&sort=price-low
```

<u>HTTP Responses</u>

An HTTP response is the server’s way of *communicating the result of a client request*. It 
typically consists of 3 main components:

- **Status Line** → Indicates whether the request was successful, failed, or needs further 
action. *e.g. HTTP/1.1 404 Not Found*
- **Headers** → Contain metadata about the response, such as content type, server 
information, or cookies. *e.g. Content-Type: Image/png*
- **Body** → The actual content returned, such as an HTML page, JSON data, or an image.
*e.g. The file/resource requested*

These responses are standardized, and each carries a status code that provides immediate 
insight into what happened with the request.

### DevTools
Every modern **web browser** includes a powerful suite of develop tools. These tools
can do multiple things such as:

- Inspecting currently-loaded HTML, CSS and JavaScript.
- Showing which assets the page has requested.
- Showing how long they took to load.

The DevTools usually open by default the `inspector`, this tool shows what the HTML on 
the page looks at runtime *(e.g. headers, fonts, body, images...)*. It also allows you to 
**instantly modify** the HTML and CSS *(CSS is the language we use to style an HTML document.)* and see the results of your changes reflected live in the 
browser viewport.

Keyboard shortcut to open DevTools:
- Windows: Ctrl + Shift + I or F12
- macOS: ⌘ + ⌥ + I

### Local host
**Localhost** is always your computer, which means the computer is talking to itself. Within 
my network `localhost(127.0.0.1)` allows the computer to communicate with itself, while
*192.168.0.1* is a private IP address used for local network devices. **Localhost is a 
domain name reserved as a **top-level domain** for documentation and testing purposes.
If you access`http://localhost//` in the browser, the request will not be forwarded to the 
internet, it will instead remain in your system. 
>**Notes:** Localhost has the *IP address 127.0.0.1*. This refers back to your server. 

An **IP** address is a unique number that identifies a device on the internet or a local
network. This is used to help computers find and communicate with each other.

<u>How to set up a local testing server with Python</u>

Python contains a `http:server` module. These are the steps to follow:

1. Navigate to the directory that contains the website code you want to test, using the `cd` command.

2. Enter the command to start up the server in that directory.

```bash
python3 -m http.server
```

3. By default, this will run the contents of the directory on a local web server, on port 8000. You can specify a port writing it at the end 

```bash
python3 -m http.server 8080

# Or a directory using with the flag -d

python3 -m http.server 8000 -d src/main/frontend/web/
```
