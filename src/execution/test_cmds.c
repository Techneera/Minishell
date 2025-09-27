#include "execution.h"

/*
                        [NODE_PIPE]
                            /       \
                           /         \
                  [NODE_CMD]          [NODE_CMD]
                    `-- cmd:            `-- cmd: {
                        {args: {"cat", "README.md", NULL}, ...}
                                              args: {"grep", "minishell", NULL},
                                              redir_count: 1,
                                              redirs: { {label: REDIR_OUT, file_name: "output.txt"} }
                                            }
*/

t_ast	*ft_cmd1()
{
	// Command 1: cat README.md
	char **args_cmd1 = (char *[]){"cat", "README.md", NULL};
	t_cmd *cmd1 = malloc(sizeof(t_cmd));
	cmd1->args = args_cmd1;
	cmd1->redir_count = 0;
	cmd1->redirs = NULL;
	
	// Command 2: grep "minishell" > output.txt
	char **args_cmd2 = (char *[]){"grep", "minishell", NULL};
	t_redir *redir2 = malloc(sizeof(t_redir));
	redir2->label = REDIR_OUT;
	redir2->file_name = strdup("output.txt"); // Use strdup for string literal
	
	t_cmd *cmd2 = malloc(sizeof(t_cmd));
	cmd2->args = args_cmd2;
	cmd2->redir_count = 1;
	cmd2->redirs = redir2; // Note: For one redir, you can point directly. For multiple, you'd use an array.
	
	// AST Nodes
	t_ast *ast_cmd1 = malloc(sizeof(t_ast));
	ast_cmd1->type = NODE_CMD;
	ast_cmd1->cmd = cmd1;
	ast_cmd1->left = NULL;
	ast_cmd1->right = NULL;
	
	t_ast *ast_cmd2 = malloc(sizeof(t_ast));
	ast_cmd2->type = NODE_CMD;
	ast_cmd2->cmd = cmd2;
	ast_cmd2->left = NULL;
	ast_cmd2->right = NULL;
	
	// Root: NODE_PIPE
	t_ast *ast_pipe_root = malloc(sizeof(t_ast));
	ast_pipe_root->type = NODE_PIPE;
	ast_pipe_root->cmd = NULL; // NODE_PIPE doesn't have a t_cmd
	ast_pipe_root->left = ast_cmd1;
	ast_pipe_root->right = ast_cmd2;

	return (ast_pipe_root);
}

/*

                           [NODE_PIPE]
                              /     \
                             /       \
                    [NODE_PIPE]       [NODE_CMD]  <-- 'wc -l > file_out.txt'
                       /     \          `-- cmd: {
                      /       \              args: {"wc", "-l", NULL},
             [NODE_CMD]      [NODE_CMD]        redir_count: 1,
               `-- cmd:       `-- cmd:         redirs: { {label: REDIR_OUT, file_name: "file_out.txt"} }
                   {args: {"cat", "file_in.txt", NULL}, ...}
                       {args: {"grep", "keyword", NULL}, ...}
*/

t_ast	*ft_cmd2()
{
		// --- Command 1 (C1): cat file_in.txt ---
	char **args_c1 = (char *[]){"cat", "file_in.txt", NULL};
	t_cmd *c1 = malloc(sizeof(t_cmd));
	c1->args = args_c1; c1->redir_count = 0; c1->redirs = NULL;
	t_ast *ast_c1 = malloc(sizeof(t_ast));
	ast_c1->type = NODE_CMD; ast_c1->cmd = c1; ast_c1->left = NULL; ast_c1->right = NULL;

	// --- Command 2 (C2): grep "keyword" ---
	char **args_c2 = (char *[]){"grep", "keyword", NULL};
	t_cmd *c2 = malloc(sizeof(t_cmd));
	c2->args = args_c2; c2->redir_count = 0; c2->redirs = NULL;
	t_ast *ast_c2 = malloc(sizeof(t_ast));
	ast_c2->type = NODE_CMD; ast_c2->cmd = c2; ast_c2->left = NULL; ast_c2->right = NULL;

	// --- Command 3 (C3): wc -l > file_out.txt ---
	char **args_c3 = (char *[]){"wc", "-l", NULL};

	// Redirection for C3: > file_out.txt
	t_redir *redir_c3 = malloc(sizeof(t_redir));
	redir_c3->label = REDIR_OUT;
	redir_c3->file_name = strdup("file_out.txt");

	t_cmd *c3 = malloc(sizeof(t_cmd));
	c3->args = args_c3;
	c3->redir_count = 1;
	c3->redirs = redir_c3; // For simplicity, assumes a single redirection

	t_ast *ast_c3 = malloc(sizeof(t_ast));
	ast_c3->type = NODE_CMD; ast_c3->cmd = c3; ast_c3->left = NULL; ast_c3->right = NULL;

	// ------------------------------------------
	// --- Inner Pipe (P1): C1 | C2 ---
	// Represents: cat file_in.txt | grep "keyword"
	t_ast *ast_pipe_inner = malloc(sizeof(t_ast));
	ast_pipe_inner->type = NODE_PIPE;
	ast_pipe_inner->cmd = NULL;
	ast_pipe_inner->left = ast_c1;
	ast_pipe_inner->right = ast_c2;

	// --- Outer Pipe (P2): P1 | C3 ---
	// Represents: (cat ... | grep ...) | wc -l ...
	t_ast *ast_pipe_outer_root = malloc(sizeof(t_ast));
	ast_pipe_outer_root->type = NODE_PIPE;
	ast_pipe_outer_root->cmd = NULL;
	ast_pipe_outer_root->left = ast_pipe_inner; // The inner pipe is the left operand
	ast_pipe_outer_root->right = ast_c3;      // The final command is the right operand

	// The final AST is ast_pipe_outer_root
	return (ast_pipe_outer_root);
}