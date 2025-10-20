CC = cc

CFLAGS = -Wall -Werror -Wextra -g -Iinclude -Ilibft

NAME = minishell

UNIT = unit_test

#		---DIRECTORIES---		#

SRC_DIR = src

OBJS_DIR = $(SRC_DIR)/obj

AST_DIR = $(SRC_DIR)/ast

LEXER_DIR = $(SRC_DIR)/lexer

TESTER_DIR = $(SRC_DIR)/test_module

#		---PARSER---			#

SRCS_AST=  ft_ast.c

OBJS_AST = $(SRCS_PARSER:.c=.o)

PATH_SRCS_AST = $(patsubst %,$(AST_DIR)/%,$(SRCS_AST))

PATH_OBJS_AST = $(patsubst %,$(OBJS_DIR)/%,$(OBJS_AST))

#		---LEXER---			#

SRCS_LEXER = ft_lexer.c

OBJS_LEXER = $(SRCS_LEXER:.c=.o)

PATH_SRCS_LEXER = $(patsubst %,$(LEXER_DIR)/%,$(SRCS_LEXER))

PATH_OBJS_LEXER = $(patsubst %,$(OBJS_DIR)/%,$(OBJS_LEXER))

#		---TESTER---			#

SRCS_TESTER = unit.c

OBJS_TESTER = $(SRCS_LEXER:.c=.o)

PATH_TESTER = $(patsubst %,$(TESTER_DIR)/%,$(SRCS_TESTER))

#		---STATIC LIBRARIES---		#

LFT = libft/libft.a

#		---RULESET---			#

all: $(NAME)

unit: $(PATH_OBJS_LEXER) $(LFT)
	$(CC) $(CFLAGS)  $(PATH_TESTER) $^ -o $(UNIT)

$(NAME): $(PATH_OBJS_LEXER) $(LFT)
	$(CC) $(CFLAGS) -lreadline $(PATH_TESTER) $^ -o $(NAME)

$(LFT):
	$(MAKE) -C libft

$(OBJS_DIR)/%.o:$(LEXER_DIR)/%.c | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR):
	@mkdir -p $@

clean:
	$(MAKE) -C libft clean
	rm -rf $(OBJS_DIR)

fclean: clean
	$(MAKE) -C libft fclean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re unit
