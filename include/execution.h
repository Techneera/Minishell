/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:56:25 by rluis-ya@stud    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EXECUTION_H
# define EXECUTION_H

# define CMD_NOT_FOUND 127 /**< Exit status when a command is not found in PATH. */
# define FAIL_STATUS 1     /**< Generic failure exit status. */

# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <limits.h>
# include "libshell.h"
# include "ast.h"
# include "lexer.h"
# include "expansion.h"

/**
 * \brief Process one readline line through lexer → parser → executor.
 * \param data The shell state (lexer and AST are populated here).
 * \return 1 to continue the REPL, 0 to exit.
 */
int		process_rl(t_data *data);

/**
 * \brief Resolve a command name to its full filesystem path.
 * \param arg  The argument vector (arg[0] is the command name).
 * \param env  The environment array used to read PATH.
 * \param data The shell state (used for error reporting).
 * \return Heap-allocated full path string, or NULL if not found.
 */
char	*get_command_path(char **arg, char **env, t_data *data);

/**
 * \brief Replace wildcard args in an argument vector with filesystem matches.
 * \param old_args The original NULL-terminated argument array.
 * \param files    Linked list of matching filenames.
 * \param index    Position in \c old_args to replace.
 * \return New heap-allocated argument array with matches spliced in.
 */
char	**insert_wildcard_args(char **old_args, t_list *files, int index);

/**
 * \brief Restore masked `\001` bytes in a string back to `*`.
 * \param str The string to restore in place.
 */
void	unmask_wildcards(char *str);

/**
 * \brief Test if the command is a built-in and execute it if so.
 * \param data The shell state.
 * \param arg  The command name to test.
 * \return 1 if the command is a built-in (and was executed); 0 otherwise.
 */
int		ft_is_builtin(t_data *data, char *arg);

/**
 * \brief Look up a variable in the environment array.
 * \param env The NULL-terminated environment array (KEY=value format).
 * \param arg The variable name to search for.
 * \return Pointer to the value part of the matching entry, or NULL.
 */
char	*ft_getenv(char **env, char *arg);

/**
 * \brief Test whether a key already exists in the environment linked list.
 * \param env The environment node to inspect.
 * \param arg The key to look for.
 * \return Non-zero if the key matches.
 */
int		exist_in_list(t_env *env, char *arg);

/**
 * \brief Print all exported environment variables to stdout.
 * \param data The shell state.
 * \return 0 on success.
 */
int		ft_env(t_data *data);

/**
 * \brief Set or update one or more environment variables.
 * \param list The environment linked list.
 * \param args The argument vector (args[0] is "export"; args[1…] are KEY=val pairs).
 * \param data The shell state (env array is refreshed after changes).
 * \return 0 on success, 1 on error.
 */
int		ft_export(t_list *list, char **args, t_data *data);

/**
 * \brief Print all exported variables in sorted `declare -x` format.
 * \param list The environment linked list.
 * \param data The shell state.
 * \return 0 on success.
 */
int		ft_print_sorted_export(t_list *list, t_data *data);

/**
 * \brief Build the initial environment linked list from \c envp.
 * \param env The envp array passed to main().
 * \return Heap-allocated linked list of \c t_env nodes.
 */
t_list	*init_env(char **env);

/**
 * \brief Allocate a new environment linked-list node.
 * \param arg     The "KEY=value" string.
 * \param has_arg Non-zero if the variable has an assigned value.
 * \return Heap-allocated \c t_list node, or NULL on failure.
 */
t_list	*create_node_env(char *arg, int has_arg);

/**
 * \brief Convert the environment linked list to a flat envp array.
 * \param list The environment linked list.
 * \return Heap-allocated NULL-terminated \c char** array, or NULL on failure.
 */
char	**envlist_to_array(t_list *list);

/**
 * \brief Free a \c t_env content node (used as lstclear destructor).
 * \param content The \c t_env pointer cast to void*.
 */
