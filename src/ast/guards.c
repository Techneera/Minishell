#include "libshell.h"
#include "ast.h"
#include "lexer.h"

void	ft_free_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
	arr = NULL;
}

void	ft_free_cmd(t_cmd *cmd)
{
	int	i;

	if (!cmd)
		return ;
	ft_free_array(cmd->args);
	if (cmd->redirs)
	{
		i = 0;
		while (i < cmd->redir_count)
		{
			free(cmd->redirs[i].file_name);
			i++;
		}
		free(cmd->redirs);
	}
	free(cmd);
	cmd = NULL;
}

void	ft_free_ast(t_ast *root)
{
	if (!root)
		return ;
	if (root->type == NODE_AND || root->type == NODE_PIPE \
|| root->type == NODE_OR)
	{
		ft_free_ast(root->left);
		ft_free_ast(root->right);
	}
	else if (root->type == NODE_SUBSHELL)
		ft_free_ast(root->body);
	ft_free_cmd(root->cmd);
	free(root);
	root = NULL;
}

void	ft_free_redir_struct_only(void *content)
{
	t_redir	*redir;

	redir = (t_redir *)content;
	if (!redir)
		return ;
	free(redir);
	redir = NULL;
}
