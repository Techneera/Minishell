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
			cat file_in.txt | grep "a" | wc -l > file_out.txt

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
    const char *args_literal2[] = {"grep", "a", NULL};
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

#include <stdlib.h> 
#include <string.h> 

// (Assuming dup_args, t_cmd, t_redir, t_ast, and enums are defined)

/*


                  [NODE_CMD]
                    `-- cmd: {
                        args: {"./my_script.sh", NULL},
                        redir_count: 4,
                        redirs: [
                          {label: REDIR_IN,     file_name: "in.txt"},
                          {label: REDIR_OUT,    file_name: "err.log"}, // (Proxy for STDERR)
                          {label: REDIR_APPEND, file_name: "hist.txt"},
                          {label: REDIR_OUT,    file_name: "out.txt"}
                        ]
                      }
*/

t_ast	*ft_cmd4()
{
    int redir_count = 4;
    
    // --- 1. Command Arguments ---
    char **args = dup_args((const char *[]){"./my_script.sh", NULL});

    // --- 2. Redirection Array Allocation ---
    t_redir *redirs_array = malloc(sizeof(t_redir) * redir_count);
    if (!redirs_array) { /* Cleanup args */ return (NULL); }

    // 3. Populate Redirection Structures (Order matters!)
    
    // Redir 1: < in.txt
    redirs_array[0].label = REDIR_IN;
    redirs_array[0].file_name = strdup("in.txt");

    // Redir 2: 2> err.log (Uses REDIR_OUT; your executor must check '2' token)
    redirs_array[1].label = REDIR_OUT;
    redirs_array[1].file_name = strdup("err.log");

    // Redir 3: >> hist.txt 
    redirs_array[2].label = REDIR_APPEND;
    redirs_array[2].file_name = strdup("hist.txt");

    // Redir 4: > out.txt (This will overwrite the redirection from Redir 3 on FD 1)
    redirs_array[3].label = REDIR_OUT;
    redirs_array[3].file_name = strdup("out.txt");


    // --- 4. Create t_cmd structure ---
    t_cmd *cmd = malloc(sizeof(t_cmd));
    if (!cmd) { /* Cleanup redirs_array */ return (NULL); }
    cmd->args = args;
    cmd->redir_count = redir_count;
    cmd->redirs = redirs_array; // Array of 4 t_redir structs

    // --- 5. Create Root AST Node ---
    t_ast *ast_redir_root = malloc(sizeof(t_ast));
    if (!ast_redir_root) { /* Cleanup cmd */ return (NULL); }
    ast_redir_root->type = NODE_CMD;
    ast_redir_root->cmd = cmd;
    ast_redir_root->left = NULL;
    ast_redir_root->right = NULL;

    return (ast_redir_root);
}

/*
		cat < file1.txt | grep "pattern" >> log.txt | sort -r | uniq > final.out

                          [NODE_PIPE] (Outer Root Pipe)
                             /     \
                            /       \
                  [NODE_PIPE] (Mid Pipe) [NODE_CMD]  <-- 'uniq > final.out'
                     /     \             `-- cmd: {
                    /       \                args: {"uniq", NULL},
          [NODE_PIPE] (Inner Pipe)  [NODE_CMD]      redir_count: 1,
             /     \             sort `         -- cmd: {         redirs: [ {label: REDIR_OUT, file_name: "final.out"} ]
            /       \                
   [NODE_CMD]      [NODE_CMD]        
     `-- cmd:        `-- cmd:args: {"grep", "pattern", NULL},
{args: {"cat", NULL},        redir_count: 1,
redir_count: 1,              redirs: [ {label: REDIR_APPEND, file_name: "log.txt"} ]
redirs: [ {label: REDIR_IN, file_name: "file1.txt"} ]
}

*/

