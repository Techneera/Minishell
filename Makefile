CC = cc

CFLAGS = -Wall -Werror -Wextra -lreadline -Iinclude 

NAME = minishell
#		DIRECTORIES		#

SRC_DIR = src
OBJ_DIR = obj
UNIT_DIR = unit

#		PATHS			#

SRCS = src/shell.c
OBJS = $(SRCS:.c=.o)

SRCS_PATH = $(pathsubst %, $(SRC_DIR)/%.c)

#		RULESET			#

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $(NAME)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclea re
