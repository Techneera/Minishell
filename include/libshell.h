#ifndef LIBSHELL_H
# define LIBSHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stddef.h>
# include <stdbool.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "libft.h"

# define PROMPT            "minishell$> "
# define TC_BLACK          "\001\033[30m\002"
# define TC_RED            "\001\033[31m\002"
# define TC_GREEN          "\001\033[32m\002"
# define TC_YELLOW         "\001\033[33m\002"
# define TC_BLUE           "\001\033[34m\002"
# define TC_MAGENTA        "\001\033[35m\002"
# define TC_CYAN           "\001\033[36m\002"
# define TC_WHITE          "\001\033[37m\002"
# define TC_BRIGHT_BLACK   "\001\033[90m\002"
# define TC_BRIGHT_RED     "\001\033[91m\002"
# define TC_BRIGHT_GREEN   "\001\033[92m\002"
# define TC_BRIGHT_YELLOW  "\001\033[93m\002"
# define TC_BRIGHT_BLUE    "\001\033[94m\002"
# define TC_BRIGHT_MAGENTA "\001\033[95m\002"
# define TC_BRIGHT_CYAN    "\001\033[96m\002"
# define TC_BRIGHT_WHITE   "\001\033[97m\002"

typedef enum e_label_redir
{
	REDIR_NONE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOCK
}	t_label_redir;

typedef enum e_token_label
{
	TOKEN_ERROR,
	TOKEN_WORD, // Literal alpha string ("ls", "-la", "infile.txt")
	TOKEN_PIPE, // Pipe character
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_RIGHT_PAR, // Right parenthesis character
	TOKEN_LEFT_PAR, // Left parenthesis character
	TOKEN_REDIR_IN, // Redirection input character
	TOKEN_REDIR_OUT, // Redirection output character
	TOKEN_REDIR_APPEND, // Append character
	TOKEN_REDIR_HEREDOC, // Heredoc character
	TOKEN_EOF // End Of File
}	t_token_label;

typedef enum e_node_type
{
	NODE_EMPTY,
	NODE_CMD, // Command type node
	NODE_PIPE, // Pipe type node
	NODE_SUBSHELL, // Collection of tokens
	NODE_AND, // '&&' type node
	NODE_OR // '||' type node
}	t_node_type;

typedef struct s_token
{
	char			*str;
	t_token_label	tok_label;
	struct s_token	*next;
}	t_token;

typedef struct s_lexer
{
	char		*input;
	size_t		pos;
	size_t		len;
}	t_lexer;

typedef struct s_parser
{
	t_lexer	*lex;
	t_token	*current_token;
	t_token	*peek;
}	t_parser;

typedef struct s_redir
{
	char			*file_name;
	t_label_redir	label;
}	t_redir;

typedef struct s_cmd
{
	char	**args;
	int		redir_count;
	t_redir	*redirs;
}	t_cmd;

typedef struct s_ast
{
	t_cmd			*cmd;
	t_node_type		type;
	struct s_ast	*body;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

#endif
