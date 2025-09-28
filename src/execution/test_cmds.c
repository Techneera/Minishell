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

#include <stdlib.h> // For malloc, free, NULL
#include <string.h> // For strdup

// (t_cmd, t_redir, t_ast, and enum definitions go here)
// ... (Assuming your definitions from before are present)

// Helper function to count arguments
static int count_args(const char **args_literal)
{
    int count = 0;
    while (args_literal[count] != NULL)
        count++;
    return (count);
}

// Helper function to safely duplicate the argument array
static char **dup_args(const char **args_literal)
{
    int count = count_args(args_literal);
    char **new_args = malloc(sizeof(char *) * (count + 1));
    if (!new_args)
        return (NULL);

    for (int i = 0; i < count; i++)
    {
        // strdup allocates new memory for the string content
        new_args[i] = strdup(args_literal[i]);
        if (!new_args[i])
        {
            // IMPORTANT: Clean up already allocated strings if strdup fails
            for (int j = 0; j < i; j++)
                free(new_args[j]);
            free(new_args);
            return (NULL);
        }
    }
    new_args[count] = NULL; // Null-terminate the new array
    return (new_args);
}

t_ast   *ft_cmd1()
{
    // --- Command 1: cat README.md ---
    const char *args_literal1[] = {"cat", "README.md", NULL};
    char **args_cmd1 = dup_args(args_literal1); // Dynamically allocate args

    t_cmd *cmd1 = malloc(sizeof(t_cmd));
    if (!cmd1) return (NULL);
    cmd1->args = args_cmd1;
    cmd1->redir_count = 0;
    cmd1->redirs = NULL;

    // --- Command 2: grep "minishell" > output.txt ---
    const char *args_literal2[] = {"grep", "minishell", NULL};
    char **args_cmd2 = dup_args(args_literal2); // Dynamically allocate args
    
    // Redirection
    t_redir *redir2 = malloc(sizeof(t_redir));
    if (!redir2) { /* Cleanup cmd1 structures... */ return (NULL); }
    redir2->label = REDIR_OUT;
    // strdup is correct here to duplicate "output.txt"
    redir2->file_name = strdup("output.txt"); 
    
    t_cmd *cmd2 = malloc(sizeof(t_cmd));
    if (!cmd2) { /* Cleanup... */ return (NULL); }
    cmd2->args = args_cmd2;
    cmd2->redir_count = 1;
    cmd2->redirs = redir2;
    
    // --- AST Nodes ---
    t_ast *ast_cmd1 = malloc(sizeof(t_ast));
    if (!ast_cmd1) { /* Cleanup... */ return (NULL); }
    ast_cmd1->type = NODE_CMD;
    ast_cmd1->cmd = cmd1;
    ast_cmd1->left = NULL;
    ast_cmd1->right = NULL;
    
    t_ast *ast_cmd2 = malloc(sizeof(t_ast));
    if (!ast_cmd2) { /* Cleanup... */ return (NULL); }
    ast_cmd2->type = NODE_CMD;
    ast_cmd2->cmd = cmd2;
    ast_cmd2->left = NULL;
    ast_cmd2->right = NULL;
    
    // --- Root: NODE_PIPE ---
    t_ast *ast_pipe_root = malloc(sizeof(t_ast));
    if (!ast_pipe_root) { /* Cleanup... */ return (NULL); }
    ast_pipe_root->type = NODE_PIPE;
    ast_pipe_root->cmd = NULL;
    ast_pipe_root->left = ast_cmd1;
    ast_pipe_root->right = ast_cmd2;

    return (ast_pipe_root);
}

