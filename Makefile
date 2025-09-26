CC = cc

CFLAGS = -Wall -Werror -Wextra -g -Iinclude -Ilibft

NAME = minishell
#		DIRECTORIES		#

SRC_DIR = src
OBJ_DIR = obj
UNIT_DIR = unit

#		PATHS			#

SRCS = src/shell.c src/ft_strtok.c src/ft_token_utils.c
OBJS = $(SRCS:.c=.o)

SRCS_PATH = $(pathsubst %, $(SRC_DIR)/%.c)

LFT= libft/libft.a
#		RULESET			#

all: $(NAME)

$(NAME): $(OBJS) $(LFT)
	$(CC) $(CFLAGS) -lreadline  $^ -o $(NAME)

$(LFT):
	$(MAKE) -C libft

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) -C libft clean
	rm -rf $(OBJS)

fclean: clean
	$(MAKE) -C libft fclean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclea re
