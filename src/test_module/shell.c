#include "libshell.h"
#include "ast.h"
#include "lexer.h"
#include "execution.h"

void				loop(char **envp);
// static void			print_ast(t_ast *node, int depth);
// static void			print_indent(int depth);
// // static void 		print_command_members(t_cmd *cmd, int depth);
// // static const char	*redir_map(t_label_redir label);

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	loop(envp);
	return (0);
}

void	loop(char **envp)
{
	t_data	data;

	signal(SIGQUIT, SIG_IGN);
	data = (t_data) {0};
	data.env_list = init_env(envp);
	data.envp = envlist_to_array(data.env_list);
	if (!data.envp)
	{
		perror("failled envlist_to_array malloc");
		//---free env_list
		return ;
	}
	while(1)
	{
		signal(SIGINT, &handle_sigstop);
		data.rl = readline(PROMPT);
		if (!data.rl)
		{
			ft_exit(&data);
			break ;
		}
		if (*data.rl != '\0')
		{
			add_history(data.rl);
			data.lexer = ft_state_lexer(data.rl);
			if (!data.lexer)
			{
				free_data(&data);
				break ;
			}
			data.root = ft_parser(data.lexer);
			data.tree = data.root;
			if (data.root == NULL)
				fprintf(stderr, "Syntax error AST.\n");
			else
			{
				// print_ast(data.root, 0);
				// printf("\n\n");
				ft_execution(&data);
			}
			if (data.root)
				ft_free_ast(data.root);
			if (data.lexer)
				free_lexer(data.lexer);
		}
		if (data.rl)
			free(data.rl);
	}
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
