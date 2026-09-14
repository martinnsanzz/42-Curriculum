# ft_printf

The main mechanic of *ft_printf* is the use of `Variadic Functions`.
They allow functions to accept an arbitrary number of arguments via
the ellipsis `(...)` parameter.
```c
int	ft_printf(const char *format, ...);
```

> Arguments passed via `...` are accessed through the `va_list` API from `<stdarg.h>`.

---

## Variadic library functions

| Macro | Purpose |
|---|---|
| `va_list` | Declares the argument list object |
| `va_start(ap, last)` | Initializes `ap` to the first variadic argument after `last` |
| `va_arg(ap, type)` | Extracts the next argument as `type` |
| `va_end(ap)` | Cleans up the `va_list` — must always be called |
| `va_copy` | Copies a `va_list` (unused in this project) |

> `va_list` is not a pointer on all platforms — never NULL-check it directly.
> Validity depends entirely on correct `va_start`/`va_end` discipline.

---

## ft_printf Behaviour

1. NULL-check `format` before `va_start` — return `-1` if NULL.
2. Call `va_start` to initialize the argument list.
3. Iterate over `format` character by character:
   - Plain characters → `ft_printf_char`
   - `%` → validate the next character via `check_format`:
     - Valid → dispatch to `ft_handle_spec`, advance pointer by 1
     - Invalid → print ANSI error to stderr, break loop
4. Call `va_end` before returning.
5. Return total characters written (`len`).

**Supported specifiers:**

| Specifier | Output |
|---|---|
| `%c` | Single character |
| `%s` | String (`"(null)"` if NULL) |
| `%p` | Pointer address in hex (`"(nil)"` if NULL) |
| `%d` / `%i` | Signed decimal integer |
| `%u` | Unsigned decimal integer |
| `%x` / `%X` | Unsigned hex, lower / uppercase |
| `%%` | Literal `%` |

**Return:**
- Success → total characters written (`len`)
- Failure (NULL format) → `-1`
- Invalid specifier → partial `len` (loop breaks early, no error signal to caller)

---

## Length Tracking

All handler functions receive `int *len` and increment it directly.
No handler returns a value — `len` is the single source of truth,
passed by pointer through the entire call chain.
```c
// Pattern used across all handlers:
*len += ft_strlen(s);   // or += 1, += 2, etc.
```

---

## Architecture
```
ft_printf
├── check_format        — validates the specifier character
├── ft_handle_spec      — dispatches to the correct handler
│   ├── ft_printf_char  — handles %c and %%
│   ├── ft_printf_str   — handles %s
│   ├── ft_printf_int   — handles %d and %i
│   ├── ft_printf_u_int — handles %u
│   ├── ft_printf_hex   — handles %x and %X
│   └── ft_printf_ptr   — handles %p
└── ft_putnbr_base      — base conversion used by hex and ptr
    ├── check_base_error — validates the base string
    └── convert_base    — recursive digit writer
```

---

## Handler Signatures

All handlers follow the same pattern — void return, `int *len` for tracking:
```c
void	ft_printf_char(int c, int *len);
void	ft_printf_str(char *s, int *len);
void	ft_printf_int(int num, int *len);
void	ft_printf_u_int(unsigned int num, int *len);
void	ft_printf_hex(unsigned long num, char conv, int *len);
void	ft_printf_ptr(void *p, int *len);
```

---

## Key Implementation Notes

- `ft_printf_char` casts `int` to `unsigned char` to avoid sign-extension UB.
- `ft_printf_char` handles `'\0'` explicitly via `write` since string functions stop at null.
- `ft_printf_int` uses `ft_itoa` only to measure length — `ft_putnbr_fd` does the actual output.
- `ft_printf_ptr` prints `"(nil)"` for NULL pointers — requires a `return` after that branch.
- `ft_printf_hex` uses `else if` logic internally — `conv` is either `'x'` or `'X'`, never both.
- `ft_putnbr_base` recomputes base size on every call via manual iteration.
- `convert_base` is recursive — stack depth is `floor(log_base(num)) + 1`.
- `check_base_error` does **not** reject ASCII 32 (space) — only controls 9–13 are caught.
- `check_format` writes an error to stderr for invalid specifiers.