#include "ast.h"    // Your AST/Parser headers
#include "lexer.h"  // Your Lexer headers
#include <stdio.h>    // For printf
#include <string.h>   // For strdup, strcmp

/*
 * ===================================================================
 * YOUR FUNCTION PROTOTYPES (Assumed to exist)
 * ===================================================================
 */

// From Lexer (or libshell.h)
t_lexer *ft_state_lexer(char *line);
void    free_lexer(t_lexer *l); // Make sure you have this!
void    free_token(t_token *t); // Make sure you have this!
// ... and all your libft functions (ft_strdup, etc.)

// From Parser (core.txt, ft_ast.txt, guards.txt)
t_ast   *ft_parser(t_lexer *l);
void    ft_free_ast(t_ast *root);
void	ft_free_redir_content(void *content);
void    ft_free_redir_struct_only(void *content);


/*
 * ===================================================================
 * AST PRETTY-PRINTER (For Test Verification)
 * ===================================================================
 * This printer is built to match your exact structs.
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

// Helper to print a command's args and its REDIRECTION ARRAY
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

	// Print Redirections (from the array)
	if (cmd->redirs && cmd->redir_count > 0)
	{
		print_indent(depth);
		printf("Redirs (count: %d): ", cmd->redir_count);
		for (int i = 0; i < cmd->redir_count; i++)
		{
			// Access the struct *at the array index*
			t_redir *r = &cmd->redirs[i];
			printf("(%s %s) ", redir_label_to_str(r->label), r->file_name);
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
			// A subshell can have its *own* redirections
			print_command_details(node->cmd, depth + 1);
			// And it has a body
			print_indent(depth);
			printf("Body:\n");
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
static int run_test(char *input_line)
{
	t_lexer *lexer;
	t_ast   *ast_root;
	int     success = 1;
	
	// We need a mutable copy for the lexer
	char *input_copy = ft_strdup(input_line);
	if (!input_copy)
	{
		printf("FATAL: strdup failed in test harness.\n");
		return (0);
	}

	printf("\n======================================================\n");
	printf("TESTING: %s\n", input_line);
	printf("------------------------------------------------------\n");

	lexer = ft_state_lexer(input_copy);
	if (!lexer)
	{
		printf("TEST FAILED: Lexer initialization returned NULL.\n");
		free(input_copy);
		return (0);
	}

	ast_root = ft_parser(lexer);// This calls your entry point [cite: 24]

	if (ast_root == NULL)
	{
		printf("AST: [NULL] (Syntax Error Detected)\n");
		// This is the *correct* outcome for syntax error tests
	}
	else
	{
		print_ast(ast_root, 0);
	}

	// Full cleanup
	ft_free_ast(ast_root);
	free_lexer(lexer); // You must provide this
	free(input_copy);
	
	// This is a simple verification. A real test would compare
	// the AST to an expected structure.
	return (success);
}

int main(void)
{
	int pass_count = 0;
	int total_tests = 0;

	// --- Category 1: Simple Commands ---
	total_tests++; pass_count += run_test("ls");
	total_tests++; pass_count += run_test("ls -la /tmp");

	// --- Category 2: Redirections (Tests the new array) ---
	total_tests++; pass_count += run_test("cat < in.txt");
	total_tests++; pass_count += run_test("echo hello > out.txt");
	total_tests++; pass_count += run_test("grep a << EOF");
	total_tests++; pass_count += run_test("echo append >> log.txt");
	total_tests++; pass_count += run_test("cmd < in1 > out1 < in2 >> out2"); // CRITICAL: Multiple redirs
	
	// --- Category 3: Pipes ---
	total_tests++; pass_count += run_test("ls | wc -l");
	total_tests++; pass_count += run_test("cmd1 | cmd2 | cmd3"); // Left-associativity
	total_tests++; pass_count += run_test("cat < in.txt | grep 'a' | wc -l > out.txt"); // Pipes + Redirs

	// --- Category 4: Boolean Operators (&& ||) ---
	total_tests++; pass_count += run_test("ls && echo ok");
	total_tests++; pass_count += run_test("ls || echo failed");
	total_tests++; pass_count += run_test("cmd1 && cmd2 || cmd3"); // Associativity: (cmd1 && cmd2) || cmd3
	total_tests++; pass_count += run_test("cmd1 || cmd2 && cmd3"); // Associativity: (cmd1 || cmd2) && cmd3
	
	// --- Category 5: Precedence (Pipes vs. Booleans) ---
	total_tests++; pass_count += run_test("ls | wc && cat file"); // (ls | wc) && (cat file)
	total_tests++; pass_count += run_test("cat file && ls | wc"); // (cat file) && (ls | wc)
	
	// --- Category 6: Subshells (The other major feature) ---
	total_tests++; pass_count += run_test("(ls)");
	total_tests++; pass_count += run_test("(ls | wc -l)"); // Pipe in subshell
	total_tests++; pass_count += run_test("(ls && echo ok) || echo failed"); // Boolean in subshell
	total_tests++; pass_count += run_test("(ls) > out.txt"); // CRITICAL: Redir ON subshell
	total_tests++; pass_count += run_test("(ls > in.txt) > out.txt"); // Redir IN and ON subshell
	
	// --- Category 7: Complex Combinations ---
	total_tests++; pass_count += run_test("(cmd1 < in && (cmd2 | cmd3)) > out || cmd4 << EOF");

	// --- Category 8: Edge Cases ---
	total_tests++; pass_count += run_test("> out");
	total_tests++; pass_count += run_test("< in");
	total_tests++; pass_count += run_test(">> app");
	total_tests++; pass_count += run_test("<< here");
	total_tests++; pass_count += run_test("ls > out -la");
	total_tests++; pass_count += run_test("< ~/Documents ls -l");

	// --- Category 9: Syntax Errors (Should return NULL) ---
	printf("\n======================================================\n");
	printf("TESTING SYNTAX ERRORS (Should return [NULL])\n");
	printf("======================================================\n");
	total_tests++; pass_count += run_test("| ls");
	total_tests++; pass_count += run_test("ls |");
	total_tests++; pass_count += run_test("ls | | wc");
	total_tests++; pass_count += run_test("&& ls");
	total_tests++; pass_count += run_test("ls &&");
	total_tests++; pass_count += run_test("ls >");
	total_tests++; pass_count += run_test("ls > | wc");
	total_tests++; pass_count += run_test("(ls");
	total_tests++; pass_count += run_test("ls )");
	total_tests++; pass_count += run_test("()"); // Your subshell fix

	printf("\n======================================================\n");
	printf("Test Suite Finished. (%d tests run)\n", total_tests);
	printf("======================================================\n");

	return (0);
}
