###
### Build configuration for libft.a
### - Compiles each libft source file into an object file in $(OBJS_DIR)
### - Packs all object files into the static library $(NAME)
### - Updates the library index so the linker can resolve symbols correctly
###

# COLORS
GREEN = \033[1;92m
RED = \033[1;91m
YELLOW = \033[1;93m
RESET = \033[0m

NAME = libft.a
OBJS_DIR = objs# Directory used to store generated object files

SRCS = ft_isalpha.c \
	ft_isdigit.c \
	ft_isalnum.c \
	ft_isascii.c \
	ft_isprint.c \
	ft_strlen.c \
	ft_memset.c \
	ft_bzero.c \
	ft_memcpy.c \
	ft_memmove.c \
	ft_strlcpy.c \
	ft_strlcat.c \
	ft_toupper.c \
	ft_tolower.c \
	ft_strchr.c \
	ft_strrchr.c \
	ft_strncmp.c \
	ft_memchr.c \
	ft_memcmp.c \
	ft_strnstr.c \
	ft_atoi.c \
	ft_calloc.c \
	ft_strdup.c \
	ft_substr.c \
	ft_strjoin.c \
	ft_strtrim.c \
	ft_split.c \
	ft_itoa.c \
	ft_strmapi.c \
	ft_striteri.c \
	ft_putchar_fd.c \
	ft_putstr_fd.c \
	ft_putendl_fd.c \
	ft_putnbr_fd.c \
	ft_lstnew.c \
	ft_lstadd_front.c \
	ft_lstsize.c \
	ft_lstlast.c \
	ft_lstadd_back.c \
	ft_lstdelone.c \
	ft_lstclear.c \
	ft_lstiter.c \
	ft_lstmap.c \
	ft_printf.c \
	ft_printf_char.c \
	ft_printf_hex.c \
	ft_printf_int.c \
	ft_printf_ptr.c \
	ft_printf_str.c \
	ft_printf_u_int.c \
	ft_putnbr_base.c

# Convert each source file name into its matching object path inside $(OBJS_DIR)
OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.o)

# Standard build tools and flags
CC = cc
CFLAGS = -Wall -Werror -Wextra
RM = rm -rf

all: $(NAME)

# Build the static library from all compiled object files
$(NAME): $(OBJS)
# `ar` creates an archive file (.a) containing the object files
# `r` replaces existing members in the archive if needed
# `c` creates the archive if it does not already exist
	@ar rc $(NAME) $(OBJS)
# Generate/update the archive index so the linker can find symbols quickly
	@ranlib $(NAME)

# Compile each .c file into a .o file inside $(OBJS_DIR)
# The order-only prerequisite ensures the directory exists before compilation
$(OBJS_DIR)/%.o: %.c | $(OBJS_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$@ -> ${GREEN}compiled.${RESET}"

# Create the object directory only if it does not already exist
$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

clean:
	@$(RM) $(OBJS_DIR)
	@echo "${RED}objects removed${RESET}"

fclean: clean
	@$(RM) $(NAME)
	@echo "${RED}libft removed.${RESET}"

# Full rebuild from scratch
re: fclean all

# Declare targets that do not represent real files
.PHONY: all clean fclean re