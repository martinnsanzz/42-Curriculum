# Push_Swap

## Project Overview
Push_swap is a sorting program written in C. It receives a list of
unique integers as command-line arguments, and outputs the shortest
possible sequence of operations that sorts those integers in
ascending order into stack `a`.

The program operates on two stacks, named `a` and `b`. At the
start, stack a contains all the input integers in the order they
were passed, and stack b is empty. Sorting is achieved exclusively
through a fixed set of stack operations — no direct access or
swapping by index is allowed.

The program must implement four distinct sorting strategies:
| Strategy | Complexity |
|---|---|
| Simple | O(n²) |
| Medium | O(n√n) |
| Complex | O(n log n) |
| Adaptive | Depends on disorder metric |

The strategy is selected via an optional flag (`--simple`,
`--medium`, `--complex`, `--adaptive`). If no flag is provided, the
adaptive strategy is used by default.

The program also supports a `--bench` flag that outputs diagnostic information to stderr:
- The disorder value of the initial stack
- The strategy used and its complexity class
- The total operation count
- A breakdown by operation type


>Error handling: if arguments contain non-integers, duplicates, or
>values outside the `int` range, the program writes `Error\n` to
>`stderr` and exits.

--- 

## Data Structures

Before implementing any sorting logic, we need to decide how to
represent a stack in C. The two main options are arrays and doubly
linked lists. Each has tradeoffs worth understanding before
committing to one.

### Arrays

An array allocates a contiguous block of memory upfront. You know
the size of the input at the start (via `argc`), so you can allocate
exactly what you need.

**Advantages:**
- Simple to implement and debug
- Direct index access — O(1)
- Better cache performance — elements are contiguous in memory
- No `prev`/`next` pointers to manage

**Disadvantages:**
- Rotate (`ra`, `rra`) requires shifting all elements — O(n)
- Fixed size — requires knowing `n` upfront (acceptable here)
- Less natural fit for stack operations conceptually


### Singly Linked List

Each node holds a value and one pointer: `next`.
You maintain a `top` pointer to the head of the stack.
A `tail` pointer is optional but recommended for reverse rotate
operations, otherwise `rra` and `rrb` require a full traversal.
```c
typedef struct s_node
{
    int             value;
    struct s_node   *next;
}   t_node;
```

**Advantages:**
- Simpler to implement than a doubly linked list — no `prev` pointer
- Push/pop from top — O(1)
- No element shifting required
- One less pointer per node to manage and free

**Disadvantages:**
- Reverse rotate (`rra`, `rrb`) requires traversing the full list
  to find the second-to-last node — O(n) unless you maintain a
  `tail` pointer
- Without a `tail` pointer, bottom access is always O(n)
- One `malloc` per node — all must be freed individually
- Worse cache locality — nodes are scattered across heap memory

> `rra` and `rrb` are used heavily in most sorting strategies.
> A `tail` pointer is strongly recommended to avoid O(n) traversal
> on every reverse rotate.

### Summary

| Property | Array | Singly Linked List |
|---|---|---|
| Index access | O(1) | O(n) |
| Push / Pop (top) | O(1) | O(1) |
| Rotate | O(n) | O(1) with tail pointer |
| Reverse rotate | O(n) | O(n) without tail / O(1) with tail |
| Memory layout | Contiguous | Scattered |
| Implementation complexity | Low | Medium |
| Free complexity | One `free` | One `free` per node |

> At 500 elements the performance difference is negligible.
> The linked list maps more naturally onto the operation set.
> The array is simpler to get correct and debug.

---

## Allowed Operations

To sort the numbers of stack `a` in ascending order, the following
operations are available:

### Swap

| Operation | Description |
|---|---|
| `sa` | Swap the first two elements at the top of stack `a`. Do nothing if there is only one or no elements. |
| `sb` | Swap the first two elements at the top of stack `b`. Do nothing if there is only one or no elements. |
| `ss` | `sa` and `sb` at the same time. |

### Push

| Operation | Description |
|---|---|
| `pa` | Take the first element at the top of `b` and put it at the top of `a`. Do nothing if `b` is empty. |
| `pb` | Take the first element at the top of `a` and put it at the top of `b`. Do nothing if `a` is empty. |

### Rotate

| Operation | Description |
|---|---|
| `ra` | Shift up all elements of stack `a` by one. The first element becomes the last one. |
| `rb` | Shift up all elements of stack `b` by one. The first element becomes the last one. |
| `rr` | `ra` and `rb` at the same time. |

### Reverse Rotate

| Operation | Description |
|---|---|
| `rra` | Shift down all elements of stack `a` by one. The last element becomes the first one. |
| `rrb` | Shift down all elements of stack `b` by one. The last element becomes the first one. |
| `rrr` | `rra` and `rrb` at the same time. |

