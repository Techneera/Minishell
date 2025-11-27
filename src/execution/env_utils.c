#include "execution.h"

static t_list	*create_node_env(char *arg);
void	free_content(void *content);

t_list	*init_env(char **env)
{
	t_list	*env_list;
	t_list	*node;
	int		i;

	if (!env)
		return (NULL);
	i = 0;
	env_list = NULL;
	node = NULL;
	while (env[i])
	{
		node = create_node_env(env[i]);
		if (!node)
		{
			ft_lstclear(&env_list, &free_content);
			return (NULL);
		}
		ft_lstadd_back(&env_list, node);
		i++;
	}
	return (env_list);
}

void	free_content(void *content)
{
	t_env *env;

	env = (t_env *) content;
	if (!env)
		return ;
	if (env->variable)
	{
		free(env->variable);
		env->variable = NULL;
	}
	free(content);
	content = NULL;
}

static t_list	*create_node_env(char *arg)
{
	t_list	*new_list;
	t_env	*new_node;

	new_list = ft_calloc(1, sizeof(t_list));
	if (!new_list)
		return (NULL);
	new_node = ft_calloc(1, sizeof(t_env));
	if (!new_node)
		return (free(new_list), NULL);
	new_node->variable = ft_strdup(arg);
	if (!new_node->variable)
		return (free(new_node), free(new_list), NULL);
	new_node->has_arg = 1;
	new_list->content = (void *) new_node;
	new_list->next = NULL;
	return (new_list);
}
