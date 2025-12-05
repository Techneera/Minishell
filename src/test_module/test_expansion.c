#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expansion.h" 
// Ensure expansion.h includes prototypes for:
// ft_strdup, ft_strjoin, ft_substr, ft_isalnum, ft_itoa, ft_strlen

/* ==========================================
 * MOCK / HELPER FUNCTIONS
 * ========================================== */

// Mock implementation of ft_getenv to search a char **envp array
char *ft_getenv(char *name, char **envp)
{
    int i = 0;
    int name_len = strlen(name);

    if (!envp || !name)
        return (NULL);
    
    while (envp[i])
    {
        // Check if envp[i] starts with "name="
        if (strncmp(envp[i], name, name_len) == 0 && envp[i][name_len] == '=')
        {
            return (envp[i] + name_len + 1); // Return value part
        }
        i++;
    }
    return (NULL);
}

// Helper to print test results nicely
void assert_expansion(char *test_name, char *input, char *expected, char **envp, int status)
{
    char *result = expand_word(input, envp, status);
    
    if (result && expected && strcmp(result, expected) == 0)
    {
        printf("[\033[32mPASS\033[0m] %s\n", test_name);
    }
    else
    {
        printf("[\033[31mFAIL\033[0m] %s\n", test_name);
        printf("   Input:    [%s]\n", input);
        printf("   Expected: [%s]\n", expected);
        printf("   Got:      [%s]\n", result ? result : "(null)");
    }
    if (result)
        free(result);
}

/* ==========================================
 * MAIN TEST HARNESS
 * ========================================== */

int main(void)
{
    // 1. Setup Mock Environment
    char *mock_env[] = {
        "USER=rluis-ya@student.42porto.com",
        "HOME=/home/rluis-ya",
        "EMPTY=",
        "VAR_WITH_SPACES=has spaces inside",
        NULL
    };

    printf("=== Starting Expansion Test Suite ===\n\n");

    // --- Category 1: Basic Strings (No Expansion) ---
    assert_expansion("Basic Word", "hello", "hello", mock_env, 0);
    assert_expansion("Empty String", "", "", mock_env, 0);
    assert_expansion("Number expansion", "$123", "23", mock_env, 0);
    assert_expansion("Number expansion 1", "$1", "", mock_env, 0);
    assert_expansion("Number expansion 2", "$1adfk23", "adfk23", mock_env, 0);

    // --- Category 2: Single Quotes (Rule A: No Expansion) ---
    assert_expansion("Single Quote Basic", "'hello'", "hello", mock_env, 0);
    assert_expansion("Single Quote Var", "'$USER'", "$USER", mock_env, 0);
    assert_expansion("Single Quote Mixed", "'hello world'", "hello world", mock_env, 0);

    // --- Category 3: Double Quotes (Rule B: Expand $) ---
    assert_expansion("Double Quote Basic", "\"hello\"", "hello", mock_env, 0);
    assert_expansion("Double Quote Var", "\"$USER\"", "tester", mock_env, 0);
    assert_expansion("Double Quote Sentence", "\"Hello $USER!\"", "Hello tester!", mock_env, 0);
    assert_expansion("Double Quote Unknown", "\"$INVALID\"", "", mock_env, 0); // Should be empty

    // --- Category 4: General State Expansion (Rule C) ---
    assert_expansion("General Var", "$USER", "tester", mock_env, 0);
    assert_expansion("General Unknown", "$INVALID", "", mock_env, 0);
    assert_expansion("General Concatenation", "user is $USER", "user is tester", mock_env, 0);
    assert_expansion("Adjacent Vars", "$USER$HOME", "tester/home/test", mock_env, 0);

    // --- Category 5: Exit Status (Rule D: $?) ---
    assert_expansion("Exit Status 0", "$?", "0", mock_env, 0);
    assert_expansion("Exit Status 42", "$?", "42", mock_env, 42);
    assert_expansion("Exit Status in DQuote", "\"$?\"", "127", mock_env, 127);

    // --- Category 6: Mixed Quotes & Complex Edge Cases ---
    assert_expansion("Mixed S and D Quotes", "\"$USER\"\'$USER\'", "tester$USER", mock_env, 0);
    assert_expansion("Mixed D and S Quotes", "\'$USER\'\"$USER\"", "$USERtester", mock_env, 0);
    assert_expansion("Mixed S, D Quotes and S Quotes", "\'$USER\'$USER\"$USER\"", "$USERtestertester", mock_env, 0);
    assert_expansion("Quotes inside Quotes 1", "\"'hello'\"", "'hello'", mock_env, 0); // Double containing Single
    assert_expansion("Quotes inside Quotes 2", "'\"$USER\"'", "\"$USER\"", mock_env, 0); // Single containing Double
    assert_expansion("Lone Dollar", "$", "$", mock_env, 0);
    assert_expansion("Dollar Space", "$ ", "$ ", mock_env, 0);
    assert_expansion("Var with trailing text", "$USER;", "tester;", mock_env, 0);
    
    // --- Category 7: Empty Variables ---
    assert_expansion("Empty Var", "$EMPTY", "", mock_env, 0);
    assert_expansion("Empty Var in String", "a$EMPTYb", "ab", mock_env, 0);

    printf("\n=== Tests Finished ===\n");
    return (0);
}
