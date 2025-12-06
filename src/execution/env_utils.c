#include "execution.h"

t_list	*create_node_env(char *arg, int has_arg);
void	ft_free_content(void *content);

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
		node = create_node_env(env[i], 1);
		if (!node)
		{
			ft_lstclear(&env_list, &ft_free_content);
			return (NULL);
		}
		ft_lstadd_back(&env_list, node);
		i++;
	}
	return (env_list);
}

void	ft_free_content(void *content)
{
	t_env	*env;

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

t_list	*create_node_env(char *arg, int has_arg)
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
	new_node->has_arg = has_arg;
	new_list->content = (void *) new_node;
	new_list->next = NULL;
	return (new_list);
}

void	to_array_env(t_list *head, char **array)
{
	int		i;
	t_list	*tmp;
	t_env	*redir_to_arr;

	i = 0;
	tmp = head;
	while (tmp)
	{
		redir_to_arr = (t_env *)tmp->content;
		array[i] = redir_to_arr->variable;
		i++;
		tmp = tmp->next;
	}
}

char	**envlist_to_array(t_list *list)
{
	char	**array;
	t_list	*tmp;
	t_env	*redir_to_arr;
	int		i;

	i = 0;
	array = ft_calloc(ft_lstsize(list) + 1, sizeof(char *));
	if (!array)
		return (NULL);
	i = 0;
	tmp = list;
	while (tmp)
	{
		redir_to_arr = (t_env *)tmp->content;
		if (redir_to_arr->has_arg)
		{
			array[i] = ft_strdup(redir_to_arr->variable);
			if (!array[i])
				return (ft_free_array(array), NULL);
			i++;
		}
		tmp = tmp->next;
	}
	array[i] = NULL;
	return (array);
}
