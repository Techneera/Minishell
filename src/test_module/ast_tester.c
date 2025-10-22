#include "ast.h"
#include <assert.h>

void	test_ast_node(void)
{
	t_ast	*root;
	t_cmd	*cmd;
	char	**av;
	t_redir	*rdir = (t_redir *)ft_calloc(5, sizeof(t_redir));

	av = ft_calloc(4, sizeof(char *));
	av[0] = strdup("uva");
	av[1] = strdup("abacaxi");
	av[2] = strdup("banana");
	av[3] = NULL;

	cmd = ft_create_cmd(av, rdir);
	root = ft_ast_node(cmd);

	assert(ft_strcmp(cmd->args[0], "uva") == 0);
	assert(ft_strcmp(cmd->args[1], "abacaxi") == 0);
	assert(ft_strcmp(cmd->args[2], "banana") == 0);
	assert(cmd->args[3] == NULL);

	assert(root->type == NODE_EMPTY);
	assert(root->cmd != NULL);
	assert(root->left == NULL);
	assert(root->right == NULL);

	ft_free_node(root);
}
int	main(void)
{
    test_ast_node();
    return (0);
}
