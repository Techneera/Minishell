/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libshell.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LIBSHELL_H
# define LIBSHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stddef.h>
# include <stdbool.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include "libft.h"

/** \brief Shell prompt string displayed before each command. */
# define PROMPT            "minishell$> "
# define TC_BLACK          "\001\033[30m\002"
# define TC_RED            "\001\033[31m\002"
# define TC_GREEN          "\001\033[32m\002"
# define TC_YELLOW         "\001\033[33m\002"
# define TC_BLUE           "\001\033[34m\002"
# define TC_MAGENTA        "\001\033[35m\002"
# define TC_CYAN           "\001\033[36m\002"
# define TC_WHITE          "\001\033[37m\002"
# define TC_BRIGHT_BLACK   "\001\033[90m\002"
# define TC_BRIGHT_RED     "\001\033[91m\002"
# define TC_BRIGHT_GREEN   "\001\033[92m\002"
# define TC_BRIGHT_YELLOW  "\001\033[93m\002"
# define TC_BRIGHT_BLUE    "\001\033[94m\002"
# define TC_BRIGHT_MAGENTA "\001\033[95m\002"
# define TC_BRIGHT_CYAN    "\001\033[96m\002"
# define TC_BRIGHT_WHITE   "\001\033[97m\002"

/**
 * \brief Redirection type label.
 *
 * Identifies the kind of I/O redirection attached to a command node.
 */
typedef enum e_label_redir
{
	REDIR_NONE,      /**< No redirection (default / uninitialised). */
	REDIR_IN,        /**< Input redirection: `< file`. */
	REDIR_OUT,       /**< Output redirection (truncate): `> file`. */
	REDIR_APPEND,    /**< Output redirection (append): `>> file`. */
	REDIR_HEREDOCK   /**< Heredoc: `<< DELIM`. */
}	t_label_redir;

/**
 * \brief Token type label produced by the lexer.
 *
 * Each value corresponds to one syntactic token class.
 */
typedef enum e_token_label
{
	TOKEN_ERROR,         /**< Unrecognised or invalid token. */
	TOKEN_WORD,          /**< A word (command name, argument, or quoted string). */
	TOKEN_PIPE,          /**< The pipe operator `|`. */
	TOKEN_AND,           /**< The logical AND operator `&&`. */
	TOKEN_OR,            /**< The logical OR operator `||`. */
	TOKEN_RIGHT_PAR,     /**< Closing parenthesis `)`. */
	TOKEN_LEFT_PAR,      /**< Opening parenthesis `(`. */
	TOKEN_REDIR_IN,      /**< Input redirection `<`. */
	TOKEN_REDIR_OUT,     /**< Output redirection `>`. */
	TOKEN_REDIR_APPEND,  /**< Append redirection `>>`. */
	TOKEN_REDIR_HEREDOC, /**< Heredoc `<<`; \c str holds the raw delimiter. */
	TOKEN_EOF            /**< End of input sentinel. */
}	t_token_label;

/**
 * \brief AST node type.
 *
 * Determines which fields of \c t_ast are meaningful for a given node.
 */
typedef enum e_node_type
{
	NODE_EMPTY,    /**< Uninitialised / placeholder node. */
	NODE_CMD,      /**< A simple command; \c cmd holds args and redirections. */
	NODE_PIPE,     /**< A pipe `|`; uses \c left and \c right subtrees. */
	NODE_SUBSHELL, /**< A subshell `( body )`; uses \c body and \c cmd (ext. redirs). */
	NODE_AND,      /**< Logical AND `&&`; uses \c left and \c right subtrees. */
	NODE_OR        /**< Logical OR `||`; uses \c left and \c right subtrees. */
}	t_node_type;

/**
 * \brief A single lexer token.
 *
 * Tokens form a singly-linked list consumed by the parser.
 */
typedef struct s_token
{
	char			*str;       /**< The raw text of the token (heap-allocated). */
	t_token_label	tok_label;  /**< Semantic classification of the token. */
	struct s_token	*next;      /**< Pointer to the next token in the list. */
}	t_token;

/**
 * \brief Lexer cursor state.
 *
 * Holds a pointer to the raw input string and a position cursor.
 * Does NOT own the \c input string (owned by the REPL).
 */
typedef struct s_lexer
{
	char		*input; /**< The raw command-line string (not owned). */
	size_t		pos;    /**< Current read position within \c input. */
	size_t		len;    /**< Total length of \c input. */
}	t_lexer;

/**
 * \brief Parser state with one-token look-ahead.
 *
 * Holds the lexer reference and the two current look-ahead tokens.
 */
typedef struct s_parser
{
	t_lexer	*lex;           /**< The lexer used to fetch new tokens. */
	t_token	*current_token; /**< The token currently being examined. */
	t_token	*peek;          /**< The next token (one-token look-ahead). */
}	t_parser;

/**
 * \brief A single I/O redirection descriptor.
 *
 * Stores the target filename and the redirection type.
 */
