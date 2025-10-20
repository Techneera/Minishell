CC = cc

PATH_LIB = ../../include
PATH_LFT = ../../libft

CFLAGS = -Wall -Werror -Wextra -g -I$(PATH_LIB) -I$(PATH_LFT)
SRC_LEX = ft_lexer.c
LIBRARIES = -L$(PATH_LFT) -lft
OBJS = $(SRC_LEX:.c=.o)

NAME = shell

UNIT = lexer_unit

all: $(NAME)

$(NAME): $(OBJS)
	$(MAKE) -C $(PATH_LFT)
	$(CC) $(CFLAGS) shell.c $^ $(LIBRARIES) -lreadline -o $(NAME)

$(UNIT): $(OBJS)
	$(MAKE) -C $(PATH_LFT)
	$(CC) $(CFLAGS) unit.c $^ $(LIBRARIES) -o $(NAME)


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(MAKE) -C $(PATH_LFT) clean
	rm -rf $(OBJS)

fclean: clean
	$(MAKE) -C $(PATH_LFT) fclean
	@rm -rf $(UNIT)
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclea re
