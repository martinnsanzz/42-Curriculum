*This project has been created as part of the 42 curriculum by masanz-s and danmorei*  
[Martin personal github](https://github.com/martinnsanzz) / 
[Daniel personal github](https://github.com/Danitell0)

---

# DESCRIPTION
**push_swap** is a sorting algorithm project from the 42 curriculum. The goal is 
to sort a stack of integers using two stacks (`a` and `b`) and a restricted set 
of operations, while minimizing the total number of operations used.

The program reads a list of integers from the command line and a set of flags, 
validates the input, normalizes the values to contiguous ranks, and then applies 
one of three sorting  strategies depending on the degree of disorder in the input or the specified alghoritm to use depending on the user flags. The selected strategy outputs the sequence of operations to stdout and if flag `--bench` is written the stats of the alghoritm 
to stderr.

---

# INSTRUCTIONS

**Requirements:** A C compiler (`cc`), `make`, and a compiled `libft` in the `libft/` subdirectory.

**Compilation:**
```bash
make
```
This builds `push_swap` and compiles `libft` as a dependency.

**Usage:**
```bash
./push_swap [--simple|--medium|--complex|--adaptive] [--bench] [ARG]
```

**Examples:**
```bash
./push_swap 3 1 4 1 5            # error: duplicate
./push_swap 5 3 1 2 4            # sorts and prints operations to stdout
./push_swap --complex 5 3 1 2 4  # forces the radix sort strategy
./push_swap --bench 5 3 1 2 4    # prints stats to stderr and stats to stdout
./push_swap --complex --bench 5 3 1 2 4  # force strategy + bench mode
```

**Flags:**
- `--simple` / `--medium` / `--complex` / `--adaptive` — force a specific strategy (default: adaptive)
- `--bench` — prints operations to stdout and print algorithm stats to stderr (disorder score, strategy used, operation counts)

**Testing**

fish:
```fish
shuf -i 0-9999 -n 500 > args.txt ; ./push_swap --bench (cat args.txt) 1> actions.txt
```

bash:
```bash
shuf -i 0-9999 -n 500 > args.txt && ./push_swap --bench $(cat args.txt) 1> actions.txt
```

**Additional Makefile targets:**
```bash
make clean    # remove object files
make fclean   # remove objects and binary
make re       # fclean + all
make debug    # compile with -g for use with gdb/lldb
```

---

# ALGORITHMS AND DATA STRUCTURES

**Data structure — Linked list**

Both stacks are implemented as singly linked lists (`t_list`) rather than fixed-size arrays with an index. Each node holds a heap-allocated `int` as its content. This choice means stack operations (push, rotate, reverse rotate) are pointer manipulations rather than index arithmetic, which maps naturally to the conceptual model of a stack and avoids any hardcoded size limit.

### Input normalization

Before any sorting strategy runs, the integer array is normalized to contiguous 
ranks in `[0, n-1]`. For each element, its rank equals the count of elements strictly
smaller than it. This lets all three strategies operate on small non-negative integers
regardless of the original input range, which is especially important for the radix sort.

*Implemented by Martin.*

### Adaptive strategy selection

In adaptive mode (default), the disorder of the input is computed as the inversion ratio: the number of pairs `(i, j)` where `i` appears before `j` but holds a greater value, divided by the total number of pairs. This produces a float in `[0.0, 1.0]`.

- disorder `< 0.2` → **Simple** (selection sort, O(n²))
- disorder `0.2–0.5` → **Medium** (chunk-based, O(n√n))
- disorder `> 0.5` → **Complex** (radix sort, O(n log n))

The idea is that nearly-sorted inputs should not pay the overhead of a full radix sort, while heavily disordered inputs should not be handled by a quadratic algorithm.

*Implemented by Daniel.*

### Simple strategy — Selection sort (O(n²))

At each iteration, the minimum value in `stack_a` is located by index. The cheaper rotation direction (forward `ra` or reverse `rra`) brings it to the top, then it is pushed to `stack_b`. Once `stack_a` is empty, all elements are pushed back to `stack_a` in order. This is correct but scales poorly; it is only used on nearly-sorted inputs.

*Implemented by Daniel.*

### Medium strategy — Chunk-based sort (O(n√n))

The stack size is divided into `⌈√n⌉` chunks of equal size (ceiling division for non perfect square roots). Each chunk covers a contiguous range of normalized ranks. The algorithm iterates chunk by chunk: for every element in `stack_a` belonging to the current chunk, the cheapest rotation brings it to the top and it is pushed to `stack_b`. Once all chunks are processed, `stack_b` is drained back into `stack_a` by repeatedly locating the current maximum, rotating it to the top, pushing it to `stack_a`, and reverse-rotating to restore order. The result is `stack_a` sorted in ascending order.

*Implemented by Martin.*

### Complex strategy — Radix sort (O(n log n))

The algorithm performs one pass per bit position, up to `⌊log₂(n-1)⌋ + 1` passes. On each pass, every element in `stack_a` is inspected at bit position `i`: elements with that bit unset are pushed to `stack_b`, elements with it set are rotated to the bottom via `ra`. After the pass, all of `stack_b` is pushed back to `stack_a`. Correctness depends entirely on values being normalized to `[0, n-1]` before entry.

*Implemented by Martin.*

### Bench mode

When `--bench` is passed, a summary is written to stderr: the disorder score as a percentage, the strategy that was selected and its time complexity class, the total operation count, and a per-operation breakdown (sa, sb, ss, pa, pb, ra, rb, rr, rra, rrb, rrr).

*Implemented by Martin.*

### Tasks Division

| Module | Martin | Daniel |
|---|---|---|
| Input validation | ✅ | |
| Linked list creation | ✅ | |
| Stack operations (sa, sb, pa, pb, ra, rb, rr, rra, rrb, rrr) | | ✅ |
| Disorder metric | | ✅ |
| Simple strategy (selection sort) | | ✅ |
| Adaptive strategy selection | | ✅ |
| Medium strategy (chunk-based) | ✅ | |
| Complex strategy (radix sort) | ✅ | |
| Bench mode | ✅ | |

---

# AI USAGE:

Claude by Anthropic was used in this project for the following:
- **As a teacher:** Explaining the time complexity (Big O) of the three sorting algorithms and how to reason about algorithmic efficiency.
- **Visualizing stack operations:** Helping trace and understand stack state during operation sequences, making it easier to debug and reason about algorithm correctness.
- **Rewriting comments:** Improving clarity and precision of function-level documentation.
- **Testing:** Discussing edge cases and verifying expected behavior of the sorting strategies.

AI was NOT used to write or generate any code. All function bodies were written by Martin and Daniel.

---

# RESOURCES

[Medium - Explanation of Push_Swap](https://medium.com/@jamierobertdawson/push-swap-the-least-amount-of-moves-with-two-stacks-d1e76a71789a)

[Wikipedia - Big O notation](https://en.wikipedia.org/wiki/Big_O_notation)

[YT - 10 Sorting Alghorithms Easily Explain](https://www.youtube.com/watch?v=rbbTd-gkajw)

[YT - Alghorithms explained for begginers](https://www.youtube.com/watch?v=JJkWemM03Lg)

[YT - CS50 Alghorithms](https://www.youtube.com/watch?v=iCx3zwK8Ms8)

[Comparison Sorting Alghorithms](https://www.cs.usfca.edu/~galles/visualization/ComparisonSort.html)

[Sort Visualizer](https://sortvisualizer.com/)

[Push Swap Visualizer](https://saadloukili.github.io/Push-Swap-Visualizer/index.html)

[N log N alghorithms](https://www.educative.io/answers/nlogn-sorting-algorithms)

[GeeksforGeeks - Radix Sort](https://www.geeksforgeeks.org/dsa/radix-sort/)

[YT - Bitwise Operators](https://www.youtube.com/watch?v=igIjGxF2J-w)

---