void	ft_free_content(void *content);

/**
 * \brief Print a malloc failure message and clean up shell state.
 * \param data The shell state to free.
 * \param str  The descriptive message suffix.
 */
void	failed_malloc(t_data *data, char *str);

/**
 * \brief Change the current working directory.
 * \param node  The AST node carrying the cd argument.
 * \param envp  The environment array.
 * \param data  The shell state (PWD and OLDPWD are updated).
 * \return 0 on success, 1 on error.
 */
int		ft_cd(t_ast *node, char **envp, t_data *data);

/**
 * \brief Print the current working directory to stdout.
 * \param data The shell state.
 * \return 0 on success, 1 on error.
 */
int		ft_pwd(t_data *data);

/**
 * \brief Print a generic "command: message" error to stderr.
 * \param str  The error message suffix.
 * \param file The command / file name.
 * \param type Reserved for future use.
 */
void	message_error(char	*str, char *file, int type);

/**
 * \brief Print a "No such file or directory" error for a command.
 * \param cmd  The command name.
 * \param file The target file name.
 */
void	no_such_file(char *cmd, char *file);

/**
 * \brief Print an export validation error message to stderr.
 * \param str The invalid export argument.
 */
void	export_error(char *str);

/**
 * \brief Signal handler: Ctrl-C at the interactive prompt.
 *
 * Prints a newline, resets the readline line, redisplays the prompt,
 * and sets the exit status to 130.
 * \param sig The signal number (unused).
 */
void	handle_sigstop(int sig);

/**
 * \brief Signal handler: Ctrl-C while waiting for a heredoc.
 *
 * Closes stdin so the heredoc read loop terminates, sets exit status
 * to 130, and marks the heredoc as interrupted.
 * \param sig The signal number (unused).
 */
void	handle_sigstop_heredoc(int sig);

/**
 * \brief Signal handler: Ctrl-C while waiting for a child process.
 *
 * Prints a newline and sets the exit status to 130.
 * \param sig The signal number (unused).
 */
void	handle_sigint_wait(int sig);

/**
 * \brief Get or set the heredoc-interrupted flag.
 * \param state  New flag value (used only when \c write_ is non-zero).
 * \param write_ Non-zero to update the flag; 0 to only read.
 * \return The current (or newly set) flag value.
 */
int		heredoc_status(int state, int write_);

/**
 * \brief Execute the right-hand side of `||` if the left side failed.
 * \param data  The shell state; \c tree is set to the OR node.
 * \param envp  The environment array.
 * \return The exit status of the branch that ran.
 */
int		execute_or(t_data	*data, char **envp);

/**
 * \brief Execute the right-hand side of `&&` only if the left side succeeded.
 * \param data  The shell state; \c tree is set to the AND node.
 * \param envp  The environment array.
 * \return The exit status of the last executed branch (0 on success).
 */
int		execute_and(t_data	*data, char **envp);

/**
 * \brief Wait for all children in a bonus (logical-operator) execution.
 * \param data The shell state.
 * \param fds  The fd table containing the PID array.
 * \return The exit status of the last waited child.
 */
int		wait_bonus(t_data *data, t_fds *fds);

/**
 * \brief Free the \c t_fds structure associated with a bonus execution step.
 * \param data The shell state whose \c fds member is freed and NULLed.
 */
void	free_fds_bonus(t_data	*data);
int		docs_bonus(t_ast *ast_root, t_fds **fds);

/**
 * \brief Count heredocs inside a bonus subtree and return the count.
 * \param ast_root Root of the subtree to inspect.
 * \param fds      Pointer to the fd table (updated with doc counts).
 * \return Number of heredocs found.
 */
int		docs_bonus(t_ast *ast_root, t_fds **fds);

/**
 * \brief Child process body for a subshell node.
 *
 * Applies external redirections, executes the subshell body tree,
 * and calls \c secure_exit() — never returns.
 * \param data The shell state.
 * \param envp The environment array.
 */
