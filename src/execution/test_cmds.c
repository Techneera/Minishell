#include "execution.h"

#include <stdlib.h>
#include <string.h>

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
    char **new_args = ft_calloc(sizeof(char *), (count + 1));
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
    new_args[count] = NULL;
    return (new_args);
}

// Helper to create a NODE_CMD (for simpler code, matching your structure)
t_ast *new_cmd_node(char **args) {
    t_cmd *cmd = ft_calloc(sizeof(t_cmd), 1);
    if (!cmd) return (NULL);
    cmd->args = args;
    cmd->redir_count = 0;
    cmd->redirs = NULL;

    t_ast *node = ft_calloc(sizeof(t_ast), 1);
    if (!node) { free(cmd); return (NULL); }
    node->type = NODE_CMD;
    node->cmd = cmd;
    node->left = NULL;
    node->right = NULL;
    node->body = NULL;
    return (node);
}

// Helper to create an operator node (NODE_PIPE, NODE_AND, NODE_OR, NODE_SUBSHELL)
t_ast *new_op_node(t_node_type type, t_ast *left, t_ast *right) {
    t_ast *node = ft_calloc(sizeof(t_ast), 1);
    if (!node) return (NULL);
    node->type = type;
    node->body = NULL;
    node->cmd = NULL;
    node->left = left;
    node->right = right;
    return (node);
}

t_ast	*bonus_cmd()
{
    t_ast *ast_c1 = new_cmd_node(dup_args((const char *[]){"echo", "QUEM MECHER CODA EM PYTHON",NULL}));
    t_ast *ast_c2 = new_cmd_node(dup_args((const char *[]){"cat", NULL}));

    ast_c2->cmd->redir_count = 1;
    ast_c2->cmd->redirs = ft_calloc(sizeof(t_redir), 1);
    ast_c2->cmd->redirs[0].file_name = strdup("a");
    ast_c2->cmd->redirs[0].label = REDIR_IN;

    t_ast *ast_sub = new_op_node(NODE_AND, ast_c1, ast_c2);
    t_ast *ast_root = ast_sub;
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

t_ast	*ft_cmd1()
{
    t_ast *ast_c1 = new_cmd_node(dup_args((const char *[]){"banana", NULL}));
    t_ast *ast_c2 = new_cmd_node(dup_args((const char *[]){"sort", "-r", NULL}));
    t_ast *ast_c3 = new_cmd_node(dup_args((const char *[]){"uniq", "-r", NULL}));
    t_ast *ast_c4 = new_cmd_node(dup_args((const char *[]){"head", "-n", "5", NULL}));

    ast_c1->cmd->redir_count = 1;
    ast_c1->cmd->redirs = ft_calloc(sizeof(t_redir), 1);
    ast_c1->cmd->redirs[0].file_name = strdup("file_list.txt");
    ast_c1->cmd->redirs[0].label = REDIR_OUT;

    t_ast *ast_pipe1 = new_op_node(NODE_PIPE, ast_c1, ast_c2);
    t_ast *ast_pipe2 = new_op_node(NODE_PIPE, ast_pipe1, ast_c3);
    t_ast *ast_pipe3 = new_op_node(NODE_PIPE, ast_pipe2, ast_c4);

    t_ast *ast_root = ast_pipe3;
    return (ast_root);
}

t_ast	*ft_cmd2()
{
    t_ast *ast_c1 = new_cmd_node(dup_args((const char *[]){"ls", NULL}));

    ast_c1->cmd->redir_count = 3;
    ast_c1->cmd->redirs = ft_calloc(sizeof(t_redir), ast_c1->cmd->redir_count);
    ast_c1->cmd->redirs[0].file_name = strdup("o1");
    ast_c1->cmd->redirs[0].label = REDIR_IN;
    ast_c1->cmd->redirs[1].file_name = strdup("o2");
    ast_c1->cmd->redirs[1].label = REDIR_OUT;
    ast_c1->cmd->redirs[2].file_name = strdup("o3");
    ast_c1->cmd->redirs[2].label = REDIR_OUT;
    t_ast *ast_root = ast_c1;
    return (ast_root);
}

t_ast	*ft_cmd3()
{
    t_ast *ast_c1 = new_cmd_node(dup_args((const char *[]){NULL}));

    ast_c1->cmd->redir_count = 5;
    ast_c1->cmd->redirs = ft_calloc(ast_c1->cmd->redir_count, sizeof(t_redir));
    ast_c1->cmd->redirs[0].file_name = strdup("END");
    ast_c1->cmd->redirs[0].label = REDIR_HEREDOCK;

    ast_c1->cmd->redirs[1].file_name = strdup("a");
    ast_c1->cmd->redirs[1].label = REDIR_IN;

    ast_c1->cmd->redirs[2].file_name = strdup("b");
    ast_c1->cmd->redirs[2].label = REDIR_OUT;

    ast_c1->cmd->redirs[3].file_name = strdup("c");
    ast_c1->cmd->redirs[3].label = REDIR_OUT;

    ast_c1->cmd->redirs[4].file_name = strdup("d");
    ast_c1->cmd->redirs[4].label = REDIR_IN;
    t_ast *ast_root = ast_c1;
    return (ast_root);
}