/*
			cat file_in.txt | grep "keyword" | wc -l > file_out.txt
			argv => [cmd1] [cmd2] [cmd3] [redirs]

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

t_ast   *ft_cmd2()
{
    // --- Command 1 (C1): cat file_in.txt ---
    const char *args_literal1[] = {"cat", "file_in.txt", NULL};
    char **args_c1 = dup_args(args_literal1); // SAFE ALLOCATION
    t_cmd *c1 = malloc(sizeof(t_cmd));
    if (!c1) return (NULL); // Basic error check
    c1->args = args_c1; c1->redir_count = 0; c1->redirs = NULL;
    t_ast *ast_c1 = malloc(sizeof(t_ast));
    if (!ast_c1) { /* Cleanup c1 */ return (NULL); }
    ast_c1->type = NODE_CMD; ast_c1->cmd = c1; ast_c1->left = NULL; ast_c1->right = NULL;

    // --- Command 2 (C2): grep "keyword" ---
    const char *args_literal2[] = {"grep", "keyword", NULL};
    char **args_c2 = dup_args(args_literal2); // SAFE ALLOCATION
    t_cmd *c2 = malloc(sizeof(t_cmd));
    if (!c2) { /* Cleanup... */ return (NULL); }
    c2->args = args_c2; c2->redir_count = 0; c2->redirs = NULL;
    t_ast *ast_c2 = malloc(sizeof(t_ast));
    if (!ast_c2) { /* Cleanup... */ return (NULL); }
    ast_c2->type = NODE_CMD; ast_c2->cmd = c2; ast_c2->left = NULL; ast_c2->right = NULL;

    // --- Command 3 (C3): wc -l > file_out.txt ---
    const char *args_literal3[] = {"wc", "-l", NULL};
    char **args_c3 = dup_args(args_literal3); // SAFE ALLOCATION

    // Redirection for C3: > file_out.txt
    t_redir *redir_c3 = malloc(sizeof(t_redir));
    if (!redir_c3) { /* Cleanup... */ return (NULL); }
    redir_c3->label = REDIR_OUT;
    redir_c3->file_name = strdup("file_out.txt"); // strdup is already safe

    t_cmd *c3 = malloc(sizeof(t_cmd));
    if (!c3) { /* Cleanup... */ return (NULL); }
    c3->args = args_c3;
    c3->redir_count = 1;
    c3->redirs = redir_c3;

    t_ast *ast_c3 = malloc(sizeof(t_ast));
    if (!ast_c3) { /* Cleanup... */ return (NULL); }
    ast_c3->type = NODE_CMD; ast_c3->cmd = c3; ast_c3->left = NULL; ast_c3->right = NULL;

    // ------------------------------------------
    // --- Inner Pipe (P1): C1 | C2 ---
    t_ast *ast_pipe_inner = malloc(sizeof(t_ast));
    if (!ast_pipe_inner) { /* Cleanup... */ return (NULL); }
    ast_pipe_inner->type = NODE_PIPE;
    ast_pipe_inner->cmd = NULL;
    ast_pipe_inner->left = ast_c1;
    ast_pipe_inner->right = ast_c2;

    // --- Outer Pipe (P2): P1 | C3 ---
    t_ast *ast_pipe_outer_root = malloc(sizeof(t_ast));
    if (!ast_pipe_outer_root) { /* Cleanup... */ return (NULL); }
    ast_pipe_outer_root->type = NODE_PIPE;
    ast_pipe_outer_root->cmd = NULL;
    ast_pipe_outer_root->left = ast_pipe_inner;
    ast_pipe_outer_root->right = ast_c3;

    return (ast_pipe_outer_root);
}

/*
	head -n 2 < README.md | (grep "project" | tail -n 1)
	argv => [redir] [cmd1] [cmd2] [cmd3]

			[NODE_PIPE] (Outer Root Pipe)
				/     \
			/       \
	[NODE_CMD]       [NODE_PIPE] (Subshell Root: grep | tail)
	`-- cmd: {        /     \
		args: {"head", "-n", "2", NULL},
		redir_count: 1,
		redirs: [ {label: REDIR_IN, file_name: "README.md"} ]
		}
						/       \
			[NODE_CMD]      [NODE_CMD]
				`-- cmd:         `-- cmd:
					{args: {"grep", "project", NULL}, ...}
						{args: {"tail", "-n", "1", NULL}, ...}
*/

t_ast	*ft_cmd3()
{
		// --- C1: head -n 2 < README.md ---
	char **args_c1 = dup_args((const char *[]){"head", "-n", "2", NULL});
	t_redir *redir_c1 = malloc(sizeof(t_redir));
	redir_c1->label = REDIR_IN;
	redir_c1->file_name = strdup("README.md");

	t_cmd *c1 = malloc(sizeof(t_cmd)); c1->args = args_c1; c1->redir_count = 1; c1->redirs = redir_c1;
	t_ast *ast_c1 = malloc(sizeof(t_ast)); ast_c1->type = NODE_CMD; ast_c1->cmd = c1; ast_c1->left = NULL; ast_c1->right = NULL;

	// --- C2.1: grep "project" (Inner Subshell Command) ---
	char **args_c21 = dup_args((const char *[]){"grep", "project", NULL});
	t_cmd *c21 = malloc(sizeof(t_cmd)); c21->args = args_c21; c21->redir_count = 0; c21->redirs = NULL;
	t_ast *ast_c21 = malloc(sizeof(t_ast)); ast_c21->type = NODE_CMD; ast_c21->cmd = c21; ast_c21->left = NULL; ast_c21->right = NULL;

	// --- C2.2: tail -n 1 (Inner Subshell Command) ---
	char **args_c22 = dup_args((const char *[]){"tail", "-n", "1", NULL});
	t_cmd *c22 = malloc(sizeof(t_cmd)); c22->args = args_c22; c22->redir_count = 0; c22->redirs = NULL;
	t_ast *ast_c22 = malloc(sizeof(t_ast)); ast_c22->type = NODE_CMD; ast_c22->cmd = c22; ast_c22->left = NULL; ast_c22->right = NULL;

	// --- P2 (Subshell Root): C2.1 | C2.2 ---
	t_ast *ast_subshell_root = malloc(sizeof(t_ast));
	ast_subshell_root->type = NODE_PIPE; ast_subshell_root->cmd = NULL;
	ast_subshell_root->left = ast_c21;
	ast_subshell_root->right = ast_c22;

	// --- P1 (Outer Root Pipe): C1 | P2 ---
	t_ast *ast_root = malloc(sizeof(t_ast));
	ast_root->type = NODE_PIPE; ast_root->cmd = NULL;
	ast_root->left = ast_c1;
	ast_root->right = ast_subshell_root;

	// return ast_root;
	return (ast_root);
}