typedef struct s_redir
{
	char			*file_name; /**< Heap-allocated target filename or heredoc delimiter. */
	t_label_redir	label;      /**< The type of this redirection. */
}	t_redir;

/**
 * \brief A parsed simple command.
 *
 * Holds the argument vector and the array of I/O redirections.
 */
typedef struct s_cmd
{
	char	**args;       /**< NULL-terminated argument array (argv-style). */
	int		redir_count;  /**< Number of entries in the \c redirs array. */
	t_redir	*redirs;      /**< Array of \c redir_count redirection descriptors. */
}	t_cmd;

/**
 * \brief An Abstract Syntax Tree node.
 *
 * Forms the execution tree produced by the parser.
 * Which fields are valid depends on \c type.
 */
typedef struct s_ast
{
	t_cmd			*cmd;   /**< Command data (valid for NODE_CMD and NODE_SUBSHELL). */
	t_node_type		type;   /**< The semantic type of this node. */
	struct s_ast	*body;  /**< Subshell body (valid for NODE_SUBSHELL). */
	struct s_ast	*left;  /**< Left operand (valid for NODE_PIPE/AND/OR). */
	struct s_ast	*right; /**< Right operand (valid for NODE_PIPE/AND/OR). */
}	t_ast;

/**
 * \brief Accumulator context used while parsing a simple command.
 *
 * Collects argument and redirection lists incrementally before
 * they are finalised into a \c t_cmd / \c t_ast node.
 */
typedef struct s_simp_ctx
{
	t_list	*args;      /**< Linked list of heap-allocated argument strings. */
	t_list	*redirs;    /**< Linked list of heap-allocated \c t_redir nodes. */
	t_cmd	*cmd;       /**< The command being assembled (set during finalisation). */
	t_ast	*node;      /**< The resulting AST node (set during finalisation). */
	t_redir	*tmp_redir; /**< Temporary redir pointer used during parsing. */
}	t_simp_ctx;

/**
 * \brief A single environment variable entry.
 *
 * Stored in \c t_data.env_list.
 */
typedef struct s_env
{
	int		has_arg;   /**< Non-zero if the variable has an associated value. */
	char	*variable; /**< The full "KEY=value" string (heap-allocated). */
}	t_env;

/**
 * \brief Counts of resources in one execution cycle.
 *
 * Used to pre-allocate arrays in \c t_fds.
 */
typedef struct s_get
{
	int		n_files; /**< Number of non-heredoc file redirections. */
	int		n_cmds;  /**< Number of command / subshell nodes. */
	int		n_docs;  /**< Number of heredoc redirections. */
}	t_get;

/**
 * \brief Positional index cursors for resource arrays.
 *
 * Each counter advances as the execution tree is traversed so
 * that every node claims the correct pre-allocated descriptor.
 */
typedef struct s_pos
{
	int		file_id; /**< Index into \c t_fds.fd_files for the current node. */
	int		doc_id;  /**< Index into \c t_fds.heredoc_fds for the current node. */
	int		fork_id; /**< Index into \c t_fds.c_pids for the current fork. */
}	t_pos;

/**
 * \brief Pre-allocated file descriptor table for one execution cycle.
 *
 * All file descriptors and child PIDs are allocated before any fork
 * so that child processes can read their assigned fds by index.
 */
typedef struct s_fds
{
	int		**heredoc_fds; /**< 2-D array [doc_id][0=read,1=write] for heredocs. */
	int		*fd_files;     /**< Opened fds for each non-heredoc redirection. */
	int		*c_pids;       /**< PIDs of all forked child processes. */
	t_get	get;           /**< Resource counts (n_files, n_cmds, n_docs). */
	t_pos	pos;           /**< Current positional indexes during tree traversal. */
}	t_fds;

/**
 * \brief Global shell state passed through the entire execution pipeline.
 *
 * One instance lives on the stack inside the REPL loop.
 * Every subsystem receives a pointer to this struct.
 */
typedef struct s_data
{
	t_ast	*root;     /**< Root of the AST for the current input line. */
	t_ast	*tree;     /**< Execution cursor pointing to the current node. */
	t_fds	*fds;      /**< Pre-allocated file descriptors and PID array. */
	char	**envp;    /**< Flat environment array passed to execve(). */
	t_list	*env_list; /**< Linked list of t_env nodes (authoritative env store). */
	char	*rl;       /**< Raw readline input string (heap-allocated). */
	t_lexer	*lexer;   /**< Lexer state for the current input line. */
}	t_data;

/**
 * \brief Temporary state used during wildcard glob expansion.
 *
 * Splits a pattern into a directory prefix and a filename pattern,
 * then accumulates matching filesystem entries.
 */
typedef struct s_globs
{
	char	*dir_prefix;   /**< Directory portion of the pattern (may be NULL). */
	char	*file_pattern; /**< Filename portion of the pattern. */
	char	*last_slash;   /**< Pointer to the last '/' in the original pattern. */
	char	*full_match;   /**< Assembled full path for each matched entry. */
}	t_globs;

#endif
