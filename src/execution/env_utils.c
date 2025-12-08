/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "execution.h"

t_list			*create_node_env(char *arg, int has_arg);
void			ft_free_content(void *content);
static int		node_in_list(char *str, t_list **env_list);
static t_list	*no_env(t_list **env_list);

t_list	*init_env(char **env)
{
	t_list	*env_list;
	int		i;

	i = 0;
	env_list = NULL;
	if (!env || (env && !(*env)))
		return (no_env(&env_list));
	while (env && env[i])
	{
		node_in_list(env[i], &env_list);
		i++;
	}
	return (env_list);
}

static int	node_in_list(char *str, t_list **env_list)
{
	t_list	*node;

	node = create_node_env(str, 1);
	if (!node)
	{
		ft_lstclear(env_list, &ft_free_content);
		return (0);
	}
	ft_lstadd_back(env_list, node);
	return (1);
}

static t_list	*no_env(t_list **env_list)
{
	char	buff[PATH_MAX];
	char	*str;

	ft_memset(buff, 0, PATH_MAX);
	getcwd(buff, PATH_MAX);
	node_in_list("SHLVL=1", env_list);
	if (*buff)
	{
		str = ft_strjoin("PWD=", buff);
		if (!str)
			return (*env_list);
		node_in_list(str, env_list);
		free (str);
	}
	else
		node_in_list("PWD=.", env_list);
	return (*env_list);
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
