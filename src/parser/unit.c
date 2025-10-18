#include "libshell.h"
#include "lexer.h"
#include <stdio.h>
#include <string.h>

// A helper struct to define an expected token
typedef struct s_expected_token
{
    t_token_label type;
    const char    *value;
} t_expected_token;

// Helper function to safely compare strings (handles NULLs)
static int streq(const char *s1, const char *s2)
{
    if (s1 == NULL && s2 == NULL)
        return (1);
    if (s1 == NULL || s2 == NULL)
        return (0);
    return (strcmp(s1, s2) == 0);
}

// The main test runner function
static int run_test(char *input, t_expected_token *expected_tokens, int num_expected)
{
    t_lexer *lexer;
    t_token *token;
    int     i;

    printf("--- Test: [%s] ---\n", input);
    lexer = ft_state_lexer(input);
    if (!lexer)
    {
        printf("[FAIL] Lexer initialization failed.\n\n");
        return (0);
    }

    i = 0;
    while (i < num_expected)
    {
        token = get_next_token(lexer);
        if (!token)
        {
            printf("[FAIL] Token %d: get_next_token() returned NULL unexpectedly.\n\n", i);
            free_lexer(lexer);
            return (0);
        }

        // 1. Check the token type (using your struct member `tok_label`)
        if (token->tok_label != expected_tokens[i].type)
        {
            printf("[FAIL] Token %d: Type mismatch. Expected %d, Got %d\n",
                   i, expected_tokens[i].type, token->tok_label);
            free_token(token);
            free_lexer(lexer);
            return (0);
        }

        // 2. Check the token value (using your struct member `str`)
        if (!streq(token->str, expected_tokens[i].value))
        {
            printf("[FAIL] Token %d: Value mismatch. Expected \"%s\", Got \"%s\"\n",
                   i, expected_tokens[i].value, token->str);
            free_token(token);
            free_lexer(lexer);
            return (0);
        }

        printf("  [OK] Token %d: Type %d, Value \"%s\"\n",
               i, token->tok_label, token->str ? token->str : "NULL");

        free_token(token);
        i++;
    }

    // After all expected tokens, we must get TOKEN_EOF
    token = get_next_token(lexer);
    if (token->tok_label != TOKEN_EOF)
    {
        printf("[FAIL] Did not get TOKEN_EOF at the end. Got type %d instead.\n\n", token->tok_label);
        free_token(token);
        free_lexer(lexer);
        return (0);
    }

    printf("  [OK] Token %d: Type %d, Value \"NULL\"\n", i, token->tok_label);
    printf("[PASS] Test completed successfully.\n\n");
    free_token(token);
    free_lexer(lexer);
    return (1);
}

int main(void)
{
    int pass_count = 0;
    int total_tests = 0;

    // --- Test 1: Simple Command ---
    total_tests++;
    char test1_input[] = "ls -l";
    t_expected_token test1[] = {
        {TOKEN_WORD, "ls"},
        {TOKEN_WORD, "-l"}
    };
    pass_count += run_test(test1_input, test1, 2);

    // --- Test 2: Pipes and Redirections ---
    total_tests++;
    char test2_input[] = "cat < in.txt | wc -l > out.txt";
    t_expected_token test2[] = {
        {TOKEN_WORD, "cat"},
        {TOKEN_REDIR_IN, "<"},
        {TOKEN_WORD, "in.txt"},
        {TOKEN_PIPE, "|"},
        {TOKEN_WORD, "wc"},
        {TOKEN_WORD, "-l"},
        {TOKEN_REDIR_OUT, ">"},  // This test will FAIL if you haven't fixed the bug!
        {TOKEN_WORD, "out.txt"}
    };
    pass_count += run_test(test2_input, test2, 8);

    // --- Test 3: Quoting ---
    total_tests++;
    char test3_input[] = "echo 'hello world' \"it's a test\"";
    t_expected_token test3[] = {
        {TOKEN_WORD, "echo"},
        {TOKEN_WORD, "'hello world'"},
        {TOKEN_WORD, "\"it's a test\""}
    };
    pass_count += run_test(test3_input, test3, 3);

    // --- Test 4: Bunched Quoting (Critical!) ---
    total_tests++;
    char test4_input[] = "echo cmd'foo'bar\"baz\"";
    t_expected_token test4[] = {
        {TOKEN_WORD, "echo"},
        {TOKEN_WORD, "cmd'foo'bar\"baz\""}
    };
    pass_count += run_test(test4_input, test4, 2);

    // --- Test 5: Operators and Parentheses ---
    total_tests++;
    char test5_input[] = "(ls) && echo done || cat";
    t_expected_token test5[] = {
        {TOKEN_LEFT_PAR, "("},
        {TOKEN_WORD, "ls"},
        {TOKEN_RIGHT_PAR, ")"},
        {TOKEN_AND, "&&"},
        {TOKEN_WORD, "echo"},
        {TOKEN_WORD, "done"},
        {TOKEN_OR, "||"},
        {TOKEN_WORD, "cat"}
    };
    pass_count += run_test(test5_input, test5, 8);
    
    // --- Test 6: Heredoc (Critical!) ---
    total_tests++;
    char test6_input[] = "grep a << \"EOF\""; // Delimiter with quotes
    t_expected_token test6[] = {
        {TOKEN_WORD, "grep"},
        {TOKEN_WORD, "a"},
        {TOKEN_REDIR_HEREDOC, "EOF"} // Value must be unquoted!
    };
    pass_count += run_test(test6_input, test6, 3);

    // --- Test 7: Error - Unclosed Quote ---
    total_tests++;
    char test7_input[] = "echo \"hello";
    t_expected_token test7[] = {
        {TOKEN_WORD, "echo"},
        {TOKEN_ERROR, "Unclosed word."}
    };
    pass_count += run_test(test7_input, test7, 2);

    // --- Test 8: Error - Single '&' ---
    total_tests++;
    char test8_input[] = "ls &";
    t_expected_token test8[] = {
        {TOKEN_WORD, "ls"},
        {TOKEN_ERROR, "Unsuported operator"}
    };
    pass_count += run_test(test8_input, test8, 2);

    // --- Test 9: Heredoc (No Quotes) ---
    total_tests++;
    char test9_input[] = "cat << END"; // Delimiter without quotes
    t_expected_token test9[] = {
        {TOKEN_WORD, "cat"},
        {TOKEN_REDIR_HEREDOC, "END"}
    };
    pass_count += run_test(test9_input, test9, 2);

    // --- Final Report ---
    printf("========================================\n");
    printf("Test Summary: %d / %d tests passed.\n", pass_count, total_tests);
    printf("========================================\n");

    // Return 1 (error) if any test failed
    return (pass_count == total_tests ? 0 : 1);
}
