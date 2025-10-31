#include "libshell.h"
#include "ast.h"
#include "lexer.h"

void	ft_free_redir_lst(t_redir *redir)
{
	t_redir	*tmp;

	if (!redir)
		return ;
	while (redir)
	{
		tmp = redir->next;
		free(redir->file_name);
		free(redir);
		redir = tmp;
	}
}

void	ft_free_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	ft_free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	ft_free_array(cmd->args);
	ft_free_redir_lst(cmd->redirs);
	free(cmd);
}

void	ft_free_ast(t_ast *root)
{
	if (!root)
		return ;
	if (root->type == NODE_AND || root->type == NODE_PIPE || root->type == NODE_OR)
	{
		ft_free_ast(root->left);
		ft_free_ast(root->right);
	}
	else if (root->type == NODE_SUBSHELL)
		ft_free_ast(root->body);
	ft_free_cmd(root->cmd);
	free(root);
}