t_ast	*ft_cmd5()
{
    // --- C1: cat < file1.txt ---
    char **args_c1 = dup_args((const char *[]){"cat", NULL});
    t_redir *redir_c1 = malloc(sizeof(t_redir));
    redir_c1->label = REDIR_IN; redir_c1->file_name = strdup("file1.txt");
    t_cmd *c1 = malloc(sizeof(t_cmd)); c1->args = args_c1; c1->redir_count = 1; c1->redirs = redir_c1;
    t_ast *ast_c1 = malloc(sizeof(t_ast)); ast_c1->type = NODE_CMD; ast_c1->cmd = c1; ast_c1->left = NULL; ast_c1->right = NULL;

    // --- C2: grep "pattern" >> log.txt ---
    char **args_c2 = dup_args((const char *[]){"grep", "pattern", NULL});
    t_redir *redir_c2 = malloc(sizeof(t_redir));
    redir_c2->label = REDIR_APPEND; redir_c2->file_name = strdup("log.txt");
    t_cmd *c2 = malloc(sizeof(t_cmd)); c2->args = args_c2; c2->redir_count = 1; c2->redirs = redir_c2;
    t_ast *ast_c2 = malloc(sizeof(t_ast)); ast_c2->type = NODE_CMD; ast_c2->cmd = c2; ast_c2->left = NULL; ast_c2->right = NULL;

    // --- C3: sort -r ---
    char **args_c3 = dup_args((const char *[]){"sort", "-r", NULL});
    t_cmd *c3 = malloc(sizeof(t_cmd)); c3->args = args_c3; c3->redir_count = 0; c3->redirs = NULL;
    t_ast *ast_c3 = malloc(sizeof(t_ast)); ast_c3->type = NODE_CMD; ast_c3->cmd = c3; ast_c3->left = NULL; ast_c3->right = NULL;

    // --- C4: uniq > final.out ---
    char **args_c4 = dup_args((const char *[]){"uniq", NULL});
    t_redir *redir_c4 = malloc(sizeof(t_redir));
    redir_c4->label = REDIR_OUT; redir_c4->file_name = strdup("final.out");
    t_cmd *c4 = malloc(sizeof(t_cmd)); c4->args = args_c4; c4->redir_count = 1; c4->redirs = redir_c4;
    t_ast *ast_c4 = malloc(sizeof(t_ast)); ast_c4->type = NODE_CMD; ast_c4->cmd = c4; ast_c4->left = NULL; ast_c4->right = NULL;

    // --- P1 (Inner Pipe): C1 | C2 ---
    t_ast *ast_pipe_inner = malloc(sizeof(t_ast));
    ast_pipe_inner->type = NODE_PIPE; ast_pipe_inner->cmd = NULL;
    ast_pipe_inner->left = ast_c1;
    ast_pipe_inner->right = ast_c2;

    // --- P2 (Mid Pipe): P1 | C3 ---
    t_ast *ast_pipe_mid = malloc(sizeof(t_ast));
    ast_pipe_mid->type = NODE_PIPE; ast_pipe_mid->cmd = NULL;
    ast_pipe_mid->left = ast_pipe_inner;
    ast_pipe_mid->right = ast_c3;

    // --- P3 (Outer Root Pipe): P2 | C4 ---
    t_ast *ast_root = malloc(sizeof(t_ast));
    ast_root->type = NODE_PIPE; ast_root->cmd = NULL;
    ast_root->left = ast_pipe_mid;
    ast_root->right = ast_c4;

    return (ast_root);
}

/*
                         [NODE_PIPE] (Outer Root Pipe)
                             /     \
                            /       \
                  [NODE_PIPE] (Mid Pipe) [NODE_CMD]  <-- 'wc -l > final_count.txt'
                     /     \             `-- cmd: {
                    /       \                args: {"wc", "-l", NULL},
          [NODE_PIPE] (Inner Pipe)  [NODE_CMD]      redir_count: 1,
             /     \              `-- cmd: {         redirs: [ {label: REDIR_OUT, file_name: "final_count.txt"} ]
            /       \                 args: {"grep", "keyword", NULL},
   [NODE_CMD]      [NODE_CMD]          redir_count: 1,
     `-- cmd:        `-- cmd:             redirs: [ {label: REDIR_OUT, file_name: "filtered.txt"} ]
         {args: {"cat", NULL},
          redir_count: 1,
          redirs: [ {label: REDIR_IN, file_name: "list.txt"} ]
         }
             {args: {"sort", "-f", NULL}, ...}
*/

