CC = cc

PATH_LIB = ../../include
PATH_LFT = ../../libft

CFLAGS = -Wall -Werror -Wextra -g -I$(PATH_LIB) -I$(PATH_LFT)
SRC = ft_execution.c test_cmds.c
OBJS = $(SRC:.c=.o)

NAME = exec

all: $(NAME)

$(NAME): $(OBJS) $(LFT)
	$(CC) $(CFLAGS) $^ -o $(NAME)

$(LFT):
	$(MAKE) -C $(PATH_LFT)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) -C $(PATH_LFT) clean
	rm -rf $(OBJS)

fclean: clean
	$(MAKE) -C $(PATH_LFT) fclean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclea re
