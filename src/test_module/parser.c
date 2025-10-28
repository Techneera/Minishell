#include "ast.h"    // Your AST/Parser headers
#include "lexer.h"  // Your Lexer headers
#include <stdio.h>    // For printf
#include <string.h>   // For strdup, strcmp

/*
 * ===================================================================
 * YOUR FUNCTION PROTOTYPES
 * ===================================================================
 * This file assumes you have these functions available from your
 * other .c files.
 */

// From Lexer:
t_lexer *ft_state_lexer(char *line);
void    free_lexer(t_lexer *l); // You need this!
void    free_token(t_token *t); // You need this!

// From Parser:
t_ast   *ft_parser(t_lexer *l);
void    ft_free_ast(t_ast *root);

/*
 * ===================================================================
 * AST PRETTY-PRINTER (For Test Verification)
 * ===================================================================
 * A robust print_ast is necessary to verify the tree.
 */

static void print_indent(int depth)
{
    for (int i = 0; i < depth; i++)
        printf("  ");
}

static const char *redir_label_to_str(t_label_redir label)
{
    if (label == REDIR_IN) return "<";
    if (label == REDIR_OUT) return ">";
    if (label == REDIR_APPEND) return ">>";
    if (label == REDIR_HEREDOCK) return "<<";
    return "?";
}

// Helper to print a command's args and redirections
static void print_command_details(t_cmd *cmd, int depth)
{
    if (!cmd)
        return;

    // Print Args
    if (cmd->args)
    {
        print_indent(depth);
        printf("Args: ");
        for (int i = 0; cmd->args[i]; i++)
            printf("[%s] ", cmd->args[i]);
        printf("\n");
    }

    // Print Redirections
    if (cmd->redirs)
    {
        print_indent(depth);
        printf("Redirs: ");
        t_redir *r = cmd->redirs;
        while (r)
        {
            printf("(%s %s) ", redir_label_to_str(r->label), r->file_name);
            r = r->next;
        }
        printf("\n");
    }
}

// Main recursive AST printer
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
            print_command_details(node->cmd, depth + 1);
            break;
        case NODE_SUBSHELL:
            printf("SUBSHELL ( ... )\n");
            // Print redirections attached *to the subshell*
            print_command_details(node->cmd, depth + 1);
            // Print the subshell's body
            print_ast(node->body, depth + 1);
            break;
        default:
            printf("UNKNOWN NODE TYPE\n");
    }
}

/*
 * ===================================================================
 * TEST HARNESS
 * ===================================================================
 */

// Helper to run a single test case
void run_test(char *input_line)
{
    t_lexer *lexer;
    t_ast   *ast_root;
    
    // We need a mutable copy for the lexer
    char *input_copy = strdup(input_line);

    printf("\n======================================================\n");
    printf("TESTING: %s\n", input_line);
    printf("------------------------------------------------------\n");

    lexer = ft_state_lexer(input_copy);
    if (!lexer)
    {
        printf("TEST FAILED: Lexer initialization returned NULL.\n");
        free(input_copy);
        return;
    }

    ast_root = ft_parser(lexer);

    if (ast_root == NULL)
    {
        printf("AST: [NULL] (Syntax Error Detected)\n");
    }
    else
    {
        print_ast(ast_root, 0);
    }

    // Full cleanup
    ft_free_ast(ast_root);
    free_lexer(lexer); // Assumes you have a free_lexer
    free(input_copy);
}

int main(void)
{
    // --- Category 1: Simple Commands ---
    run_test("ls");
    run_test("ls -la /tmp");

    // --- Category 2: Redirections ---
    run_test("cat < in.txt");
    run_test("echo hello > out.txt");
    run_test("grep a << EOF");
    run_test("echo append >> log.txt");
    run_test("cmd < in1 > out1 < in2 >> out2"); // Multiple and order
    
    // --- Category 3: Pipes ---
    run_test("ls | wc -l");
    run_test("cmd1 | cmd2 | cmd3"); // Left-associativity
    run_test("cat < in.txt | grep 'a' | wc -l > out.txt"); // Pipes + Redirs

    // --- Category 4: Boolean Operators (&& ||) ---
    run_test("ls && echo ok");
    run_test("ls || echo failed");
    run_test("cmd1 && cmd2 || cmd3"); // Associativity: (cmd1 && cmd2) || cmd3
    run_test("cmd1 || cmd2 && cmd3"); // Associativity: (cmd1 || cmd2) && cmd3
    
    // --- Category 5: Precedence (Pipes vs. Booleans) ---
    run_test("ls | wc && cat file"); // (ls | wc) && (cat file)
    run_test("cat file && ls | wc"); // (cat file) && (ls | wc)
    run_test("cmd1 | cmd2 && cmd3 | cmd4"); // (cmd1 | cmd2) && (cmd3 | cmd4)

    // --- Category 6: Subshells (Parentheses) ---
    run_test("(ls)");
    run_test("(ls | wc -l)"); // Pipe in subshell
    run_test("(ls && echo ok) || echo failed"); // Boolean in subshell
    run_test("(ls) > out.txt"); // Redir ON subshell
    run_test("(ls > in.txt) > out.txt"); // Redir IN and ON subshell
    
    // --- Category 7: Complex Combinations ---
    run_test("(cmd1 < in && (cmd2 | cmd3)) > out || cmd4 << EOF");

    // --- Category 8: Syntax Errors (Should return NULL) ---
    run_test("| ls");
    run_test("ls |");
    run_test("ls | | wc");
    run_test("&& ls");
    run_test("ls &&");
    run_test("ls >");
    run_test("ls > | wc");
    run_test("(ls");
    run_test("ls )");
    run_test("()");

    return (0);
}
