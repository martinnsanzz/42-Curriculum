*This project has been created as part of the 42 curriculum by masanz-s*

[Martin GitHub](https://github.com/martinnsanzz)

# Codexion
*We do not want a Coder to pick up a cable that's being used up by his neighbor.*


## Description

**Codexion** is a C based project from the 42 curriculum. In this project we are presented
with the computer science `Dining Philosophers Problem` which introduces thread syncronization
as a concept. For the project instead of **philosophers** we have **coders** and instead of
**forks** we have dongles.

### 📊 The Challenge:**
There is one or more coders sit down in a round table with a dongle in their left or right
side. Each of them have 3 actions:

- Compile
- Debug
- Refactor

This actions are done in order and each action have their own **time** in `ms`. This settings
are declared in the CLI on runtime. Once refactor the coder goes right back to compiling.
If a coder doesn't compile before the `time_to_burn_out` timer hits 0 it will burn_out (A
message announcing the coders burnout will be displayed no more than **10ms** after the actual
burnout, nothing else should display after and simulation **MUST STOP**).

There are as many dongles as coders. In order to compile a coder needs to hold one dongle in
each hand. If each coder has compiled a certain amount of times the simulation can stop
succesfully. Coder's can't communicate with each other, which means they dont know who's holding
the dongles, if a coder has burnout or if a coder has finished the compilation.

To visualize the simulation here is a diagram that represents 4 coders in a round table:

```txt
					CODER 1
					(Compile)
					/		\
			Dongle D		Dongle A
		(Taken by 1)		(Taken by 1)
			 /					   \
		  CODER 4				  CODER 2
	(Wait Dongle D)        	   (Wait Dongle A)
			\                     /
		  Dongle C            Dongle B
		(Taken by 4)     	(Taken by 2)
					\       /
					 CODER 3
					  (Wait)
```
### 📝 Rules:

- Each coder need to be represented by a **thread** (using `pthread_create`).
- If there is **1 coder**, it will **burns out** since there is only **1 USB dongle**.
- To prevent coders from duplicating dongles, **dongle's state must be protected** with a 
**mutex** (`pthread_mutex_t`). A condition variable (`pthread_cond_t`) may be
used to manage waiting queues.
- There is a **scheduler** to manage priority:
    - **FIFO**: First In, First Out. Follow the queue to give dongles.
    - **EDF**: Earliest Deadline First. The coder that will burn out first will take the 
    dongles.
