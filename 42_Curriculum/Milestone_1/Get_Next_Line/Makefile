EXEC = a.out
EXECDEBUG = a.out.debug

OBJS_DIR = objs

SRCS = $(wildcard *.c)
OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.o)

CC = cc
CFLAGS = -Wall -Werror -Wextra
DEFINEFLAG = -D
DFLAG = -g
RM = rm -rf
PARAM = 1

all: $(EXEC)

debug: $(EXECDEBUG)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(DEFINEFLAG) BUFFER_SIZE=$(PARAM) $(SRCS) -o $(EXEC)

$(EXECDEBUG): $(OBJS)
	$(CC) $(DFLAG) $(SRCS) $(DEFINEFLAG) BUFFER_SIZE=$(PARAM) -o $(EXECDEBUG)

$(OBJS_DIR)/%.o: %.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

clean:
	$(RM) $(OBJS_DIR)

fclean: clean
	$(RM) $(EXEC) $(EXECDEBUG)

re: fclean all


.PHONY: all debug clean fclean re 