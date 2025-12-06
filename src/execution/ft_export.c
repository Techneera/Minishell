/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "execution.h"

static int	has_arg(char *arg);
void		insert_in_list(t_list *list, char *arg);
int			insert_if_exist(t_list *list, char *arg);
int			is_valid(char *arg);

int	ft_export(t_list *list, char **args, t_data *data)
{
	int		n_args;
	int		i;

	i = 0;
	if (!list || !args)
		return (FAIL_STATUS);
	n_args = ft_arraylen((void **) args);
	if (n_args == 1)
		return (ft_print_sorted_export(list, data));
	else
	{
		while (++i < n_args)
		{
			if (is_valid(args[i]))
				insert_in_list(list, args[i]);
			else
				return (export_error(args[i]), FAIL_STATUS);
		}
		if (data->envp)
			ft_free_array(data->envp);
		data->envp = envlist_to_array(data->env_list);
	}
	return (0);
}

int	is_valid(char *arg)
{
	int	i;

	i = 0;
	if (!arg)
		return (0);
	if (!ft_isalpha(arg[0]))
		return (0);
	while (ft_isalpha(arg[i]) || ft_isalnum(arg[i]))
		i++;
	if (!arg[i])
		return (1);
	if (arg[i] == '=')
		return (1);
	return (0);
}

void	insert_in_list(t_list *list, char *arg)
{
	t_list	*new_arg;

	if (!arg)
		return ;
	if (insert_if_exist(list, arg))
		return ;
	new_arg = create_node_env(arg, has_arg(arg));
	if (!new_arg)
		return ;
	ft_lstadd_back(&list, new_arg);
	return ;
}

int	insert_if_exist(t_list *list, char *arg)
{
	t_env	*env;
	t_list	*actual;

	actual = list;
	while (actual)
	{
		env = (t_env *) actual->content;
		if (!env->variable || !arg)
			return (0);
		if (exist_in_list(env, arg))
		{
			free(env->variable);
			env->variable = ft_strdup(arg);
			env->has_arg = has_arg(arg);
			if (!env->variable)
				perror("failed calloc on ft_strdup on export");
			return (1);
		}
		actual = actual->next;
	}
	return (0);
}

static int	has_arg(char *arg)
{
	int	i;

	i = 0;
	if (!arg)
		return (0);
	while (arg[i])
	{
		if (arg[i] == '=')
			return (1);
		i++;
	}
	return (0);
}
