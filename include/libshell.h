#ifndef LIBSHELL_H
# define LIBSHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef enum e_label_redir
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOCK
}	t_label_redir;

typedef struct s_redir
{
	t_redir	label;
	char	*file_name;
}	t_redir;

typedef struct s_cmd
{
	char	**args;
	int		redir_count;
	t_redir	*redirs;
}	t_cmd;

typedef enum e_node_type
{
	NODE_CMD;
	NODE_PIPE;
}	t_node_type;

typedef struct s_ast
{
	t_node_type		type;
	t_cmd			*cmd;
	struct s_ast	*left;
	struct s_ast	*right;
}	t_ast;

#endif
