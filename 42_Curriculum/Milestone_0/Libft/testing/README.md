# libft — Function Tester

Standalone test suite for `libft` functions.  
Each test file targets one function, compares output against expected values, and prints `PASS` / `FAIL` per case.

---

## Requirements

- `gcc` with `-Wall -Wextra -Werror`
- A compiled `libft.a` (built automatically by the Makefile)

---

## Directory structure

The `testing/` folder **must** sit inside the root of your `libft` project, alongside your source files and header.

```
libft/               ← root directory
├── ft_*.c           ← your function source files
├── libft.h          ← your header
├── Makefile         ← your main Makefile (optional)
└── testing/         ← this folder
    ├── Makefile     ← test Makefile
    └── src/
        ├── test_strnstr.c
        ├── test_strlen.c
        └── ...
```

If the `testing/` folder is placed anywhere else, the Makefile's `SRC_DIR = ../` path will not resolve your source files and the build will fail.

---

## Usage

Navigate into the `testing/` folder and run:

```bash
make run PARAM=test_ft_<function_name>.c
```

**Example:**

```bash
cd testing
make run PARAM=test_ft_strnstr.c
```

This will:
1. Compile all `ft_*.c` files from `../` into `libft.a`
2. Compile your test file and link it against `libft.a`
3. Run the executable and print results

## Cleanup

- make clean		-> Remove object files (.o)
- make fclean		-> Remove object files and libft.a
---
