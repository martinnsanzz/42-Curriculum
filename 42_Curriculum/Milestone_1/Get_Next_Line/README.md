*This project has been created as part of the 42 curriculum by Martin Sanz Sanchez*  

---

# DESCRIPTION

**Get Next Line** is a C function that every time it gets called it reads and returns
one line at a time from a file descriptor. Each call returns the next line, including the
terminating `\n` if present, or `NULL` on EOF or error.

The main mechanism of this project is the use of **static variables** to preserve the data
of what was read between calls, specially buffered data that was read past the
previous `\n` but wasn't yet returned.

The mandatory part only handles a single file descriptor via one static `char *` buffer
while the bonus part handles multiple fd's at the same time (1024 simultaneously, which is
the default soft limit for open fd's in Linux).

---

# INSTRUCTIONS

**Compilation:**
```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c
```

The `-D BUFFER_SIZE=n` flag sets the read chunk size. The default value in the header
is `4096` if the flag is omitted which is the standard page size in most Unix-like systems. Any positive integer is valid.

**Bonus (multiple fd support):**
```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c
```

**Usage example:**
```c
#include "get_next_line.h"
#include <stdio.h>
#include <fcntl.h>

int main(void)
{
    int   fd;
    char  *line;

    fd = open("file.txt", O_RDONLY);
    while (1)
    {
        line = get_next_line(fd);
        if (line == NULL)
            break ;
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

The caller is responsible for freeing each returned line. `get_next_line` also works on `fd = 0` (standard input).

---

# RESOURCES

For the creation of this project the following resources were used:
- [Linux manual page - read()](https://man7.org/linux/man-pages/man2/read.2.html) — Official page for `read()`, used to understand what the function was and how to implement it.
- [GeeksforGeeks - Static Variables in C](https://www.geeksforgeeks.org/c/static-variables-in-c/) — Step by step explanation on how to use static variables and what they are.
- [Code Quoi - Handling a File by its Descriptor in C](https://www.codequoi.com/en/handling-a-file-by-its-descriptor-in-c/) — Used to understand what `fd` was and to test the function with custom `.txt` files. Helpful for the bonus part to handle multiple fds.

**AI usage:**

Claude by Anthropic was used in this project for the following:
- **As a teacher:** To understand complex concepts such as freeing memory correctly, dereferencing pointers, passing a pointer to a pointer, static variables, bytes, and `read()`.
- **Rewriting comments:** In my personal GitHub, all functions have a block comment explaining what it does, parameters, return values, behaviour, and extra notes. These block headers were removed for the submission.
- **Testing:** I wrote several `.txt` files to find edge cases where `get_next_line` wouldn't work as intended.

AI was NOT used to write or generate any code. All function bodies were written by myself.

---

# ALGORITHM

Two common approaches exist for GNL: a **linked list** (storing chunks as nodes) or a
**single dynamic string** (joining chunks into one buffer). This implementation uses the
string approach because string manipulation with `ft_strjoin` is simpler to reason about
than managing node allocation, traversal, and freeing. It also makes error handling more
straightforward — a `NULL` return from any string operation is easy to catch and propagate.

Each call to `get_next_line` follows these four steps:

> On first call `buffer = NULL`, so it is allocated to an empty string.

1. **`fill_buffer`** — reads from `fd` in chunks of `BUFFER_SIZE` bytes, appending each chunk to the static buffer via `ft_strjoin`, until a `\n` is found or EOF is reached.
2. **`extract_line`** — scans the buffer for `\n` and allocates a new string containing exactly one line (up to and including `\n`, or the full buffer if no `\n` exists).
3. **`trim_buffer`** — allocates a new string containing everything after the first `\n`, discarding the extracted line while preserving any leftover data for the next call.
4. The old buffer pointer is freed after trimming; the new trimmed buffer becomes the new static state.