CC = cc

CFLAGS = -Wall -Werror -Wextra -g -Iinclude -Ilibft

NAME = minishell

UNIT = unit_test

EXEC = exec_test

AST_NAME = ast_test

#		---DIRECTORIES---		#

SRC_DIR = src

OBJS_DIR = $(SRC_DIR)/obj

AST_DIR = $(SRC_DIR)/ast

LEXER_DIR = $(SRC_DIR)/lexer

TESTER_DIR = $(SRC_DIR)/test_module

EXEC_DIR = $(SRC_DIR)/execution

#		---AST---				#

SRCS_AST=  ft_ast.c

OBJS_AST = $(SRCS_AST:.c=.o)

PATH_SRCS_AST = $(patsubst %,$(AST_DIR)/%,$(SRCS_AST))

PATH_OBJS_AST = $(patsubst %,$(OBJS_DIR)/ast/%,$(OBJS_AST))

#		---LEXER---				#

SRCS_LEXER = ft_lexer.c

OBJS_LEXER = $(SRCS_LEXER:.c=.o)

PATH_SRCS_LEXER = $(patsubst %,$(LEXER_DIR)/%,$(SRCS_LEXER))

PATH_OBJS_LEXER = $(patsubst %,$(OBJS_DIR)/lexer/%,$(OBJS_LEXER))

#		---EXECUTION---			#

SRCS_EXEC = ft_execution.c test_cmds.c  init_files.c exec_utils.c ft_getters.c ft_here_doc.c add_libft.c

OBJS_EXEC = $(SRCS_EXEC:.c=.o)

PATH_SRCS_EXEC = $(patsubst %,$(EXEC_DIR)/%,$(SRCS_EXEC))

PATH_OBJS_EXEC = $(patsubst %,$(OBJS_DIR)/execution/%,$(OBJS_EXEC))

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

exec: $(PATH_OBJS_EXEC) $(LFT)
	$(CC) $(CFLAGS) $(TESTER_DIR)/exec_main.c $^ -o $(EXEC)

$(AST_NAME): $(PATH_OBJS_AST) $(LFT)
	$(CC) $(CFLAGS) $(TESTER_DIR)/ast_tester.c $^ -o $@

$(NAME): $(PATH_OBJS_LEXER) $(LFT)
	$(CC) $(CFLAGS) -lreadline src/test_module/shell.c $^ -o $(NAME)

$(LFT):
	$(MAKE) -C libft

$(OBJS_DIR)/lexer/%.o:$(LEXER_DIR)/%.c | $(OBJS_DIR)/lexer
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/execution/%.o:$(EXEC_DIR)/%.c | $(OBJS_DIR)/execution
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/ast/%.o:$(AST_DIR)/%.c | $(OBJS_DIR)/ast
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR):
	@mkdir -p $@

$(OBJS_DIR)/lexer: $(OBJS_DIR)
	@mkdir -p $@

$(OBJS_DIR)/ast: $(OBJS_DIR)
	@mkdir -p $@

$(OBJS_DIR)/execution: $(OBJS_DIR)
	@mkdir -p $@

clean:
	$(MAKE) -C libft clean
	rm -rf $(OBJS_DIR)

fclean: clean
	$(MAKE) -C libft fclean
	rm -rf $(NAME)
	rm -rf $(EXEC)
	rm -rf $(AST_NAME)

re: fclean all

vgr: all
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=./readline.supp ./$(NAME)

.PHONY: all clean fclean re unit vgr exec
