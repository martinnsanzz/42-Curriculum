.DEFAULT_GOAL := all

NAME = libftprintf.a
LIBFTLIB = libft.a

LIBFT_DIR = libft
OBJS_DIR = objs

SRCS = ft_printf.c \
		ft_printf_char.c \
		ft_printf_hex.c \
		ft_printf_int.c \
		ft_printf_ptr.c \
		ft_printf_str.c \
		ft_printf_u_int.c \
		ft_putnbr_base.c
OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.o)

CC = cc
CFLAGS = -Wall -Werror -Wextra
RM = rm -rf
AR = ar -rcs

$(NAME): $(OBJS) $(LIBFTLIB)
	cp $(LIBFT_DIR)/$(LIBFTLIB) $(NAME)
	$(AR) $(NAME) $(OBJS_DIR)/*.o

$(OBJS_DIR)/%.o: %.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(LIBFTLIB):
	$(MAKE) -C $(LIBFT_DIR)

all: libft $(NAME)

libft: $(LIBFTLIB)

clean:
	$(RM) $(OBJS_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME) 
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re