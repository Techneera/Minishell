#include "libshell.h"
#include "execution.h"

static
int	ft_is_valid_arguments(char *str)
{
	int	i;

	i = 0;
	if (!str || str[i] == '\0')
		return (0);
	if (ft_isdigit(str[i]))
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static
int	is_key_match(char *env_var, char *key)
{
	int	key_len;

	key_len = ft_strlen(key);
	if (ft_strncmp(env_var, key, key_len) != 0)
		return (0);
	if (env_var[key_len] == '=' || env_var[key_len] == '\0')
		return (1);
	return (0);
}

static void	free_env_node(t_list *node)
{
	t_env	*content;

	if (!node)
		return ;
	content = (t_env *)node->content;
	if (content)
	{
		if (content->variable)
			free(content->variable);
		free(content);
	}
	free(node);
}

/*
** Traverses the linked list, removing the node matching the key.
*/
static void	remove_env_var(t_data *data, char *key)
{
	t_list	*current;
	t_list	*prev;
	t_env	*env_struct;

	current = data->env_list;
	prev = NULL;
	while (current)
	{
		env_struct = (t_env *)current->content;
		if (is_key_match(env_struct->variable, key))
		{
			if (prev == NULL)
				data->env_list = current->next;
			else
				prev->next = current->next;
			free_env_node(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

static
void	refresh_env_array(t_data *data)
{
	if (data->envp)
	{
		ft_free_array(data->envp);
		data->envp = NULL;
	}
	data->envp = envlist_to_array(data->env_list);
	if (!data->envp && ft_lstsize(data->env_list) > 0)
		failed_malloc(data, "envp refresh");
}

int	ft_unset(t_data *data)
{
	int		i;
	int		ret;
	char	**args;

	args = data->tree->cmd->args;
	i = 1;
	ret = 0;
	if (!args[1])
		return (0);
	while (args[i])
	{
		if (!ft_is_valid_arguments(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			ret = 1;
		}
		else
			remove_env_var(data, args[i]);
		i++;
	}
	if (ret == 0 || i > 1)
		refresh_env_array(data);
	return (ret);
}
