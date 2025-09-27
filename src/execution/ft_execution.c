#include "execution.h"

int	execute_cmd(t_ast *ast_root, int *pipe_fd, int pos);
int	number_of_cmds(t_ast *ast_root);

int main()
{
	t_ast	*cmd = ft_cmd1();
	int		*pipe_fds;

	if (!cmd)
		return (-1);
	init_pipe(&pipe_fds, cmd);

	printf("%d", number_of_cmds(cmd));
}

void	init_pipe(int **pipe_fds, t_ast *ast_root)
{
	int	n_pipes;
	int	i;

	i = -1;
	n_pipes = number_of_cmds(ast_root) - 1;
	if (n_pipes > 0)
	{
		*pipe_fds = ft_calloc(n_pipes, sizeof(int));
		if (!*pipe_fds)
			exit(1);
	}
	else
	{
		while (i < n_pipes)
		{
			if(pipe(pipe_fds[++i]) == -1)
			{
				free(pipe_fds);
				exit(1);
			}
		}
	}
}

int	number_of_cmds(t_ast *ast_root)
{
	int		depth_left;
	int		depth_right;

	if (!ast_root)
		return (0);
	depth_left = number_of_cmds(ast_root->left);
	depth_right = number_of_cmds(ast_root->right);
	if (ast_root->type == NODE_CMD)
		return (1 + depth_left + depth_right);
	else
		return (depth_left + depth_right);
}

int	execute_cmd(t_ast *ast_root, int *pipe_fd, int pos)
{
	int new_pos;

	new_pos = pos;
	if (!ast_root)
		return (0);
	new_pos += execute_cmd(ast_root->left, pipe_fd, pos);
	new_pos += execute_cmd(ast_root->right, pipe_fd, new_pos);
	return (new_pos);
	/*
		Begin fork;
		inside fork {
			Check if pipe_fd is NULL or not
			if (NULL) -> possible one cmd
			if (NOT NULL) -> CMD has Pipe so recursion incresing pos
			execeve after the proper dup2;
		}
		return ()
	*/
}
