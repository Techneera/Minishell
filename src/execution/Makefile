CC = cc

PATH_LIB = ../../include
PATH_LFT = ../../libft

CFLAGS = -Wall -Werror -Wextra -g -I$(PATH_LIB) -I$(PATH_LFT)
SRC = ft_execution.c test_cmds.c  init_files.c exec_utils.c ft_getters.c ft_here_doc.c add_libft.c
LIBRARIES = -L$(PATH_LFT) -lft
OBJS = $(SRC:.c=.o)

NAME = exec

all: $(NAME)

$(NAME): $(OBJS)
	$(MAKE) -C $(PATH_LFT)
	$(CC) $(CFLAGS) $^ $(LIBRARIES) -o $(NAME)


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