void	ft_child_sshell(t_data *data, char **envp);

/**
 * \brief Execute a NODE_PIPE node by forking two children.
 * \param data The shell state; \c tree must point to a NODE_PIPE node.
 */
void	ft_execute_pipe(t_data *data);

/**
 * \brief Advance fd-position counters past all nodes below the current one.
 * \param data The shell state (positional counters in \c fds are incremented).
 */
void	update_positions(t_data *data);

/**
 * \brief Recursively advance fd-position counters below a given AST node.
 * \param node The AST subtree root.
 * \param data The shell state.
 */
void	update_from_position(t_ast *node, t_data *data);

/**
 * \brief Execute a NODE_SUBSHELL node by forking a child process.
 * \param data The shell state; \c tree must point to a NODE_SUBSHELL node.
 * \param envp The environment array.
 */
void	ft_execute_sshell(t_data *data, char **envp);

/**
 * \brief Execute a NODE_CMD node, forking if not a built-in.
 * \param data The shell state; \c tree must point to a NODE_CMD node.
 */
void	ft_execute_cmd(t_data *data);

/**
 * \brief Top-level executor: pre-allocate fds, dispatch the AST, wait for children.
 * \param data The shell state with a fully-built AST.
 * \return 0 on success, -1 if the tree is empty.
 */
int		ft_execution(t_data *data);

/**
 * \brief Open a file redirection and store the fd in the pre-allocated table.
 * \param data     The shell state.
 * \param ast_root The current node being processed.
 * \param i        Index into \c fds->fd_files for this redirection.
 * \return 1 on success, 0 on failure (error printed to stderr).
 */
int		fill_fd_file(t_data *data, t_ast *ast_root, int i);

/**
 * \brief Exit the shell process, freeing all resources first.
 * \param data   The shell state to free.
 * \param status The exit code to pass to \c exit().
 */
void	secure_exit(t_data *data, int status);

/**
 * \brief Free the execution-side resources of the shell state (\c fds and \c envp).
 * \param data The shell state (modified: \c fds and \c envp are freed and NULLed).
 */
void	free_data(t_data *data);

/**
 * \brief Free a NULL-terminated 2D array (array of pointers to arrays).
 * \param ptr  The array to free.
 * \param rows Number of rows (0 means iterate until NULL sentinel).
 */
void	free_all(void **ptr, size_t rows);

/**
 * \brief Free a \c t_fds struct and all its sub-arrays.
 * \param fds Pointer to the pointer to free (set to NULL after).
 */
void	free_fds(t_fds **fds);

/**
 * \brief Recursively free an AST and NULL the pointer.
 * \param ast_root Pointer to the AST root pointer.
 */
void	free_tree(t_ast **ast_root);

/**
 * \brief Count the number of non-NULL elements in a pointer array.
 * \param ptr A NULL-terminated pointer array.
 * \return The element count (not including the NULL sentinel).
 */
int		ft_arraylen(void **ptr);

/**
 * \brief Fork a child and store its PID in the pre-allocated table.
 * \param pid Pointer to receive the fork result.
 * \param fds Pointer to the fd table (PID is stored at \c fds->c_pids[fork_id]).
 * \return Non-zero on success; 0 if fork failed.
 */
int		init_pid(pid_t *pid, t_fds **fds);

/**
 * \brief Extract the colon-separated PATH entries from an environment array.
 * \param env The environment array.
 * \return Heap-allocated NULL-terminated array of path strings, or NULL.
 */
char	**get_paths(char **env);

/**
 * \brief Traverse the AST and populate the resource-count fields in \c fds.
 * \param ast_root       The root of the AST to inspect.
 * \param fds            The fd table whose \c get struct is filled.
 * \param inside_sshell  Non-zero if called from within a subshell child.
 */
void	get_sizes(t_ast *ast_root, t_fds **fds, int inside_sshell);