---

## Input Validation & Error Handling
Before any sorting logic runs, the program must validate every
argument passed to it. If any check fails, the program must write
`Error\n` to `stderr` and exit immediately.

### Validation Checks
The following conditions must all be verified on the raw input
before the stack is built:

| Check | Error Condition |
|---|---|
| Non-integer argument | Any argument that is not a valid integer (e.g. `"one"`, `"1.5"`, `""`) |
| Out of range | Any value outside `INT_MIN` to `INT_MAX` (-2147483648 to 2147483647) |
| Duplicate values | The same integer appears more than once |
| Empty string | An argument of `""` is passed |

### Error Output
```c
write(2, "Error\n", 6);
exit(1);
```

> Errors must be written to `stderr` (fd `2`), not `stdout` (fd `1`).
> The program must not produce any operation output if an error occurs.

### Edge Cases
- A single integer is already sorted — print nothing and exit cleanly.
- If no arguments are passed at all — print nothing and exit cleanly.
- Integers passed as a single quoted string (e.g. `"1 2 3"`) must be
  split and validated the same way as separate arguments.
- `+1` and `-0` are valid integer representations and must be handled.
- Overflow must be caught **during parsing**, before casting to `int`.
  Use `long` to parse, then range-check before storing.

### Recommended Parsing Order
1. Check each argument is composed only of digits (and an optional
   leading `+` or `-`)
2. Parse as `long` to avoid overflow during conversion
3. Check the parsed value fits within `INT_MIN` and `INT_MAX`
4. Cast to `int` and store
5. After all arguments are parsed, check for duplicates

> Check for duplicates **after** parsing all values, not during.
> Duplicate detection requires the full list to be available.

---

## Index Normalization
Before running any sorting algorithm, the actual integer values in
stack `a` are replaced with their **ranked indices** from `0` to `n-1`.
This step runs once, after validation, before any operations are executed.

### What It Is
Given the input values, each number is assigned its position in the
sorted order. The smallest value becomes `0`, the next smallest becomes
`1`, and so on up to `n-1`.

**Example:**
| Original value | Normalized index |
|---|---|
| -42 | 0 |
| 7 | 2 |
| 3 | 1 |
| 100 | 3 |

The stack goes from holding `[-42, 7, 3, 100]` to holding `[0, 2, 1, 3]`.
The order of elements in the stack does not change — only the values
stored at each position change.

### Why It Matters
Working with raw integers creates unnecessary complexity:

- Values can be negative, extremely large, or far apart
- Algorithms that rely on ranges or comparisons become harder to reason about
- Chunk-based and radix strategies in particular require values to sit
  in a predictable range

After normalization, you are always working with a contiguous range
`0` to `n-1`. This makes every algorithm simpler to implement and reason
about.

### How To Implement It
1. Copy the values into a temporary array
2. Sort the temporary array (any simple sort works here — this is not counted in operations)
3. For each element in stack `a`, find its position in the sorted array
4. Replace the value with that position
```c
// Pseudocode
int sorted[n]; // copy of stack a values
sort(sorted, n); // sort the copy
for each element e in stack_a:
    e->index = find_position(e->value, sorted, n);
```

> The temporary sort used here is internal to your C program.
> It does not generate any Push_swap operations and is not counted
> toward your operation total.

### Key Point
After this step, you never need to compare raw values again.
All sorting logic works on indices `0` to `n-1` exclusively.

---

## Disorder Metric

The disorder metric is a mandatory computation defined by the subject. It must be calculated **once**, after index normalization,
before any operations are executed on the stack.

### What It Is
The disorder is a value between `0.0` and `1.0` that measures how
far stack `a` is from being sorted.

| Value | Meaning |
|---|---|
| `0.0` | Stack is already sorted |
| `1.0` | Stack is in the worst possible order |
| Between | Stack is partially sorted |

### How It Is Calculated
Every pair of elements `(i, j)` where `i < j` is examined.
If the element at position `i` is greater than the element at 
position `j`, that pair counts as a **mistake**.

The disorder is the ratio of mistakes to total pairs:
`disorder = mistakes / total_pairs`
Where:
`total_pairs = n * (n - 1) / 2`
```C
//Pseudocode from subject
function compute_disorder(stack a):
	mistakes = 0
	total_pairs = 0
	for i from 0 to size(a)-1:
		for j from i+1 to size(a)-1:
			total_pairs += 1
			if a[i] > a[j]:
			mistakes += 1
	return mistakes / total_pairs
```
> This is an O(n²) computation. For n = 500 this is 124,750 pair
> comparisons — fast enough to run once at startup with no impact
> on performance.