t_ast	*ft_cmd6()
{
    // --- C1: cat < list.txt ---
    char **args_c1 = dup_args((const char *[]){"cat", NULL});
    t_redir *redir_c1 = malloc(sizeof(t_redir));
    redir_c1->label = REDIR_IN; redir_c1->file_name = strdup("list.txt");
    t_cmd *c1 = malloc(sizeof(t_cmd)); c1->args = args_c1; c1->redir_count = 1; c1->redirs = redir_c1;
    t_ast *ast_c1 = malloc(sizeof(t_ast)); ast_c1->type = NODE_CMD; ast_c1->cmd = c1; ast_c1->left = NULL; ast_c1->right = NULL;

    // --- C2: sort -f ---
    char **args_c2 = dup_args((const char *[]){"sort", "-f", NULL});
    t_cmd *c2 = malloc(sizeof(t_cmd)); c2->args = args_c2; c2->redir_count = 0; c2->redirs = NULL;
    t_ast *ast_c2 = malloc(sizeof(t_ast)); ast_c2->type = NODE_CMD; ast_c2->cmd = c2; ast_c2->left = NULL; ast_c2->right = NULL;

    // --- C3: grep "keyword" > filtered.txt ---
    char **args_c3 = dup_args((const char *[]){"grep", "r", NULL});
    t_redir *redir_c3 = malloc(sizeof(t_redir));
    redir_c3->label = REDIR_OUT; redir_c3->file_name = strdup("filtered.txt");
    t_cmd *c3 = malloc(sizeof(t_cmd)); c3->args = args_c3; c3->redir_count = 1; c3->redirs = redir_c3;
    t_ast *ast_c3 = malloc(sizeof(t_ast)); ast_c3->type = NODE_CMD; ast_c3->cmd = c3; ast_c3->left = NULL; ast_c3->right = NULL;

    // --- C4: wc -l > final_count.txt ---
    char **args_c4 = dup_args((const char *[]){"wc", "-l", NULL});
    t_redir *redir_c4 = malloc(sizeof(t_redir));
    redir_c4->label = REDIR_OUT; redir_c4->file_name = strdup("final_count.txt");
    t_cmd *c4 = malloc(sizeof(t_cmd)); c4->args = args_c4; c4->redir_count = 1; c4->redirs = redir_c4;
    t_ast *ast_c4 = malloc(sizeof(t_ast)); ast_c4->type = NODE_CMD; ast_c4->cmd = c4; ast_c4->left = NULL; ast_c4->right = NULL;

    // --- P1 (Inner Pipe): C1 | C2 ---
    t_ast *ast_pipe_inner = malloc(sizeof(t_ast));
    ast_pipe_inner->type = NODE_PIPE; ast_pipe_inner->cmd = NULL;
    ast_pipe_inner->left = ast_c1;
    ast_pipe_inner->right = ast_c2;

    // --- P2 (Mid Pipe): P1 | C3 ---
    t_ast *ast_pipe_mid = malloc(sizeof(t_ast));
    ast_pipe_mid->type = NODE_PIPE; ast_pipe_mid->cmd = NULL;
    ast_pipe_mid->left = ast_pipe_inner;
    ast_pipe_mid->right = ast_c3;

    // --- P3 (Outer Root Pipe): P2 | C4 ---
    t_ast *ast_root = malloc(sizeof(t_ast));
    ast_root->type = NODE_PIPE; ast_root->cmd = NULL;
    ast_root->left = ast_pipe_mid;
    ast_root->right = ast_c4;

    return (ast_root);
}

