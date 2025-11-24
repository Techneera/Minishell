#include "libshell.h"
#include "ast.h"
#include "lexer.h"
#include "execution.h"

void				loop(char **envp);
static void			print_ast(t_ast *node, int depth);
static void			print_indent(int depth);
static void 		print_command_members(t_cmd *cmd, int depth);
static const char	*redir_map(t_label_redir label);

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	loop(envp);
	return (0);
}

void	loop(char **envp)
{
	char				*line;
	t_lexer				*lexer;
	t_ast				*head;
	struct sigaction	sa;
	struct sigaction	sh_sigt;

	sh_sigt.sa_handler = SIG_IGN;
	sigemptyset(&sh_sigt.sa_mask);
	sh_sigt.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sh_sigt, NULL);
	sa.sa_handler = &handle_sigstop;
	sa.sa_flags = SA_RESTART;
	line = NULL;
	lexer = NULL;
	while(1)
	{
		sigaction(SIGINT, &sa, NULL);
		head = NULL;
		line = readline(PROMPT);
		if (!line)
			break ;
		add_history(line);
		if (!ft_strcmp(line, "exit"))
			return (free(line));
		lexer = ft_state_lexer(line);
		if (!lexer)
		{
			free(line);
			break ;
		}
		head = ft_parser(lexer);
		if (head == NULL)
			fprintf(stderr, "Syntax error AST.\n");
		else
		{
			print_ast(head, 0);
			printf("\n\n");

			ft_execution(&head, envp);
		}
		if (head)
			ft_free_ast(head);
		free_lexer(lexer);
		free(line);
	}
}

static
void	print_indent(int depth)
{
	for (int i = 0; i < depth; i++)
		printf("\t");
}

static
const char	*redir_map(t_label_redir label)
{
	if (label == REDIR_IN) return ("<");
	if (label == REDIR_OUT) return (">");
	if (label == REDIR_APPEND) return (">>");
	if (label == REDIR_HEREDOCK) return ("<<");
	return ("?");
}

static
void	print_command_members(t_cmd *cmd, int depth)
{
	if (!cmd)
		return ;
	if (cmd->args)
	{
		print_indent(depth);
		printf("Args: ");
		for (int i = 0; cmd->args[i]; i++)
			printf("[%s]", cmd->args[i]);
		printf("\n");
	}
	if (cmd->redirs)
	{
		print_indent(depth);
		printf("Redirs: ");
		t_redir *r = cmd->redirs;
		int	i = 0;
		while (i < cmd->redir_count)
		{
			printf("(%s %s) ", redir_map(r[i].label), r[i].file_name);
			i++;
		}
		printf("\n");
	}
}

static
void print_ast(t_ast *node, int depth)
{
    if (!node)
    {
        print_indent(depth);
        printf("(NULL NODE)\n");
        return;
    }

    print_indent(depth);

    switch (node->type)
    {
        case NODE_PIPE:
            printf("PIPE\n");
            print_ast(node->left, depth + 1);
            print_ast(node->right, depth + 1);
            break;
        case NODE_AND:
            printf("AND (&&)\n");
            print_ast(node->left, depth + 1);
            print_ast(node->right, depth + 1);
            break;
        case NODE_OR:
            printf("OR (||)\n");
            print_ast(node->left, depth + 1);
            print_ast(node->right, depth + 1);
            break;
        case NODE_CMD:
            printf("COMMAND\n");
            print_command_members(node->cmd, depth + 1);
            break;
        case NODE_SUBSHELL:
            printf("SUBSHELL ( ... )\n");
            print_command_members(node->cmd, depth + 1);
            print_ast(node->body, depth + 1);
            break;
        default:
            printf("UNKNOWN NODE TYPE\n");
    }
}
