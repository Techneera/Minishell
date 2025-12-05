CC = cc

CFLAGS = -Wall -Werror -Wextra -g -Iinclude -Ilibft -fsanitize=address

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

EXP_DIR = $(SRC_DIR)/expansion

BUILTINS_DIR = $(SRC_DIR)/builtins

WILDCARD_DIR = $(SRC_DIR)/wildcard

#		---AST---				#

SRCS_AST=  ft_ast.c ft_ast_utils.c core.c guards.c core_utils.c redir_utils.c subshell_utils.c

OBJS_AST = $(SRCS_AST:.c=.o)

PATH_SRCS_AST = $(patsubst %,$(AST_DIR)/%,$(SRCS_AST))

PATH_OBJS_AST = $(patsubst %,$(OBJS_DIR)/ast/%,$(OBJS_AST))

#		---LEXER---				#

SRCS_LEXER = ft_lexer.c ft_lexer_guards.c ft_lexer_utils.c ft_redir_utils.c

OBJS_LEXER = $(SRCS_LEXER:.c=.o)

PATH_SRCS_LEXER = $(patsubst %,$(LEXER_DIR)/%,$(SRCS_LEXER))

PATH_OBJS_LEXER = $(patsubst %,$(OBJS_DIR)/lexer/%,$(OBJS_LEXER))

#		---EXECUTION---			#

SRCS_EXEC = add_libft.c exec_tree_utils.c exec_utils.c files_utils.c ft_child_cmd.c ft_exec_tree.c \
			ft_fill_fds_file.c ft_getters.c ft_here_doc.c here_doc_utils.c ft_fill_fds_file.c test_cmds.c \
			ft_closing_all.c ft_create_fds.c exec_tree_bonus.c ft_execution.c ft_execute_cmd.c ft_execute_sshell.c \
			ft_update_position.c ft_execute_pipe.c ft_child_sshell.c ft_create_fds_bonus.c utils_bonus.c ft_execute_and.c \
			ft_execute_or.c handle_signal.c errors_messages.c ft_cd.c ft_pwd.c ft_env.c ft_export.c env_utils.c error_handler.c \
			ft_print_sorted_export.c list_utils.c ft_getenv.c

OBJS_EXEC = $(SRCS_EXEC:.c=.o)

PATH_SRCS_EXEC = $(patsubst %,$(EXEC_DIR)/%,$(SRCS_EXEC))

PATH_OBJS_EXEC = $(patsubst %,$(OBJS_DIR)/execution/%,$(OBJS_EXEC))

#		---EXPANSION---				#

SRCS_EXP =  args_handle.c args_handle_utils.c general_state.c

OBJS_EXP = $(SRCS_EXP:.c=.o)

PATH_SRCS_EXP = $(patsubst %,$(EXP_DIR)/%,$(SRCS_EXP))

PATH_OBJS_EXP = $(patsubst %,$(OBJS_DIR)/expansion/%,$(OBJS_EXP))

#		---BUILTINS---				#

SRCS_BUILTINS =  ft_echo.c ft_exit_status.c ft_unset.c

OBJS_BUILTINS = $(SRCS_BUILTINS:.c=.o)

PATH_SRCS_BUILTINS = $(patsubst %,$(BUILTINS_DIR)/%,$(SRCS_BUILTINS))

PATH_OBJS_BUILTINS = $(patsubst %,$(OBJS_DIR)/builtins/%,$(OBJS_BUILTINS))

#		---WILDCARD---				#

SRCS_WILDCARD = ft_wildcard.c

OBJS_WILDCARD = $(SRCS_WILDCARD:.c=.o)

PATH_SRCS_WILDCARD = $(patsubst %,$(WILDCARD_DIR)/%,$(SRCS_WILDCARD))

PATH_OBJS_WILDCARD = $(patsubst %,$(OBJS_DIR)/wildcard/%,$(OBJS_WILDCARD))

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

test_parser: $(PATH_OBJS_AST) $(PATH_OBJS_LEXER) $(LFT)
	$(CC) $(CFLAGS) $(TESTER_DIR)/parser.c $^ -o $@

refactor_parser: $(PATH_OBJS_AST) $(PATH_OBJS_LEXER) $(LFT)
	$(CC) $(CFLAGS) $(TESTER_DIR)/refactor_parser.c $^ -o $@

$(AST_NAME): $(PATH_OBJS_AST) $(PATH_OBJS_LEXER) $(LFT)
	$(CC) $(CFLAGS) $(TESTER_DIR)/ast_tester.c $^ -o $@

$(NAME): $(PATH_OBJS_AST) $(PATH_OBJS_LEXER) $(PATH_OBJS_EXEC) $(PATH_OBJS_EXP) $(PATH_OBJS_BUILTINS) $(PATH_OBJS_WILDCARD) $(LFT)
	$(CC) $(CFLAGS) -lreadline src/test_module/shell.c $^ -o $(NAME)

$(LFT):
	$(MAKE) -C libft
	$(MAKE) -C libft bonus

$(OBJS_DIR)/lexer/%.o:$(LEXER_DIR)/%.c | $(OBJS_DIR)/lexer
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/execution/%.o:$(EXEC_DIR)/%.c | $(OBJS_DIR)/execution
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/ast/%.o:$(AST_DIR)/%.c | $(OBJS_DIR)/ast
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/builtins/%.o:$(BUILTINS_DIR)/%.c | $(OBJS_DIR)/builtins
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/expansion/%.o:$(EXP_DIR)/%.c | $(OBJS_DIR)/expansion
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR)/wildcard/%.o:$(WILDCARD_DIR)/%.c | $(OBJS_DIR)/wildcard
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJS_DIR):
	@mkdir -p $@

$(OBJS_DIR)/lexer: $(OBJS_DIR)
	@mkdir -p $@

$(OBJS_DIR)/ast: $(OBJS_DIR)
	@mkdir -p $@

$(OBJS_DIR)/execution: $(OBJS_DIR)
	@mkdir -p $@

$(OBJS_DIR)/builtins: $(OBJS_DIR)
	@mkdir -p $@

$(OBJS_DIR)/expansion: $(OBJS_DIR)
	@mkdir -p $@

$(OBJS_DIR)/wildcard: $(OBJS_DIR)
	@mkdir -p $@

clean:
	$(MAKE) -C libft clean
	rm -rf $(OBJS_DIR)

fclean: clean
	$(MAKE) -C libft fclean
	rm -rf $(NAME)
	rm -rf $(UNIT)
	rm -rf $(EXEC)
	rm -rf $(AST_NAME)
	rm -rf test_parser
	rm -rf refactor_parser

re: fclean all

vgr: all
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=./readline.supp ./$(NAME)

.PHONY: all clean fclean re unit vgr exec test_parser refactor_parser