/*
            ps aux | awk '{print $1, $11}' | sort | head -n 10

                          [NODE_PIPE] (Outer Root Pipe)
                             /     \
                            /       \
                  [NODE_PIPE] (Mid Pipe) [NODE_CMD]  <-- 'head -n 10'
                     /     \             `-- cmd: {args: {"head", "-n", "10", NULL}, ...}
                    /       \
          [NODE_PIPE] (Inner Pipe)  [NODE_CMD]  <-- 'sort'
             /     \              `-- cmd: {args: {"sort", NULL}, ...}
            /       \
   [NODE_CMD]      [NODE_CMD]
     `-- cmd:         `-- cmd:
         {args: {"ps", "aux", NULL}, ...}
             {args: {"awk", "{print $1, $11}", NULL}, ...}
*/
t_ast   *ft_cmd7()
{
    // --- C1: ps aux ---
    char **args_c1 = dup_args((const char *[]){"ps", "aux", NULL});
    t_cmd *c1 = malloc(sizeof(t_cmd)); c1->args = args_c1; c1->redir_count = 0; c1->redirs = NULL;
    t_ast *ast_c1 = malloc(sizeof(t_ast)); ast_c1->type = NODE_CMD; ast_c1->cmd = c1; ast_c1->left = NULL; ast_c1->right = NULL;

    // --- C2: awk '{print $1, $11}' ---
    char **args_c2 = dup_args((const char *[]){"awk", "{print $1, $11}", NULL});
    t_cmd *c2 = malloc(sizeof(t_cmd)); c2->args = args_c2; c2->redir_count = 0; c2->redirs = NULL;
    t_ast *ast_c2 = malloc(sizeof(t_ast)); ast_c2->type = NODE_CMD; ast_c2->cmd = c2; ast_c2->left = NULL; ast_c2->right = NULL;

    // --- C3: sort ---
    char **args_c3 = dup_args((const char *[]){"sort", NULL});
    t_cmd *c3 = malloc(sizeof(t_cmd)); c3->args = args_c3; c3->redir_count = 0; c3->redirs = NULL;
    t_ast *ast_c3 = malloc(sizeof(t_ast)); ast_c3->type = NODE_CMD; ast_c3->cmd = c3; ast_c3->left = NULL; ast_c3->right = NULL;

    // --- C4: head -n 10 ---
    char **args_c4 = dup_args((const char *[]){"head", "-n", "10", NULL});
    t_cmd *c4 = malloc(sizeof(t_cmd)); c4->args = args_c4; c4->redir_count = 0; c4->redirs = NULL;
    t_ast *ast_c4 = malloc(sizeof(t_ast)); ast_c4->type = NODE_CMD; ast_c4->cmd = c4; ast_c4->left = NULL; ast_c4->right = NULL;

    // --- P1 (Inner Pipe): C1 | C2 ---
    t_ast *ast_pipe_inner = malloc(sizeof(t_ast));
    ast_pipe_inner->type = NODE_PIPE; ast_pipe_inner->cmd = NULL;
    ast_pipe_inner->left = ast_c1;
    ast_pipe_inner->right = ast_c2;

    // --- P2 (Mid Pipe): P1 | C3 ---
    t_ast *ast_pipe_mid = malloc(sizeof(t_ast));
    ast_pipe_mid->type = NODE_PIPE; ast_pipe_mid->cmd = NULL;
    ast_pipe_mid->left = ast_pipe_inner;
    ast_pipe_mid->right = ast_c3;

    // --- P3 (Outer Root Pipe): P2 | C4 ---
    t_ast *ast_root = malloc(sizeof(t_ast));
    ast_root->type = NODE_PIPE; ast_root->cmd = NULL;
    ast_root->left = ast_pipe_mid;
    ast_root->right = ast_c4;

    return (ast_root);
}