### How It Is Used

The disorder value drives the adaptive strategy:

| Disorder range | Required complexity |
|---|---|
| `disorder < 0.2` | O(n) |
| `0.2 ≤ disorder < 0.5` | O(n√n) |
| `disorder ≥ 0.5` | O(n log n) |

> Compute disorder on the **normalized** stack, not the raw values.
> Since normalization does not change the order of elements, the
> result is identical — but the comparison logic is simpler.

### Benchmark Output

When `--bench` is passed, the disorder is printed to `stderr`
as a percentage with two decimal places:
`[bench] disorder: 49.93%`

---

## The Four Strategies
Push_swap requires four distinct sorting strategies. Each strategy
is a different approach to sorting stack `a` using only the allowed
operations. The strategy is selected via a flag at runtime. If no
flag is provided, the adaptive strategy is used by default.

### What Is An Algorithm
An algorithm is a fixed sequence of steps that solves a problem.
For sorting, the problem is always the same: put things in order.
The difference between algorithms is *how* they get there and how
many steps they need to do it.

The number of steps an algorithm needs is described using **Big-O
notation**, which measures how the operation count grows as the
input size `n` grows:

| Notation | Name | Approx. ops for n=500 |
|---|---|---|
| O(n) | Linear | ~500 |
| O(n log n) | Log-linear | ~4500 |
| O(n√n) | Sub-quadratic | ~11000 |
| O(n²) | Quadratic | ~250000 |


### Strategy 1 — Simple O(n²)
These are the naive algorithms. Easy to understand and implement,
but expensive at scale. Required to handle the baseline case.

#### Selection Sort
Find the minimum value in stack `a`, rotate it to the top, then
push or place it in its correct position. Repeat for every element.
Natural fit for push_swap since rotating to a minimum is a direct
use of `ra` and `rra`.

#### Insertion Sort
Take one element at a time from stack `a` and insert it into its
correct position relative to what is already sorted. Works well
for small stacks or nearly sorted inputs.

#### Bubble Sort
Repeatedly swap adjacent elements that are out of order using `sa`
until no swaps are needed. Maps poorly onto the push_swap operation
set and is the least efficient option here.

> **Recommendation:** Selection sort or insertion sort.
> Bubble sort is the weakest fit for this project's operation model.


### Strategy 2 — Medium O(n√n)
These algorithms divide the stack into groups and sort group by
group, reducing the number of full passes needed.

#### Chunk Sort
Divide the normalized index range `0` to `n-1` into √n equal chunks.
Push all elements belonging to the first chunk to stack `b`, then
the second chunk, and so on. Once all elements are in `b` in chunk
order, pull them back to `a` in sorted order using rotations to
always bring the correct element to the top.

This is the most common medium strategy used in push_swap and maps
naturally onto the two-stack model.

#### Bucket Sort Adaptation
Similar to chunk sort but buckets are defined by value ranges rather
than equal-size index chunks. Slightly more flexible but harder to
tune for consistent operation counts.

#### Block-based Partitioning
Partition the stack into blocks and sort each block independently
before merging. Similar in spirit to chunk sort with different
boundary logic.

> **Recommendation:** Chunk sort. It is the most natural fit for
> the two-stack operation model and is well documented.


### Strategy 3 — Complex O(n log n)
These are the efficient algorithms used in real-world sorting.
They scale well to large inputs and are required to meet the
excellent performance benchmarks.

#### Radix Sort (LSD)
Works on the **binary representation** of the normalized indices.
Each pass sorts by one bit, starting from the least significant bit
(LSD) and moving toward the most significant bit. Elements whose
current bit is `0` stay in `a`, elements whose bit is `1` get pushed
to `b` with `pb`. After each pass, all elements are pulled back to
`a` with `pa`. After `log2(n)` passes, the stack is sorted.

No comparisons are needed — only bit checks. Produces consistent
operation counts regardless of input order.

#### Merge Sort Adaptation
Divide stack `a` in half, sort each half recursively, then merge
the two sorted halves back together. Difficult to implement cleanly
with only two stacks and the limited operation set.

#### Quick Sort Adaptation
Pick a pivot value, partition elements smaller than the pivot into
stack `b` and leave larger elements in `a`, then recurse on each
partition. Tricky to implement correctly within the operation
constraints and can degrade to O(n²) with a bad pivot choice.

#### Heap Sort Adaptation
Build a heap structure from the stack elements and extract the
minimum repeatedly. Complex to simulate with stack operations alone.

#### Binary Indexed Tree
An advanced data structure approach. Significantly more complex
to implement and offers no practical advantage over radix sort
in this context.

