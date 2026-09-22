*This project has been created as part of the 42 curriculum by masanz-s*

[Martin GitHub](https://github.com/martinnsanzz)

# Codexion
*We do not want a Coder to pick up a cable that's being used up by his neighbor.*


## Description

**Codexion** is a C based project from the 42 curriculum.

---

## Instructions


---

## Additional requirements


---

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

---

## AI Usage

**AI was NOT used to generate code.** All function implementations were written by Martin™.

**Where AI was used:**

This README.md file was done by **human fingers, sweat and tears**. No AI wrote a single
line of text :)

---

# Project Notes (TEMP)

## Program arguments !!

Your program must take the following arguments (all mandatory):

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```
- **number_of_coders**: The number of coders and also the number of dongles.
- **time_to_burnout (in milliseconds)**: If a coder did not start compiling within
time_to_burnout milliseconds since the beginning of their last compile or the
beginning of the simulation, they burn out.
- **time_to_compile (in milliseconds)**: The time it takes for a coder to compile.
During that time, they must hold two dongles.
- **time_to_debug (in milliseconds)**: The time a coder will spend debugging.
- **time_to_refactor (in milliseconds)**: The time a coder will spend refactoring.
After completing the refactoring phase, the coder will immediately attempt to
acquire dongles and start compiling again.
- **number_of_compiles_required**: If all coders have compiled at least this
many times, the simulation stops. Otherwise, it stops when a coder burns
out.
- **dongle_cooldown (in milliseconds)**: After being released, a dongle is unavailable until its cooldown has passed.
- **scheduler**: The arbitration policy used by dongles to decide who gets them
when multiple coders request them.



**Note:**The value must be exactly one of: `fifo` or `edf`.
- fifo means First In, First Out: the dongle is granted to the coder whose
request arrived first.
- edf means Earliest Deadline First with deadline = last_compile_start +
time_to_burnout.