/**
 * \brief Return the larger of two integers.
 * \param a First integer.
 * \param b Second integer.
 * \return The larger value.
 */
int		ft_max(int a, int b);

/**
 * \brief Close a file descriptor and set its slot to -1.
 * \param fd Pointer to the fd to close.
 */
void	secure_close(int *fd);

/**
 * \brief Reallocate an argument array, removing NULL entries from the middle.
 * \param args The original NULL-terminated argument array.
 * \return A compacted copy of \c args with internal NULLs removed.
 */
char	**ft_realloc_empty(char **args);

/**
 * \brief Read a heredoc from stdin until the delimiter is matched.
 * \param lim Delimiter string (unquoted).
 * \param fd  Output: write end stored at fd[1], read end at fd[0].
 * \return 0 on success, 1 if interrupted by SIGINT.
 */
int		here_doc(char *lim, int **fd);

/**
 * \brief Child process body for a NODE_CMD node.
 *
 * Applies redirections, resolves the command path, and calls execve().
 * Never returns on success; calls secure_exit() on failure.
 * \param data The shell state.
 * \param envp The environment array.
 */
void	ft_child_cmd(t_data	*data, char **envp);

/**
 * \brief Close all pre-opened file descriptors (both heredoc and file fds).
 * \param fds Pointer to the fd table.
 */
void	ft_closing_all(t_fds **fds);

/**
 * \brief Close only the non-heredoc file descriptor slots.
 * \param fds Pointer to the fd table.
 */
void	closing_files(t_fds **fds);

/**
 * \brief Count the number of command / subshell nodes in an AST subtree.
 * \param ast_root The root of the subtree.
 * \return The command count.
 */
int		number_of_cmds(t_ast *ast_root);

/**
 * \brief Pre-allocate the \c t_fds table for a standard (non-bonus) execution.
 * \param data The shell state (fds member is populated).
 */
void	ft_create_fds(t_data *data);

/**
 * \brief Pre-allocate the \c t_fds table for a bonus (logical-operator) step.
 * \param data The shell state (fds member is populated).
 */
void	ft_create_fds_bonus(t_data *data);

/**
 * \brief Dispatch execution for the current AST node in \c data->tree.
 * \param data  The shell state.
 * \param envp  The environment array.
 * \return 0 (return value is informational only).
 */
int		ft_exec_tree(t_data	*data, char **envp);

/**
 * \brief Initialise all heredoc pipes before any forking.
 * \param data The shell state (heredoc_fds in \c fds are populated).
 * \return 0 on success, 1 if a heredoc was interrupted by SIGINT.
 */
int		init_heredoc(t_data *data);

/**
 * \brief Apply external redirections attached to a NODE_SUBSHELL node.
 * \param data The shell state; \c tree must point to a NODE_SUBSHELL node.
 */
void	apply_redirs_subshell(t_data *data);

/**
 * \brief Built-in `echo`: print arguments to stdout (or a redirected fd).
 * \param data The shell state.
 * \return 0 on success, 1 on error.
 */
int		ft_echo(t_data *data);

/**
 * \brief Determine the target output fd for a built-in running in-process.
 *
 * Iterates the command's redirections and returns the last output fd,
 * or 1 (stdout) if no output redirection is present.
 * \param data The shell state.
 * \return The target fd, or -1 if a redirection file could not be opened.
 */
int		ft_target_fd(t_data *data);

/**
 * \brief Built-in `exit`: exit the shell with an optional numeric status.
 * \param data The shell state.
 * \return Does not return on success; returns 1 on "too many arguments".
 */
int		ft_exit(t_data *data);

/**
 * \brief Get, set, or act on the shell's last exit status.
 * \param state  New status value (used when \c write_ is non-zero).
 * \param write_ Non-zero to update the stored status.
 * \param exit_  Non-zero to call \c exit() with the current status.
 * \return The current (or newly set) exit status.
 */
