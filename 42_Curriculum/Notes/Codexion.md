# Codexion

## Dining Philosopers Probems

The dining philosophers problem is a famous problem in computer science used to illustrate
common issues in concurrent programming. The problem was originally formulated in 1965 by Edsger
Dijkstra, and is stated as follows:

```
X amount of philosophers sit at a round table with bowls of food.
Forks are placed in front of each philosopher.
There are as many forks as philosophers.
All day the philosophers take turns eating, sleeping, and thinking.
A philosopher must have two forks in order to eat, and each fork
may only be used by one philosopher at a time. At any time a
philosopher can pick up or set down a fork,
but cannot start eating until picking up both forks.
The philosophers alternatively eat, sleep, or think.
While they are eating, they are not thinking nor sleeping,
while thinking, they are not eating nor sleeping,
and, of course, while sleeping, they are not eating nor thinking.
```

## Concepts

### Process vs Threads

A **process** is an instance of a computer program that is currently being executed by the OS.  While a program is a passive, static file stored on disk containing instructions, a
process is a dynamic, active entity that includes the program code, its current activity,
allocated system resources (such as memory, CPU time, and I/O devices), and the execution state.

A `multitasking operating` system may just switch between processes to give the appearance of
many processes executing simultaneously (that is, in parallel), though in fact only one process
can be executing at any one time on a single CPU (unless the CPU has multiple cores, then
multithreading or other similar technologies can be used).

A **Thread** is a component of a `process`. Is the smallest sequence of programmed instructions that
can be managed independently by a `scheduler`.

The multiple threads of a given process may be executed *concurrently* (via multithreading
capabilities), sharing resources such as memory, while different processes do not share these 
resources.

| Aspect | Processes | Threads |
|---|---|---|
| Independence | Run independently of each other | Exist as part of, and depend on, a parent process |
| State | Carry their own separate state information | Share the state of their parent process |
| Memory/Resources | Have their own memory and resources | Share memory and resources with other threads in the same process |
| Address space | Each has a separate address space | Share the same address space within a process |
| Communication | Communicate only via OS-level IPC mechanisms | Can access shared memory directly |
| Context switch speed | Slower to switch between | Faster to switch between (same process) |

### Scheduler
Scheduling is the action of assigning resources to perform tasks.
- The resources may be: Processors, Network links or expansion cards...
- The tasks may be: Threads, Processes or Data Flows...

Schedulers are often designed so as to keep all computer resources busy (as in load balancing),
allow multiple users to share system resources effectively, or to achieve a target
quality-of-service.

### Mutexes
A Mutex (from `Mutual Exclusion`) is a synchronization object, a variant of a semaphore. Mutexes
are typically used to protect a shared resource from concurrent access. A task acquires and
blocks a mutes, then accesses the resource and the releases back the mutex.

A mutex is a low-level syncronization primitive which biggest downside is **deadlock**

- A deadlock can occur when two tasks try to acquire the one of two mutex and wait for the other
one to be release simultaneously.

There exists a simple technique of deadlock prevention when mutexes are seized in some fixed
order. This is discussed in depth in the `Dining philosophers` problem.

### Data Races / Race Condition
Race condition is a design or implementation bug where the outcome of a process or function
can vary depending on the scheduling/processing of other threads or processes. It usually happens
when two processes depend or manipulate the same piece of memory or multiple processes attempt to
access the same file.

In any situation where a resource is shared and access by multiple threads a race condition is
possible. One way of avoiding this happening is with the use of mutexes or specific contention
avoidance.

`Threads` implicitly share most or all of their memory. Programming languages with support for
threading supply various forms of locking and synchronization primitives.

Most solutions to this problems revolve around preventing access to the same data at the same
time.

`Locks` are mechanism that force one thread to wait until another thread is finished with a
piece of data. Various sorts of locking primitives include `semaphores` and `mutexes` (mutual
exclusion objects).

### Thread scheduling
SCHED_DEADLINE

SCHED_FIFO