/*
        cat < file_list.txt | sort -r | uniq | head -n 5

                          [NODE_PIPE] (Outer Root Pipe)
                             /     \
                            /       \
                  [NODE_PIPE] (Mid Pipe) [NODE_CMD]  <-- 'head -n 5' (No redirs)
                     /     \             `-- cmd: {args: {"head", "-n", "5", NULL}, ...}
                    /       \
          [NODE_PIPE] (Inner Pipe)  [NODE_CMD]  <-- 'uniq' (No redirs)
             /     \              `-- cmd: {args: {"uniq", NULL}, ...}
            /       \
   [NODE_CMD]      [NODE_CMD]
     `-- cmd:        `-- cmd: {args: {"sort", "-r", NULL}, ...}
         {args: {"cat", NULL},
          redir_count: 1,
          redirs: [ {label: REDIR_IN, file_name: "file_list.txt"} ]
         }
*/
t_ast   *ft_cmd8()
{
    // --- C1: cat < file_list.txt (Input Redir) ---
    char **args_c1 = dup_args((const char *[]){"cat", NULL});
    t_redir *redir_c1 = malloc(sizeof(t_redir));
    redir_c1->label = REDIR_IN; redir_c1->file_name = strdup("file_list.txt");
    t_cmd *c1 = malloc(sizeof(t_cmd)); c1->args = args_c1; c1->redir_count = 1; c1->redirs = redir_c1;
    t_ast *ast_c1 = malloc(sizeof(t_ast)); ast_c1->type = NODE_CMD; ast_c1->cmd = c1; ast_c1->left = NULL; ast_c1->right = NULL;

    // --- C2: sort -r (No Redir) ---
    char **args_c2 = dup_args((const char *[]){"sort", "-r", NULL});
    t_cmd *c2 = malloc(sizeof(t_cmd)); c2->args = args_c2; c2->redir_count = 0; c2->redirs = NULL;
    t_ast *ast_c2 = malloc(sizeof(t_ast)); ast_c2->type = NODE_CMD; ast_c2->cmd = c2; ast_c2->left = NULL; ast_c2->right = NULL;

    // --- C3: uniq (No Redir) ---
    char **args_c3 = dup_args((const char *[]){"uniq", NULL});
    t_cmd *c3 = malloc(sizeof(t_cmd)); c3->args = args_c3; c3->redir_count = 0; c3->redirs = NULL;
    t_ast *ast_c3 = malloc(sizeof(t_ast)); ast_c3->type = NODE_CMD; ast_c3->cmd = c3; ast_c3->left = NULL; ast_c3->right = NULL;

    // --- C4: head -n 5 (No Redir) ---
    char **args_c4 = dup_args((const char *[]){"head", "-n", "5", NULL});
    t_cmd *c4 = malloc(sizeof(t_cmd)); c4->args = args_c4; c4->redir_count = 0; c4->redirs = NULL;
    t_ast *ast_c4 = malloc(sizeof(t_ast)); ast_c4->type = NODE_CMD; ast_c4->cmd = c4; ast_c4->left = NULL; ast_c4->right = NULL;

    // --- P1 (Inner Pipe): C1 | C2 ---
    t_ast *ast_pipe_inner = malloc(sizeof(t_ast));
    ast_pipe_inner->type = NODE_PIPE; ast_pipe_inner->cmd = NULL;
    ast_pipe_inner->left = ast_c1;
    ast_pipe_inner->right = ast_c2;

    // --- P2 (Mid Pipe): P1 | C3 ---
    t_ast *ast_pipe_mid = malloc(sizeof(t_ast));
    ast_pipe_mid->type = NODE_PIPE; ast_pipe_mid->cmd = NULL;
    ast_pipe_mid->left = ast_pipe_inner;
    ast_pipe_mid->right = ast_c3;

    // --- P3 (Outer Root Pipe): P2 | C4 ---
    t_ast *ast_root = malloc(sizeof(t_ast));
    ast_root->type = NODE_PIPE; ast_root->cmd = NULL;
    ast_root->left = ast_pipe_mid;
    ast_root->right = ast_c4;

    return (ast_root);
}

