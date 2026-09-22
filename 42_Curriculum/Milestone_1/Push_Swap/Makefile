.DEFAULT_GOAL := all

#COLORS
GREEN = \033[1;92m
RED = \033[1;91m
YELLOW = \033[1;93m
RESET = \033[0m

NAME = push_swap
NAME_BONUS = checker
DEBUGEXEC = a.out.debug
LIBFTLIB = libft.a

LIBFT_DIR = libft
OBJS_DIR = objs
STACK_OP_DIR = stack_operations
STRATS_DIR = strats
TESTING_UTILS_DIR = testing_utils

OBJS_DIR_BONUS = objs_bonus
BONUS_DIR = bonus

SRCS = $(wildcard *.c) \
		$(wildcard $(STACK_OP_DIR)/*.c) \
		$(wildcard $(TESTING_UTILS_DIR)/*.c) \
		$(wildcard $(STRATS_DIR)/*.c)
SRCS_BONUS = $(wildcard $(BONUS_DIR)/*.c) \
				$(wildcard $(BONUS_DIR)/$(STACK_OP_DIR)/*.c) \
				$(wildcard $(BONUS_DIR)/get_next_line/*.c)

OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.o)
OBJS_BONUS = $(patsubst $(BONUS_DIR)/%.c, $(BONUS_DIR)/$(OBJS_DIR_BONUS)/%.o, $(SRCS_BONUS))

CC = cc
CFLAGS = -Wall -Werror -Wextra
GDBFLAG = -g
RM = rm -rf
AR = ar -rcs
LIBFTLINK = -L$(LIBFT_DIR) -lft

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFTLINK) -o $(NAME)
	@echo "${GREEN}program compiled.${RESET}"

$(NAME_BONUS): $(OBJS_BONUS)
	@$(CC) $(CFLAGS) $(OBJS_BONUS) $(LIBFTLINK) -o $(NAME_BONUS)
	@echo "${GREEN}bonus program compiled.${RESET}"

$(DEBUGEXEC): $(OBJS)
	@$(CC) $(GDBFLAG) $(OBJS) $(LIBFTLINK) -o $(DEBUGEXEC)
	@echo "${YELLOW}program compiled with debugger.${RESET}"

$(OBJS_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(GDBFLAG) -c $< -o $@
	@echo "$@ -> ${GREEN}compiled.${RESET}"

$(BONUS_DIR)/$(OBJS_DIR_BONUS)/%.o: $(BONUS_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(GDBFLAG) -c $< -o $@
	@echo "$@ -> ${GREEN}bonus compiled.${RESET}"

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(OBJS_DIR_BONUS):
	@mkdir $(OBJS_DIR_BONUS)

$(LIBFTLIB):
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) CFLAGS="$(CFLAGS) $(GDBFLAG)"
	@echo "$@ -> ${GREEN}compiled.${RESET}"

all: libft $(NAME)

debug: libft $(DEBUGEXEC)

bonus: libft $(NAME_BONUS)

libft: $(LIBFTLIB)

clean:
	@$(RM) $(OBJS_DIR) $(BONUS_DIR)/$(OBJS_DIR_BONUS)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) clean
	@echo "${RED}objects removed.${RESET}"

fclean: clean
	@$(RM) $(NAME) $(NAME_BONUS) $(DEBUGEXEC) $(DEBUGEXEC_BONUS)
	@$(MAKE) --no-print-directory -C $(LIBFT_DIR) fclean
	@echo "${RED}program and debugger removed.${RESET}"

re: fclean all

.PHONY: all clean fclean re debug bonus