int		ft_exit_status(int state, int write_, int exit_);
int		ft_error(int err_no, char *arg, char *err_msg);
void	ft_exit_print(void);
int		ft_safe_atoll(char *str, long long *out);

/**
 * \brief Print a formatted error message and return the given error code.
 * \param err_no  The error code to return.
 * \param arg     Optional argument name included in the message.
 * \param err_msg The human-readable error message.
 * \return \c err_no.
 */
int		ft_error(int err_no, char *arg, char *err_msg);

/**
 * \brief Print the "exit" message to stderr if the shell is interactive.
 */
void	ft_exit_print(void);

/**
 * \brief Safely convert a string to a long long, detecting overflow.
 * \param str The string to convert.
 * \param out Pointer to receive the result.
 * \return 1 on success, 0 if the string overflows or is not a valid integer.
 */
int		ft_safe_atoll(char *str, long long *out);

/**
 * \brief Built-in `unset`: remove one or more variables from the environment.
 * \param data The shell state.
 * \return 0 on success, 1 if any argument is invalid.
 */
int		ft_unset(t_data *data);
int		ft_is_valid_arguments(char *str);
int		is_key_match(char *env_var, char *key);
void	free_env_node(t_list *node);
void	remove_env_var(t_data *data, char *key);
void	refresh_env_array(t_data *data);

/**
 * \brief Test whether a string is a valid shell identifier (variable name).
 * \param str The string to validate.
 * \return 1 if valid, 0 otherwise.
 */
int		ft_is_valid_arguments(char *str);

/**
 * \brief Test whether an env-list key matches a given key string.
 * \param env_var The full "KEY=value" string from the env list.
 * \param key     The key to match against.
 * \return Non-zero if \c env_var's key portion equals \c key.
 */
int		is_key_match(char *env_var, char *key);

/**
 * \brief Free a single environment linked-list node and its content.
 * \param node The node to free.
 */
void	free_env_node(t_list *node);

/**
 * \brief Remove a variable from the environment linked list.
 * \param data The shell state.
 * \param key  The variable name to remove.
 */
void	remove_env_var(t_data *data, char *key);

/**
 * \brief Rebuild the flat \c envp array from the environment linked list.
 * \param data The shell state (envp is freed and rebuilt).
 */
void	refresh_env_array(t_data *data);

/**
 * \brief Recursive wildcard pattern matcher supporting `*`.
 * \param pattern The remaining pattern string.
 * \param str     The remaining input string.
 * \return 1 if the pattern matches \c str; 0 otherwise.
 */
int		ft_match_asterisk(char *pattern, char *str);

/**
 * \brief Match a glob pattern against a filesystem entry name.
 *
 * Refuses to match hidden files (names starting with `.`) unless the
 * pattern also starts with `.`.
 * \param pattern The glob pattern.
 * \param name    The filesystem entry name.
 * \return 1 if they match; 0 otherwise.
 */
int		ft_match_wildcard(char *pattern, char *name);

/**
 * \brief Collect all filesystem entries matching a glob pattern.
 * \param pattern The glob pattern (may include a directory prefix).
 * \return Linked list of heap-allocated matching path strings, or NULL.
 */
t_list	*get_wildcard_matches(char *pattern);

/**
 * \brief Test if a readline line contains only whitespace.
 * \param rl Pointer to the readline buffer (freed and NULLed if all spaces).
 * \return 1 if all whitespace, 0 otherwise.
 */
int		ft_verify_spaces(char **rl);

/**
 * \brief Free the lexer and the AST after each REPL iteration.
 * \param data The shell state (lexer and root AST are freed and NULLed).
 */
void	cleanup_loop(t_data *data);

/**
 * \brief Increment the SHLVL environment variable by one.
 * \param data The shell state.
 */
void	increase_shlv(t_data *data);

//--main shell
int		ft_verify_spaces(char **rl);
void	cleanup_loop(t_data *data);
void	increase_shlv(t_data *data);

#endif
