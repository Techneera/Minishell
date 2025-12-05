#include "execution.h"

void	ft_free_all(char **str)
{
	int	i;

	i = -1;
	if (!str)
		return ;
	while (str && str[++i])
	{
		free(str[i]);
		str[i] = NULL;
	}
	free(str);
	str = NULL;
}

char	**get_paths(char **env)
{
	char	**paths;
	char	**paths2;
	int		i;

	i = -1;
	while (env && env[++i])
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			break ;
	if (env && env[i])
	{
		paths = ft_split(env[i], '=');
		paths2 = ft_split(paths[1], ':');
		free_all((void **) paths, ft_arraylen((void **) paths));
		return (paths2);
	}
	return (NULL);
}

char	*get_command_path(char **arg, char **env, t_data *data)
{
	int			i;
	char		*tmp;
	char		*path;
	char		**paths;
	struct stat	st;

	i = -1;
	if (!(*arg) || **arg == '\0')
		return (0);
	if (stat(*arg, &st) == 0 && S_ISDIR(st.st_mode))
	{
		if (**arg == '/' || (*arg)[1] == '/' || (*arg)[ft_strlen((*arg)) - 1] == '/')
		{
			message_error(arg[0], ": Is a directory", 1);
			secure_exit(data, 126);
		}
		message_error(arg[0], ": command not found", 1);
		secure_exit(data, 127);
	}
	if (**arg == '/' && access(*arg, X_OK) == 0)
		return (ft_strdup(*arg));
	else if (**arg == '/' && access(*arg, F_OK) != 0)
	{
		message_error(arg[0], ": No such file or directory", 1);
		secure_exit(data, 127);
	}
	if (**arg == '.' && (*arg)[1] == '/')
	{
		if (access(*arg, F_OK) != 0)
		{
			message_error(arg[0], ": No such file or directory", 1);
			secure_exit(data, 127);
		}
		if(access(*arg, X_OK) == 0)
			return (ft_strdup(*arg));
		message_error(arg[0], ": Permission denied", 1);
		secure_exit(data, 126);
	}
	paths = get_paths(env);
	while (paths && paths[++i])
	{
		tmp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(tmp, *arg);
		free(tmp);
		if (access(path, X_OK) == 0)
			return (ft_free_all(paths), path);
		free(path);
	}
	return (ft_free_all(paths), NULL);
}

void	get_sizes(t_ast *ast_root, t_fds **fds, int inside_sshell)
{
	int	i;

	if (!ast_root)
		return ;
	if (ast_root->type == NODE_AND || ast_root->type == NODE_OR)
	{
		(*fds)->get.n_docs += docs_bonus(ast_root, fds);
		return ;
	}
	i = 0;
	if (ast_root->type == NODE_SUBSHELL)
		get_sizes(ast_root->body, fds, 1);
	get_sizes(ast_root->left, fds, inside_sshell);
	get_sizes(ast_root->right, fds, inside_sshell);
	if (ast_root->type == NODE_CMD || ast_root->type == NODE_SUBSHELL)
	{
		if (!inside_sshell)
			(*fds)->get.n_cmds++;
		while (i < ast_root->cmd->redir_count)
		{
			if (ast_root->cmd->redirs[i].label != REDIR_HEREDOCK)
				(*fds)->get.n_files++;
			if (ast_root->cmd->redirs[i].label == REDIR_HEREDOCK)
				(*fds)->get.n_docs++;
			i++;
		}
	}
}
