#include "ast.h"
#include <assert.h>

# define ANSI_COLOR_RED     "\x1b[31m"
# define ANSI_COLOR_GREEN   "\x1b[32m"
# define ANSI_COLOR_YELLOW  "\x1b[33m"
# define ANSI_COLOR_BLUE    "\x1b[34m"
# define ANSI_COLOR_MAGENTA "\x1b[35m"
# define ANSI_COLOR_CYAN    "\x1b[36m"
# define ANSI_COLOR_RESET   "\x1b[0m"

static
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

    printf(ANSI_COLOR_MAGENTA "Node created\t\t");
    printf(ANSI_COLOR_GREEN "{OK}");
    printf(ANSI_COLOR_RESET "\n");

	ft_free_node(root);
}

static
void	test_tree(void)
{
	return ;
}

int	main(void)
{
    test_ast_node();
    return (0);
}
