/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:57:06 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:59:09 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libshell.h"
#include "ast.h"
#include "lexer.h"
#include "execution.h"

// static void			print_ast(t_ast *node, int depth);
// static void			print_indent(int depth);
// static void 		print_command_members(t_cmd *cmd, int depth);
// static const char	*redir_map(t_label_redir label);

static void	loop(char **envp);
char		**envlist_to_array(t_list *list);

/**
 * \brief Shell entry point. Ignores argc/argv and starts the REPL.
 * \param argc Argument count (ignored).
 * \param argv Argument vector (ignored).
 * \param envp The inherited environment from the parent process.
 * \return Always 0.
 */
int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	loop(envp);
	return (0);
}

/**
 * \brief Initialise the shell state: build env list, array, and increment SHLVL.
 * \param data  Output shell state struct (zeroed then populated).
 * \param envp  The inherited environment array.
 * \return 0 on success, 1 on allocation failure (state is freed).
 */
static
int	env_init(t_data *data, char **envp)
{
	*data = (t_data){0};
	data->env_list = init_env(envp);
	data->envp = envlist_to_array(data->env_list);
	if (!data->envp)
	{
		perror("failled envlist_to_array malloc");
		free_data(data);
		return (1);
	}
	increase_shlv(data);
	return (0);
}

/**
 * \brief Set up signals and read one line with readline.
 *
 * SIGQUIT is ignored; SIGINT is directed to handle_sigstop.
 * If readline returns NULL (Ctrl-D), ft_exit() is called.
 * \param data The shell state (rl member is set).
 */
static
void	ft_sig_and_line(t_data *data)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, &handle_sigstop);
	data->rl = readline(PROMPT);
	if (!data->rl)
		ft_exit(data);
}

/**
 * \brief The main REPL loop: read, lex, parse, expand, execute, repeat.
 * \param envp The inherited environment array.
 */
static
void	loop(char **envp)
{
	t_data	data;

	if (env_init(&data, envp))
		return ;
	while (1)
	{
		ft_sig_and_line(&data);
		if (ft_verify_spaces(&data.rl))
			continue ;
		if (*data.rl != '\0')
		{
			if (!process_rl(&data))
				break ;
		}
		if (data.rl)
			free(data.rl);
		data.rl = NULL;
		data.tree = NULL;
	}
	rl_clear_history();
}

/**
 * \brief Convert the environment linked list to a NULL-terminated string array.
 *
 * Only entries with \c has_arg set are included (i.e. variables that
 * carry a value are exported to child processes).
 * \param list The environment linked list.
 * \return Heap-allocated \c char** array suitable for \c execve(), or NULL.
 */
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
// static
// void	print_indent(int depth)
// {
// 	for (int i = 0; i < depth; i++)
// 		printf("\t");
// }

// static
// const char	*redir_map(t_label_redir label)
// {
// 	if (label == REDIR_IN) return ("<");
// 	if (label == REDIR_OUT) return (">");
// 	if (label == REDIR_APPEND) return (">>");
// 	if (label == REDIR_HEREDOCK) return ("<<");
// 	return ("?");
// }

// static
// void	print_command_members(t_cmd *cmd, int depth)
// {
// 	if (!cmd)
// 		return ;
// 	if (cmd->args)
// 	{
// 		print_indent(depth);
// 		printf("Args: ");
// 		for (int i = 0; cmd->args[i]; i++)
// 			printf("[%s]", cmd->args[i]);
// 		printf("\n");
// 	}
// 	if (cmd->redirs)
// 	{
// 		print_indent(depth);
// 		printf("Redirs: ");
// 		t_redir *r = cmd->redirs;
// 		int	i = 0;
// 		while (i < cmd->redir_count)
// 		{
// 			printf("(%s %s) ", redir_map(r[i].label), r[i].file_name);
// 			i++;
// 		}
// 		printf("\n");
// 	}
// }

// static
// void print_ast(t_ast *node, int depth)
// {
//     if (!node)
//     {
//         print_indent(depth);
//         printf("(NULL NODE)\n");
//         return;
//     }

//     print_indent(depth);

//     switch (node->type)
//     {
//         case NODE_PIPE:
//             printf("PIPE\n");
//             print_ast(node->left, depth + 1);
//             print_ast(node->right, depth + 1);
//             break;
//         case NODE_AND:
//             printf("AND (&&)\n");
//             print_ast(node->left, depth + 1);
//             print_ast(node->right, depth + 1);
//             break;
//         case NODE_OR:
//             printf("OR (||)\n");
//             print_ast(node->left, depth + 1);
//             print_ast(node->right, depth + 1);
//             break;
//         case NODE_CMD:
//             printf("COMMAND\n");
//             print_command_members(node->cmd, depth + 1);
//             break;
//         case NODE_SUBSHELL:
//             printf("SUBSHELL ( ... )\n");
//             print_command_members(node->cmd, depth + 1);
//             print_ast(node->body, depth + 1);
//             break;
//         default:
//             printf("UNKNOWN NODE TYPE\n");
//     }
// }