> **Recommendation:** Radix sort (LSD). It is the most natural fit
> for the two-stack model, requires no comparisons, and produces
> consistent results across all input orders.


### Strategy 4 — Adaptive
The adaptive strategy is not a named algorithm. It is a controller
that measures the **disorder metric** of stack `a` before any moves
and selects one of the other three strategies based on the result.

| Disorder range | Selected complexity |
|---|---|
| `disorder < 0.2` | O(n) |
| `0.2 ≤ disorder < 0.5` | O(n√n) |
| `disorder ≥ 0.5` | O(n log n) |

The thresholds and internal techniques used in each regime must be
documented in your `README.md` along with a complexity argument for
each case.

> This is the default strategy when no flag is passed.
> The disorder must be computed **before** any operations run.

---

## Benchmark Requirements
The subject defines performance targets measured in total operation
count. These are verified during evaluation using the provided checker.
All benchmarks use randomly generated inputs.


### Operation Count Targets

#### 100 Random Numbers

| Performance | Max Operations |
|---|---|
| Minimum (pass) | < 2000 |
| Good | < 1500 |
| Excellent | < 700 |

#### 500 Random Numbers

| Performance | Max Operations |
|---|---|
| Minimum (pass) | < 12000 |
| Good | < 8000 |
| Excellent | < 5500 |


### How To Test
Generate a random input and count the operations your program outputs:
```bash
# Test with 100 numbers
ARG=$(shuf -i 0-9999 -n 100); ./push_swap $ARG | wc -l

# Test with 500 numbers
ARG=$(shuf -i 0-9999 -n 500); ./push_swap $ARG | wc -l
```

Verify correctness by piping into the checker:
```bash
ARG=$(shuf -i 0-9999 -n 500)
./push_swap $ARG | ./checker_linux $ARG
```

The checker prints `OK` if the stack is correctly sorted and `KO`
if it is not.


### Benchmark Mode
Running with `--bench` outputs diagnostic information to `stderr`:
```bash
./push_swap --bench $(shuf -i 0-9999 -n 500) 2> bench.txt
cat bench.txt
```
```
[bench] disorder:    49.93%
[bench] strategy:    Adaptive / O(n√n)
[bench] total_ops:   7997
[bench] sa: 0  sb: 0  ss: 0  pa: 500  pb: 500
[bench] ra: 4840  rb: 1098  rr: 0  rra: 0  rrb: 1059  rrr: 0
```
---

## Task Assignment

| Task | Assigned To
|---|---|
| Input validation & error handling | Martin |
| Stack data structure & operation helpers | Daniel|
| Index normalization | Martin |
| Disorder metric | Daniel |
| Simple strategy — O(n²) | Daniel |
| Medium strategy — O(n√n) | Martin |
| Complex strategy — O(n log n) | Daniel & Martin |
| Adaptive strategy | Daniel |
| Makefile | Martin |
| README.md | Daniel & Martin |

> Assigned To: **Martin** or **Daniel**

---

## Decisions

| Decision | Choice |
|---|---|
| Data structure | Singly linked list with `tail` pointer |
| Simple strategy — O(n²) | Selection sort |
| Medium strategy — O(n√n) | Chunk sort |
| Complex strategy — O(n log n) | TBD |
| Adaptive strategy | TBD — depends on complex choice |

> Update this section as decisions are made.

---

## Project Structure

### Struct Definition
```c
typedef struct s_stack
{
	int				content;
	struct s_stack	*next;
}	t_stack;
```

> The stack is represented as a singly linked list. Each node holds
> an integer `content` and a pointer to the `next` node.
> Both stacks `a` and `b` are of type `t_stack *`.

---

### File Structure

| File | Responsibility |
|---|---|
| `main.c` | Entry point, argument parsing, stack initialization |
| `push_swap_utils.c` | Extra utils functions |
| `push_swap.h` | All headers, structs, and function prototypes |
| `validate.c` | Input validation and error handling |
| `arg_norm.c` | Index normalization — maps values to ranked indices |
| `simple_strat.c` | Simple sorting strategy — O(n²) — selection sort |
| `medium_strat.c` | Medium sorting strategy — O(n√n) — chunk sort |
| `complex_strat.c` | Complex sorting strategy — O(n log n) — TBD |
| `adaptive_strat.c` | Disorder metric + strategy selector |
| `swap_op.c` | `sa`, `sb`, `ss` |
| `push_op.c` | `pa`, `pb` |
| `rotate_op.c` | `ra`, `rb`, `rr` |
| `rev_rotate_op.c` | `rra`, `rrb`, `rrr` |

> `adaptive_strat.c` computes the disorder metric and calls the
> appropriate strategy based on the result. It does not implement
> any sorting logic itself.