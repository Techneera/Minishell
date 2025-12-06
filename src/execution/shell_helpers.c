/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libshell.h"
#include "ast.h"
#include "lexer.h"
#include "execution.h"

void	increase_shlv(t_data *data)
{
	char	*str;
	char	*arg[3];
	char	*level;

	arg[0] = "export";
	arg[2] = NULL;
	str = getenv("SHLVL");
	if (!str)
		return ;
	level = ft_itoa(ft_atoi(str) + 1);
	if (!level)
		return ;
	str = ft_strjoin("SHLVL=", level);
	if (!str)
	{
		free(level);
		return ;
	}
	arg[1] = str;
	free(level);
	ft_export(data->env_list, arg, data);
	free(str);
}

void	cleanup_loop(t_data *data)
{
	if (data->root)
		ft_free_ast(data->root);
	if (data->lexer)
		free_lexer(data->lexer);
	data->lexer = NULL;
	data->root = NULL;
}

int	ft_verify_spaces(char **rl)
{
	int	len;
	int	i;

	len = ft_strlen(*rl);
	i = 0;
	while (i < len)
	{
		if (!ft_isspace((*rl)[i]))
			return (0);
		i++;
	}
	return (1);
}

int	process_rl(t_data *data)
{
	add_history(data->rl);
	data->lexer = ft_state_lexer(data->rl);
	if (!data->lexer)
	{
		free_data(data);
		return (0);
	}
	data->root = ft_parser(data->lexer);
	data->tree = data->root;
	if (data->root == NULL)
		ft_putstr_fd("Syntax error AST.\n", 2);
	else
		ft_execution(data);
	cleanup_loop(data);
	return (1);
}