- The program must guarantee liveness. So coders should not burn out under edf scheduling, provided the parameters are feasible.
- A separate **monitor** thread must detect burnout precisely and stop the simulation.
The burnout log must be printed **within 10 ms** of the actual burnout time.
- Logging must be **serialized** so that two messages never interleave on a single line (use a **mutex** to protect output).
- Code must compile with **-Wall -Wextra -Werror -pthread**.
- A priority queue **(heap)** for **FIFO**/**EDF** scheduling must be implemented.
- No memory leaks.
- Global variables are forbidden.
- Logs must follows rules (see below).
- Those arguments are mandatory:

|Arguments|Value|Description|
|:-------:|:---:|:---------:|
|number_of_coders|>=1|Number of coders ans also the number of donglers|
|time_to_burnout|>=0|(in ms)Time before a coder will burns out since the last compilation. Start at the beginning of the simulation|
|time_to_compile|>=0|Time it takes for a coder to compile. During that time, it need to hold two dongles|
|time_to_debug|>=0|Time a coder spend debugging|
|time_to_refactor|>=0|Time a coder will spend refactoring. After completing this phase, it will immediately attempts to acquire dongles and start compiling again|
|number_of_compiles_required|>=0|If all coders have compiled at least this many times, the simulation stops||
|dongle_cooldown|>=0|Time before a dongle can be used again||
|scheduler|`fifo` or `edf`|The arbitratin policy used by dongles||

### 🏷️ Authorized Functions:

|Thread managements|Mutex|Thread cond|Memory management|Printing|Other|
|:----------------:|:---:|:----------:|:--------------:|:------:|:---:|
|pthread_create|pthread_mutex_init|pthread_cond_init|malloc|write|gettimeofday|
|pthread_join|pthread_mutex_lock|pthread_cond_wait|free|fprintf|usleep|
||pthread_mutex_unlock|pthread_cond_timedwait|memset|printf|strcmp|
||pthread_mutex_destroy|pthread_cond_broadcast|||strlen|
|||pthread_cond_destroy|||atoi|

### 💻 Logs:

Any state change of a coder must be formatted as follows:
```bash
- timestamp_in_ms X has taken a dongle
- timestamp_in_ms X is compiling
- timestamp_in_ms X is debugging
- timestamp_in_ms X is refactoring
- timestamp_in_ms X burned out
```

*X will be the coder number*

Rules:
- A displayed state message should not be mixed up with another message.
- A message announcing that a coder burned out should be displayed no more than 10 ms after the actual burnout.

### 🏆 Goal:
The goal of Codexion is to simulate coders sharing a limited set of dongles without deadlock, 
starvation or data races. The simulation must run until one of two things happens:

- **Success:** every coder has compiled at least `number_of_compiles_required` times.
- **Failure:** a coder reaches `time_to_burnout` without starting a new compile. The simulation stops immediately, and the burnout is logged within 10 ms of the actual burnout.

To get there, the project implements:

- **One thread per coder**, plus a **monitor thread** that detects burnouts precisely and stops the simulation, and a **scheduler thread** that decides who gets the dongles.
- **A scheduler built on a custom binary min-heap** (no standard priority queue):
  - `fifo`: the coder that asked first gets the dongles first.
  - `edf`: the coder closest to burning out gets the dongles first, so no coder burns out as long as the parameters are feasible.
- **Safe dongle sharing**: each dongle's state is protected by its own mutex, and a coder only compiles when it holds both dongles. No dongle can be taken twice, and each one respects `dongle_cooldown` after it is released.
- **Serialized logging**: a mutex guarantees that two messages never interleave on one line, and nothing is printed after a burnout.
- **Clean resource handling**: no global variables, no memory leaks, and all mutexes, condition variables and threads are destroyed or joined on every exit path.

The project also trains the core concurrency skills of the 42 curriculum: thread synchronization, avoiding deadlock and starvation, and writing code that is correct under any thread interleaving, not just on one lucky run.

## Instructions
### Clone repository
To use this project you first need to clone the repository in your directory.

```bash
git clone git@github.com:martinnsanzz/Codexion.git
```

### Run make
This will compile the program with the required flags.

```bash
make
```

### Run the program
Use the mandatory arguments, program will return a specific error for wrong input.

```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles_required> <dongle_cooldown> <scheduler>
```

Here I probide an example to see the code correctly.
`time_to_burnout (3000)` is well above one compile+debug+ref (200+200+200 = 600) +
`dongle_cooldown (50)`, so no coder should burn out; the program must stop on its own once
every code least 10 times.

```bash
./codexion 5 3000 200 200 200 10 50 edf
```

> To stress-test, set `time_to_burnout` slightly above the feasible minimum:
> `ceil(n / floor(n/2)) * (time_to_compile + dongle_cooldown)`, and never below
> compile + debug + refactor. Run each case many times, since OS scheduling adds
> a few ms of drift per round. Parameters within a few ms of the minimum can burn
> out for that reason alone.

## Additional requirements

### 🐨 How it works?

1. **Initialization:**
The first step is to initialize the structures, threads, conditions and mutexes.
Stages, in this order:
   1. **Program struct:** Parses the CLI rules and sets start_time of the program.
   2. **Scheduler struct:** Must be done before the coder, since each coder stores a pointer to it.
   3. **Dongles struct**
   4. **Coders struct**
   5. **Threads:** Monitor, scheduler, coders. They are created only after everything before it is full
                   populated, so no thread can observe a half-initialised structure.

2. **Execution:**
For this program to work apart from the coders threads (Which funcionality has being already explained), we need
two extra threads.

<u>Monitor Thread</u>

This thread is continuosly checks if any coder has burn_out or if all coders have finished compiling. If any
of this conditions become true all the other threads (Coders and scheduling) will receive a signal through
a pointer to the program structure.

<u>Scheduler Thread</u>

The scheduler thread is in charge of telling a coder when they can compile. Each coder 
pushes a request into a min-heap binary tree. The scheduler scans the waiting requests and 
grants the best one (by `EDF` deadline or `FIFO` arrival, with arrival order as tie-break) 
whose two dongles are both free and out of cooldown. A coder gets both dongles or none.

3. **Clean-up:**
Once all coders have finished compiling or a coder has burnout all threads will be joined, all
mutexes and conditions will be destroyed and all struct will be freed. This makes sure no
memory leaks are found with `valgrind` and due to the implementation strategy of the program
protecting the variables with mutexes the program is also `helgrind` compliant which is a tool
use to find race conditions.

>During initialization, if anything doesn't get set correctly (Mutex, malloc...), proper clean
>up happens so there's no option for memory leaks

### 🛸 Blocking cases handled
Due to the nature of threads multiple issues may occur. This is the solutions I implemented
for the following concurrency issues.

- **Deadlock:** A deadlock is a state where two or more threads cannot continue because each one is waiting for a resource held by another. In this project it would happen if every coder picked up one dongle and then waited forever for a second one to start compiling.

  I avoid it by making dongle acquisition all-or-nothing. The scheduler only grants a coder its turn when **both** of its dongles are free and out of cooldown, and it reserves both at the same moment. A coder therefore never holds one dongle while waiting for the other, so the "hold and wait" condition required for a deadlock can never occur.

  My first solution was based on the coder's id: even-numbered coders took their left dongle first and odd-numbered coders their right, so two neighbours never competed for the same dongle first. It worked by breaking the circular wait, but it relied on a fixed lock order and had to be reasoned about for odd coder counts. The scheduler-based approach replaced it because the guarantee no longer depends on the ordering.

- **Starvation Prevention:** Starvation happens when a coder keeps losing the race for its 
dongles. Without coordination, a coder that releases both dongles can immediately grab them 
again, before a neighbour that has been waiting gets a chance, and that neighbour may never 
get to compile and burn out.

  To prevent this, every request goes through a priority queue managed by the scheduler 
  thread. Each request stores a **key** and an **arrival number** (`seq`). The coder with 
  the smallest key is served first, and equal keys fall back to the smaller arrival number. 
  The only difference between the policies is how the key is computed:

  - **FIFO:** the key is the arrival sequence number (starting at 0). The oldest waiting 
  request has the smallest key, so among the coders that can take their dongles, the one 
  that asked first is served first.
  - **EDF:** the key is the deadline, `last_compile + time_to_burnout`, meaning the moment 
  the coder would burn out. The coder closest to burning out is served first. Coders with 
  equal deadlines (for example at the start, when all of them share the same `last_compile`) 
  are served in arrival order.

  Since the scheduler is the only one that hands out dongles, a coder that just released 
  them has to queue up again like everyone else, so it cannot starve its neighbours. A coder 
  whose dongles are busy is passed over only while they are busy. The scheduler re-checks on 
  every release, so it is served as soon as they are free, and only coders whose dongles are 
  free anyway can go ahead of it.

- **Cooldown handling:** Each dongle has a state, `DONGLE_FREE` or `DONGLE_TAKEN`, protected by its own mutex, and a `last_release` timestamp. All dongles start free. When a coder finishes compiling it sets both dongles back to free, and each dongle records the current time as its `last_release`.

  A dongle can be used again only once `dongle_cooldown` ms have passed since 
  `last_release`. The scheduler checks every waiting coder in the heap, not only the root, 
  so a coder whose dongles are busy never holds back a coder whose dongles are free. For 
  each waiting coder it looks at both dongles:

  - **Either dongle is taken:** that coder cannot go yet.
  - **Both are free but still cooling down:** that coder cannot go yet, and its cooldown end 
  time is remembered.
  - **Both are free and out of cooldown:** that coder can go.

  Among the coders that can go, the scheduler picks the one with the smallest `(key, seq)`, 
  marks both of its dongles as taken, removes it from the heap and wakes it up. If nobody 
  can go, the scheduler sleeps until a coder releases a dongle, or until the earliest 
  remembered cooldown end (`pthread_cond_timedwait`), whichever comes first.

  Since only the scheduler ever marks a dongle as taken, a dongle it has seen as free cannot 
  be taken by anyone else before it reserves it.

- **Precise burnout detection:** A dedicated monitor thread checks every coder in a loop, sleeping 1 ms between rounds so it does not spin on a CPU core. For each coder it reads the state once and skips coders that are `FINISH` or `COMPILING`, since a compile in progress already resets the burnout clock. For the others, it reads `last_compile` under `compile_lock` (the lock the coder writes it under) and compares `now - last_compile` with `time_to_burnout`.

  The 1 ms polling interval keeps the detection delay far below the 10 ms limit. When a burnout is found, the monitor:

  1. Locks `write_lock`, the same lock every coder holds while it checks the burnout flag and prints. No coder can print after the flag is raised based on a check made just before it.
  2. Raises `burn_out_flag` under the scheduler's `priority_lock` and broadcasts `turn_cond`, so the scheduler and every coder waiting for a turn wake up and see it.
  3. Sets the coder's state to `BURNOUT` and prints the burnout message, then unlocks `write_lock`.

  Coders sleeping through compile, debug or refactor use `interruptible_sleep`, which checks the flag, so they stop without waiting for their full duration. Once the monitor returns, the main thread joins all threads and cleans up.

- **Log serialization:** All output goes through a single mutex, `write_lock`. A coder takes it, checks that `burn_out_flag` is not set, prints its message and releases it. Two messages therefore never interleave on one line, and nothing is printed after the burnout message, because any coder that gets the lock afterwards sees the flag and stays silent.

  Each message is written with a single `printf` call that includes the timestamp, so the line is built and printed while the lock is held. Timestamps are in ms since the start of the simulation.

### 🕛 Thread synchronization mechanisms
The project uses only `pthread_mutex_t` and `pthread_cond_t`. There is no separate event type. The "event" is the pattern of one condition variable (`turn_cond`) plus a predicate that each waiter re-checks under the mutex.

<u>Primitives and what they protect</u>

| Primitive | Protects | Used by |
|:----------|:---------|:--------|
| `dongle->lock` (one per dongle) | `state` and `last_release` of that dongle | Scheduler (reserve, read), coders (release) |
| `priority_lock` | The heap, `next_seq`, each coder's `priority` flag, and the `burn_out` and `all_finish` flags as the scheduler and coders read them | Scheduler, coders in `wait_for_turn`, monitor |
| `turn_cond` (with `priority_lock`) | Wake-ups for the scheduler and for coders waiting for a turn | All of the above |
| `write_lock` | Log output, plus the check "has the burnout flag been raised?" before every print | Coders, monitor |
| `compile_lock` | `last_compile` and `total_compiles` | Coders (write), monitor (read) |
| `state_lock` | Each coder's `state` | Coders (write), monitor (read) |

<u>How threads coordinate</u>

**Coder to scheduler.** A coder calls `wait_for_turn`. Under `priority_lock` it pushes its 
request into the heap, broadcasts `turn_cond`, and sleeps in a `while (!coder->priority && 
!burn_out)` loop. The scheduler picks the best coder that can take its dongles, removes its 
request from the heap (`heap_remove_at`), sets `priority = true` and broadcasts. The wait is 
a loop on a predicate, so spurious wake-ups and broadcasts meant for other coders are 
harmless: the coder re-checks and goes back to sleep.

**Coder to scheduler on release.** When a coder finishes compiling, `unlock_dongles` sets both dongles to `DONGLE_FREE` and then calls `wake_scheduler`, so a scheduler that was waiting for a release re-checks the root coder.

**Cooldown.** If nobody in the heap can go but some dongles are only cooling down, the 
scheduler sleeps with `pthread_cond_timedwait` on `turn_cond` until the earliest cooldown 
ends. Any broadcast wakes it earlier, and it re-evaluates.

**Monitor to everyone.** When the monitor finds a burnout or all coders finished, `raise_flag` writes the flag under `priority_lock` and broadcasts `turn_cond` in the same critical section. The scheduler and every coder waiting for a turn wake up, see the flag, and exit.

<u>Race conditions and how they are prevented</u>

- **Two coders taking the same dongle.** Only the scheduler ever writes `DONGLE_TAKEN`, and coders only write `DONGLE_FREE`. A dongle the scheduler saw as free cannot be taken by anyone else before it reserves it, so check-then-reserve is safe. The `state` and `last_release` fields are read and written together under the dongle's own mutex (`get_dongle_state` and `set_dongle_state`), so the scheduler never sees a free dongle paired with an old release time.
- **Lost wake-up.** The scheduler holds `priority_lock` from the moment it evaluates `can_grant` until it sleeps, and `pthread_cond_wait` releases the mutex atomically. A coder that releases a dongle in between blocks in `wake_scheduler` until the scheduler is asleep, and then wakes it. `priority` is a flag, not a one-shot signal, so a coder that is granted before it starts waiting sees `true` and never sleeps.
- **Hold and wait.** A coder never locks a dongle mutex while waiting for another. The dongle mutexes are held only for an instant, to read or write a field.
- **Printing after a burnout.** A coder checks `burn_out_flag` and prints while holding `write_lock`. The monitor takes the same lock to raise the flag and print the burnout line, so nothing prints after it, and two lines never interleave.
- **Monitor reading coder data.** The monitor reads a coder's state once through `get_coder_state` (under `state_lock`) and compares the copy, and it reads `last_compile` under `compile_lock`, the lock the coder writes it under. This removed the data race that Helgrind reported on `coder->state`.
- **Stop flags.** `burn_out_flag` and `all_finish` are written under `priority_lock`, the same lock the scheduler and `wait_for_turn` read them under.

<u>Lock ordering</u>

When two locks are needed, the order is always:

1. `write_lock` then `priority_lock`: the monitor raises the burnout flag while it holds `write_lock`.
2. `priority_lock` then a dongle lock: the scheduler reads and reserves dongles while it holds `priority_lock`.

A coder never holds a dongle lock while taking `priority_lock`, which is why `unlock_dongles` releases the dongles first and calls `wake_scheduler` after. `compile_lock` and `state_lock` are never held while another lock is taken, so no cycle between locks is possible.

<u>Verification</u>

The program is run under `valgrind` (memory) and `valgrind --tool=helgrind` (data races) with parameters that avoid false burnouts caused by the tool's slowdown.

## Resources
This is a list of multiple resources use through out the life-cycle of the project

### C Specifics
- [Error Handling in C](https://www.geeksforgeeks.org/c/error-handling-in-c/)
- [Threads on single Processors](https://www.youtube.com/watch?v=M9HHWFp84f0)
- [Thread Management Function in C](https://www.geeksforgeeks.org/c/thread-functions-in-c-c/)
- [Mutexes in C](https://medium.com/@sherniiazov.da/mutexes-in-c-ac2b0f1a6d34)
- [Data Structures using C](https://www.geeksforgeeks.org/dsa/lmns-data-structures/)
- [Pthread.h library](https://pubs.opengroup.org/onlinepubs/7908799/xsh/pthread.h.html)
- [C Code Style Guidelines](https://www.cs.swarthmore.edu/~newhall/unixhelp/c_codestyle.html)
- [How to use gettimeday function](https://www.youtube.com/watch?v=cunJcNgtxMk)
- [How to use gettimeday function in C](https://linuxhint.com/gettimeofday_c_language/)
- [Conditions Variables](https://ycpcs.github.io/cs365-spring2017/lectures/lecture10.html)
- [Priority Queue](https://www.geeksforgeeks.org/c/c-program-to-implement-priority-queue/)

### Extra
- [The Dining Philosophers Problem](https://pages.mtu.edu/~shene/NSF-3/e-Book/MUTEX/TM-example-philos-1.html)
- [A simple Makefile Tutorial](https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/)
- [The Dining Philosopers Problem in C](https://medium.com/swlh/the-dining-philosophers-problem-solution-in-c-90e2593f64e8)
- [CPU Cores VS Threads Explained](https://www.youtube.com/watch?v=hwTYDQ0zZOw)
- [Mutex lock for Linux Thread Synchronization](https://www.geeksforgeeks.org/linux-unix/mutex-lock-for-linux-thread-synchronization/)
- [Multithreading vs Multiprocessing](https://www.youtube.com/watch?v=PgDaJEjlBuI)
- [Lock (Computer Science)](https://en.wikipedia.org/wiki/Lock_(computer_science))
- [Philosophers 42 Guide](https://medium.com/@ruinadd/philosophers-42-guide-the-dining-philosophers-problem-893a24bc0fe2)
- [Introduction to Threads](https://www.youtube.com/watch?v=LOfGJcVnvAk)
- [Guie to use Doxygen for C code](https://embeddedinventor.com/guide-to-configure-doxygen-to-document-c-source-code-for-beginners/)
- [Doxygen Documentation](https://doxygen.nl/manual/index.html)
- [Avoiding Deadlock](https://docs.oracle.com/cd/E19455-01/806-5257/6je9h0347/index.html)
- [Process/Thread Scheduling](https://os.cs.luc.edu/scheduling.html)
- [Binary Heap](https://en.wikipedia.org/wiki/Binary_heap)
- [Introduction to heap](https://www.youtube.com/watch?v=fJORlbOGm9Y)


## AI Usage

**AI was NOT used to generate code.** All function implementations were written by Martin™.

**Where AI was used:**
- To explain complex concepts.
- To help testing different edge cases.
- Writting accurate doxygens to the functions.
- Suggesting best way of organizing the files and functions within them for clarity and 
readability

This README.md file was done by **human fingers, sweat and tears**. No AI wrote a single
line of text :)
