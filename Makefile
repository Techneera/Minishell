CC = cc

CFLAGS = -Wall -Werror -Wextra -Iinclude -Ilibft
ASAN_FLAGS = -fsanitize=address -g

NAME = minishell

# ----------------- DIRECTORIES ----------------- #

SRC_DIR = src
OBJS_DIR = $(SRC_DIR)/obj

AST_DIR = $(SRC_DIR)/ast
LEXER_DIR = $(SRC_DIR)/lexer
EXEC_DIR = $(SRC_DIR)/execution
EXP_DIR = $(SRC_DIR)/expansion
BUILTINS_DIR = $(SRC_DIR)/builtins
WILDCARD_DIR = $(SRC_DIR)/wildcard

# ----------------- SOURCES & OBJECTS ----------------- #

SRCS_AST    = ft_ast.c ft_ast_utils.c core.c guards.c core_utils.c redir_utils.c subshell_utils.c
SRCS_LEXER  = ft_lexer.c ft_lexer_guards.c ft_lexer_utils.c ft_redir_utils.c
SRCS_EXEC   = add_libft.c exec_tree_utils.c exec_utils.c files_utils.c ft_child_cmd.c ft_exec_tree.c \
              ft_fill_fds_file.c ft_getters.c ft_here_doc.c here_doc_utils.c ft_closing_all.c ft_create_fds.c \
              exec_tree_bonus.c ft_execution.c ft_execute_cmd.c ft_execute_sshell.c ft_update_position.c \
              ft_execute_pipe.c ft_child_sshell.c ft_create_fds_bonus.c utils_bonus.c ft_execute_and.c \
              ft_execute_or.c handle_signal.c errors_messages.c ft_cd.c ft_pwd.c ft_env.c ft_export.c \
              env_utils.c error_handler.c ft_print_sorted_export.c list_utils.c ft_getenv.c wildcards_utils.c \
              ft_is_builtin.c ft_get_command_path.c shell_helpers.c
SRCS_EXP    = args_handle.c args_handle_utils.c general_state.c
SRCS_BUILTINS = ft_echo.c ft_exit_status.c ft_exit_utils.c ft_unset.c ft_unset_utils.c
SRCS_WILDCARD = ft_wildcard.c

SHELL_SRC = shell.c
SHELL_OBJ = $(OBJS_DIR)/shell.o

PATH_OBJS_AST     = $(patsubst %.c,$(OBJS_DIR)/ast/%.o,$(SRCS_AST))
PATH_OBJS_LEXER   = $(patsubst %.c,$(OBJS_DIR)/lexer/%.o,$(SRCS_LEXER))
PATH_OBJS_EXEC    = $(patsubst %.c,$(OBJS_DIR)/execution/%.o,$(SRCS_EXEC))
PATH_OBJS_EXP     = $(patsubst %.c,$(OBJS_DIR)/expansion/%.o,$(SRCS_EXP))
PATH_OBJS_BUILTINS= $(patsubst %.c,$(OBJS_DIR)/builtins/%.o,$(SRCS_BUILTINS))
PATH_OBJS_WILDCARD= $(patsubst %.c,$(OBJS_DIR)/wildcard/%.o,$(SRCS_WILDCARD))

OBJECTS = $(SHELL_OBJ) $(PATH_OBJS_AST) $(PATH_OBJS_LEXER) $(PATH_OBJS_EXEC) \
          $(PATH_OBJS_EXP) $(PATH_OBJS_BUILTINS) $(PATH_OBJS_WILDCARD)

# ----------------- STATIC LIBRARIES ----------------- #

LFT = libft/libft.a

# ----------------- RULESET ----------------- #

all: $(NAME)

bonus: $(NAME)

$(NAME): $(OBJECTS) $(LFT)
	$(CC) $(filter-out $(LFT), $^) $(LFT) -o $(NAME) $(CFLAGS) -lreadline

$(LFT):
	@echo "Building libft..."
	$(MAKE) -C libft
	$(MAKE) -C libft bonus

$(SHELL_OBJ): $(SHELL_SRC) | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

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

$(OBJS_DIR)/lexer $(OBJS_DIR)/ast $(OBJS_DIR)/execution $(OBJS_DIR)/builtins $(OBJS_DIR)/expansion $(OBJS_DIR)/wildcard: $(OBJS_DIR)
	@mkdir -p $@

clean:
	$(MAKE) -C libft clean
	rm -rf $(OBJS_DIR)

fclean: clean
	$(MAKE) -C libft fclean
	rm -f $(NAME)

re: fclean all

vgr: $(LFT)
	$(CC) $(CFLAGS) -g -lreadline shell.c $^ -o $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=./readline.supp ./$(NAME)

ASAN_BUILD_FLAGS = $(CFLAGS) $(ASAN_FLAGS)

asan: fclean
	@echo "======================================================"
	@echo "  Building $(NAME) with AddressSanitizer enabled..."
	@echo "======================================================"
	$(MAKE) -C libft clean fclean CFLAGS="$(ASAN_BUILD_FLAGS)"
	$(MAKE) -C libft CFLAGS="$(ASAN_BUILD_FLAGS)"
	$(MAKE) -C libft bonus CFLAGS="$(ASAN_BUILD_FLAGS)"
	$(MAKE) all CFLAGS="$(ASAN_BUILD_FLAGS)"
	@echo "======================================================"
	@echo "  $(NAME) ready for AddressSanitizer testing."
	@echo "======================================================"

.PHONY: all clean fclean re vgr asan