/*
        cat << END | sort -r | uniq | head -n 5

                          [NODE_PIPE] (Outer Root Pipe)
                             /     \
                            /       \
                  [NODE_PIPE] (Mid Pipe) [NODE_CMD]  <-- 'head -n 5' (No redirs)
                     /     \             `-- cmd: {args: {"head", "-n", "5", NULL}, ...}
                    /       \
          [NODE_PIPE] (Inner Pipe)  [NODE_CMD]  <-- 'uniq' (No redirs)
             /     \              `-- cmd: {args: {"uniq", NULL}, ...}
            /       \
   [NODE_CMD]      [NODE_CMD]
     `-- cmd:        `-- cmd: {args: {"sort", "-r", NULL}, ...}
         {args: {"cat", NULL},
          redir_count: 1,
          redirs: [ {label: REDIR_IN, file_name: "file_list.txt"} ]
         }
*/
t_ast   *ft_cmd9()
{
    // --- C1: cat < file_list.txt (Input Redir) ---
    char **args_c1 = dup_args((const char *[]){"cat", NULL});
    t_redir *redir_c1 = malloc(sizeof(t_redir));
    redir_c1->label = REDIR_HEREDOCK; redir_c1->file_name = strdup("END");
    t_cmd *c1 = malloc(sizeof(t_cmd)); c1->args = args_c1; c1->redir_count = 1; c1->redirs = redir_c1;
    t_ast *ast_c1 = malloc(sizeof(t_ast)); ast_c1->type = NODE_CMD; ast_c1->cmd = c1; ast_c1->left = NULL; ast_c1->right = NULL;

    // --- C2: sort -r (No Redir) ---
    char **args_c2 = dup_args((const char *[]){"sort", "-r", NULL});
    t_cmd *c2 = malloc(sizeof(t_cmd)); c2->args = args_c2; c2->redir_count = 0; c2->redirs = NULL;
    t_ast *ast_c2 = malloc(sizeof(t_ast)); ast_c2->type = NODE_CMD; ast_c2->cmd = c2; ast_c2->left = NULL; ast_c2->right = NULL;

    // --- C3: uniq (No Redir) ---
    char **args_c3 = dup_args((const char *[]){"uniq", NULL});
    t_cmd *c3 = malloc(sizeof(t_cmd)); c3->args = args_c3; c3->redir_count = 0; c3->redirs = NULL;
    t_ast *ast_c3 = malloc(sizeof(t_ast)); ast_c3->type = NODE_CMD; ast_c3->cmd = c3; ast_c3->left = NULL; ast_c3->right = NULL;

    // --- C4: head -n 5 (No Redir) ---
    char **args_c4 = dup_args((const char *[]){"head", "-n", "5", NULL});
    t_cmd *c4 = malloc(sizeof(t_cmd)); c4->args = args_c4; c4->redir_count = 0; c4->redirs = NULL;
    t_ast *ast_c4 = malloc(sizeof(t_ast)); ast_c4->type = NODE_CMD; ast_c4->cmd = c4; ast_c4->left = NULL; ast_c4->right = NULL;

    // --- P1 (Inner Pipe): C1 | C2 ---
    t_ast *ast_pipe_inner = malloc(sizeof(t_ast));
    ast_pipe_inner->type = NODE_PIPE; ast_pipe_inner->cmd = NULL;
    ast_pipe_inner->left = ast_c1;
    ast_pipe_inner->right = ast_c2;

    // --- P2 (Mid Pipe): P1 | C3 ---
    t_ast *ast_pipe_mid = malloc(sizeof(t_ast));
    ast_pipe_mid->type = NODE_PIPE; ast_pipe_mid->cmd = NULL;
    ast_pipe_mid->left = ast_pipe_inner;
    ast_pipe_mid->right = ast_c3;

    // --- P3 (Outer Root Pipe): P2 | C4 ---
    t_ast *ast_root = malloc(sizeof(t_ast));
    ast_root->type = NODE_PIPE; ast_root->cmd = NULL;
    ast_root->left = ast_pipe_mid;
    ast_root->right = ast_c4;

    return (ast_root);
}

t_ast   *ft_bash()
{
    // --- Command 1: cat README.md ---
    const char *args_literal1[] = {"bash", NULL};
    char **args_cmd1 = dup_args(args_literal1); // Dynamically allocate args

    t_cmd *cmd1 = malloc(sizeof(t_cmd));
    if (!cmd1) return (NULL);
    cmd1->args = args_cmd1;
    cmd1->redir_count = 0;
    cmd1->redirs = NULL;

    t_ast *ast_cmd1 = malloc(sizeof(t_ast));
    if (!ast_cmd1) { /* Cleanup... */ return (NULL); }
    ast_cmd1->type = NODE_CMD;
    ast_cmd1->cmd = cmd1;
    ast_cmd1->left = NULL;
    ast_cmd1->right = NULL;

    // --- Root: NODE_PIPE ---
    t_ast *ast_pipe_root = ast_cmd1;
    return (ast_pipe_root);
}
