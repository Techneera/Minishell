#ifndef LIBSHELL_H
# define LIBSHELL_H

# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>

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
# define TRUE 1
# define FALSE 0

typedef enum e_label_redir
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOCK
}	t_label_redir;

typedef struct s_redir
{
	t_label_redir	label;
	char			*file_name;
}	t_redir;

typedef struct s_cmd
{
	char	**args;
	int		redir_count;
	t_redir	*redirs;
}	t_cmd;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE
}	t_node_type;

typedef struct s_ast
{
	t_node_type		type;
	t_cmd			*cmd;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

#endif
