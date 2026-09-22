.DEFAULT_GOAL := run

NAME = libftprintf.a# Name of the static library to build
EXEC = a.out
EXECDEBUG = a.out.debug

SRC_DIR = ../
OBJS_DIR = $(SRC_DIR)/objs
LIBFT_DIR = $(SRC_DIR)/libft# Path to the libft sub-project

SRCS = $(wildcard $(SRC_DIR)/ft_*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJS_DIR)/%.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror
AR = ar rcs
RM = rm -rf

TEST = srcs/$(PARAM)


# Linker flags for libft and libftprintf respectively
LIBFTLINK = -L$(LIBFT_DIR) -lft
PRINTFLINK = -L. -lftprintf

# Link the test file against both libraries to produce the executable
$(EXEC): $(NAME) $(TEST)
	$(CC) $(CFLAGS) $(TEST) $(PRINTFLINK) $(LIBFTLINK) -o $(EXEC)

$(EXECDEBUG) : $(NAME) $(TEST)
	$(CC) -g $(TEST) $(PRINTFLINK) $(LIBFTLINK) -o srcs/$(EXECDEBUG)

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -g -c $< -o $@

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

run: libft $(EXEC)
	./$(EXEC)

debug: libft $(EXECDEBUG)
	./srcs/$(EXECDEBUG)

# Build libft.a by delegating to its own Makefile
libft:
	$(MAKE) -C $(LIBFT_DIR)

clean:
	$(RM) $(OBJS_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME) $(EXEC)
	$(RM) *.dSYM
	$(MAKE) -C $(LIBFT_DIR) fclean

.PHONY: run clean fclean libft debug