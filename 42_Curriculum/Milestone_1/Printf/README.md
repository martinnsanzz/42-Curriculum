*This project has been created as part of the 42 curriculum by Martin Sanz Sanchez*  

---

# DESCRIPTION
This project is part of milestone 1 of the core curriculum at codam. The objective 
of `ft_printf` is to re-implement `printf()` function from the standard C library by 
building a static library (`libftprintf.a`).
This function handles a variadic format string and outputs to stdout formatted
text, returning the total number of characters written.

**Supported format specifiers:**

| Specifier | Output |
|---|---|
| `%c` | Single character |
| `%s` | String (`"(null)"` if NULL) |
| `%p` | Pointer address in hex (`"(nil)"` if NULL) |
| `%d` / `%i` | Signed decimal integer |
| `%u` | Unsigned decimal integer |
| `%x` / `%X` | Unsigned hex, lowercase / uppercase |
| `%%` | Literal `%` |

**Return value:**
- Total characters written on success.
- `-1` if `format` is NULL.
- Partial count if an invalid specifier is encountered (loop breaks early).

---

# INSTRUCTIONS
In order to use this library in your're project you will need to gitclone the 
repository into you're directory.

**Compilation:**
```bash
make
```
This will compile `libft` from the `libft/` subdirectory first, then it will 
build `libftprintf.a` at the root of the repository using `cc -Wall -Werror -Wextra` 
and `ar`

To utilize the library in you're project you will need to link them together using 
the following comand.

**Linking against your project:**
```bash
cc -Wall -Wextra -Werror main.c -L. -lftprintf -o my_program
```
**Makefile rules:**

| Rule | Effect |
|---|---|
| `make` / `make all` | Builds `libftprintf.a` |
| `make clean` | Removes object files |
| `make fclean` | Removes object files and `libftprintf.a` |
| `make re` | `fclean` + `all` |

Clean and fclean rules also runs the same rules from the Makefile of libft so 
everything will be deleted accordingly from both libraries.
---

# RESOURCES

For the creation of this project the following resources were used:
- [GeeksforGeeks - Variadic Functions in c](https://www.geeksforgeeks.org/c/variadic-functions-in-c/)
- [cppreference - Variadic Functions](https://cppreference.com/w/c/variadic.html)
- [GeeksforGeeks - 32-bit and 64-bit operating systems](https://www.geeksforgeeks.org/operating-systems/32-bit-vs-64-bit-operating-systems/)
- Peer to peer - For trouble shooting or testing different approaches

**AI usage:**

Claude by Anthropic was used in this project for the following:
- **As a teacher:** Explaining variadic function mechanics, `va_list` internals, 
and low-level C behavior.
- **Rewriting comments:** Improving clarity and precision of function-level documentation.
- **Testing:** Discussing edge cases and verifying expected behavior against 
the standard `printf()`.

AI was NOT used to write or generate any code. All function bodies were written by myself.

---

# ALGORITHM & DATA STRUCTURE

## Core loop
`ft_printf` loops across the format string one character at a time. Normal character 
are directly printed to stdout via `ft_printf_char`. When a `%` is found, the next character 
is validated by `check_format` against the included format specifiers.
- If invalid, an error is written to stdeer and the loop breaks immediatly, returning 
a partial count.
- If valid, `ft_handle_spec` calls the appropriate handler and the format pointer is moved 
past the specifier.
This method was chosen for clarity and extensibility. Each specifier goes to exactly one 
handler and adding a new one just requires a new handler and a new branch in `ft_handle_spec`
and `check_format`.

## Length tracking via pointer
All the handler functions are of type void, no handler returns a value. Instead, all handlers
receive `int *len` and increment it directly. This eliminates the need to sum return values
across the call chain and keeps the interface uniform across all handlers. `len` is the
single source of truth for the total character count.

## Base conversion (%x, %X, %p)
Hexadecimal output and pointer output delegate the conversion to `ft_putnbr_base`, which
takes any type of base string.
- The base gets validated via `check_base_error` (rejects bases shorter than 2, bases with
duplicate characters, and bases containing whitespace or `+`/`-`).
- Then `convert_base` gets called recursively. `convert_base` divides by the base size until
the number falls below the base, then unwinds the stack writing digits in most-significant-first order.

## Unsigned integer output (`%u`)
`ft_printf_u_int` applies the same recursive digit-writing strategy directly, without delegating
to `ft_putnbr_base`. Each recursive call writes one decimal digit via `write()` and increments
`len` by 1.

## Integer output (`%d`, `%i`)
`ft_printf_int` uses `ft_itoa` to measure the string length of the number (for updating `len`),
then frees the allocation and delegates actual output to `ft_putnbr_fd`.

# Null and edge case handling
- `ft_printf_char` casts `int` to `unsigned char` before output to avoid sign-extension. It
also handles `'\0'` explicitly via `write`, since string-based output functions stop at null.
- `ft_printf_str` prints `"(null)"` for a NULL string argument.
- `ft_printf_ptr` prints `"(nil)"` for a NULL pointer argument; non-NULL pointers are cast
to `uintptr_t` and printed with a `"0x"` prefix.