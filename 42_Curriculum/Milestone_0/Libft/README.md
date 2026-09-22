*This project has been created as part of the 42 curriculum by Martin Sanz Sanchez*

# // --- DESCRIPTION --- // 
Libft is the first project from the 42 curriculum. The goal is to recreate the core behaviour of standard C library functions (libc) from 0, and to create a set of utility functions that we will be able to reuse throughout the entire curriculum.

The project is structured in three mandatory parts:
- Part 1 - Libc functions: This part reimplements standard function such as strlen, memcpy, atoi, split, etc... with ft_ as the prefix to avoid conflicts. They strictly follow the behaviour mentioned on their man page and follow the original function prototype.

- Part 2 - Additional functions: These are custom utility functions outside of libc. Some of them are ft_substr, ft_strjoin, ft_strtrim, ft_itoa, etc... and output functions (ft_putchar_fd, ft_putstr_fd, ft_putendl_fd, ft_putnbr_fd).

- Part 3 - Linked list: In this section I implement a single linked list blueprint (t_list) inside the libft.h file and build some manipulation functions such as: ft_lstnew, ft_lstadd_front, ft_lstadd_back, ft_lstsize, ft_lstlast, ft_lstdelone, ft_lstclear, ft_lstiter, ft_lstmap.

The library is compiled into a static archive libft.a, which can be linked into any future project. 


# // --- INSTRUCTIONS --- //
To dowload this project you need to git clone the repository in your specified directory.
To compile this project you need to write 'make' in the terminal, this will compile all .c files and create libft.a at the root of the repository

The Makefile has a set of rules:
- make/make all		-> Compile and create libft.a
- make clean		-> Remove object file (.o)
- make fclean		-> Remove object files and libft.a
- make re			-> fclean + all

*NOTE* .o files get automatically stored in an objs/ folder inside the root directory.

To use the library in a project you must include the header and link the archive at compilation time:
`cc -Wall -Wextra -Werror your_file.c -L. -lft -o your_program`

The requirements for this project are as follows:
- Compiler: cc with flags -Wall -Wextra -Werror
- Archiver: ar (use of libtool is forbidden)
- Standard: C89/C99 compatible (no restrict qualifier in prototypes)

# // --- RESOURCES --- //
For the creation of this project the following resources were used:
- Linux man pages: Official man pages for the creation of part 1 functions
- W3Schools & GeeksforGeeks: Easy to understand guides for c topics such as pointers, memory, syntax, linked lists, etc...
- Neso Academy (https://www.youtube.com/playlist?list=PLBlnK6fEyqRj9lld8sWIUNwlKfdUoPd1Y): For dynamic memory allocation, in depth linked list understanding.
- Stack overflow forums: For specific questions in C programming

*AI Usage*
AI (Claude - Anthropic) and (Chatgpt - Open AI) was used in this project for the following things:
- As a teacher: To understand complex concepts such as (e.g., what was overlaping for memmove, returning value of strlcat, low-level memory concepts).
- Writing tester functions: Creating personal testers to get visual behaviour on the terminal and find;
	1) Flaws in my code
	2) Segmentation fault errors
	3) Edge cases (NULL input, empty strings, single character delimiters, etc)
	4) Help me debug and see how my code runs
- Comments: In my personal github (where my tester can be found), all functions have a block comment explaining what it does, parameters pass, return values, behaviour (When need for complex functions), and extra notes. Note this block header have been remove for the submission due to the moulinette not accepting comments. 

AI was *NOT* used to write or generate any code. All function bodies were written by myself.