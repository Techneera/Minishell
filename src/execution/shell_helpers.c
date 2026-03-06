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

/**
 * \brief Increment the SHLVL environment variable by one.
 *
 * Reads the current SHLVL via getenv(), increments it, and re-exports
 * the new value so nested shells report the correct nesting depth.
 * \param data The shell state.
 */
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

/**
 * \brief Free the lexer and AST after each REPL iteration.
 * \param data The shell state (lexer and root AST are freed and NULLed).
 */
void	cleanup_loop(t_data *data)
{
	if (data->root)
		ft_free_ast(data->root);
	if (data->lexer)
		free_lexer(data->lexer);
	data->lexer = NULL;
	data->root = NULL;
}

/**
 * \brief Test whether a readline buffer contains only whitespace.
 * \param rl Pointer to the readline buffer.
 * \return 1 if all characters are whitespace (the line should be skipped); 0 otherwise.
 */
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

/**
 * \brief Drive lexing, parsing, and execution for one readline input line.
 *
 * Adds the line to readline history, tokenises it, builds the AST,
 * and calls ft_execution().  On a NULL parse result a syntax error is
 * reported and the history is cleared.
 * \param data The shell state.
 * \return 1 to continue the REPL loop; 0 to exit.
 */
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
	{
		ft_putstr_fd("Syntax error\n", 2);
		rl_clear_history();
		ft_exit_status(2, 1, 0);
	}
	else
		ft_execution(data);
	cleanup_loop(data);
	return (1);
}
