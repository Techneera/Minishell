CC = cc

CFLAGS = -Wall -Werror -Wextra -g -Iinclude -Ilibft

NAME = minishell

#		DIRECTORIES		#

UNIT = unit_test

SRC_DIR = src

OBJ_DIR = $(SRC_DIR)/obj

PARSER_DIR = $(SRC_DIR)/parser

TESTER_DIR = $(SRC_DIR)/test_module

#		PARSER			#

SRCS_PARSER =  ft_strtok.c ft_token_utils.c shell.c ft_ast.c

OBJS_PARSER = $(SRCS_PARSER:.c=.o)

PATH_SRCS_PARSER = $(patsubst %,$(PARSER_DIR)/%,$(SRCS_PARSER))

PATH_OBJS_PARSER = $(patsubst %,$(OBJ_DIR)/%,$(OBJS_PARSER))

#		TESTER			#

PATH_TESTER = $(patsubst %,$(TESTER_DIR)/%,$(SRCS_TESTER))

#		STATIC LIBRARIES			#

LFT = libft/libft.a

#		RULESET			#

all: $(NAME)

$(NAME): $(PATH_OBJS_PARSER) $(LFT)
	$(CC) $(CFLAGS) -lreadline  $^ -o $(NAME)

$(LFT):
	$(MAKE) -C libft

$(OBJ_DIR)/%.o:$(PARSER_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $@

clean:
	$(MAKE) -C libft clean
	rm -f $(PATH_OBJS_PARSER)

fclean: clean
	$(MAKE) -C libft fclean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclea re
