#ifndef EXECUTION_H
# define EXECUTION_H

# define CMD_NOT_FOUND 127
# define FAIL_STATUS 1

#include <signal.h>
#include <sys/stat.h>
#include <dirent.h>
#include <limits.h>
#include "libshell.h"
#include "ast.h"
#include "lexer.h"
#include "expansion.h"

typedef struct s_env
{
	int		has_arg;
	char	*variable;
}	t_env;

typedef struct s_get
{
	int	n_files;
	int	n_cmds;
	int	n_docs;
}	t_get;

typedef struct s_pos
{
	int	file_id;
	int	doc_id;
	int	fork_id;
}	t_pos;

typedef struct s_fds
{
	int		**heredoc_fds;
	int		*fd_files;
	int		*c_pids;
	t_get	get;
	t_pos	pos;
}	t_fds;

typedef struct s_data
{
	t_ast	*root;
	t_ast	*tree;
	t_fds	*fds;
	char	**envp;
	t_list	*env_list;
	char	*rl;
	t_lexer *lexer;
}	t_data;

//---ft_get_command_path.c
char	*get_command_path(char **arg, char **env, t_data *data);

//---wildcards_utils.c
char	**insert_wildcard_args(char **old_args, t_list *files, int index);
void	unmask_wildcards(char *str);

//---ft_is_builtin
int	ft_is_builtin(t_data *data, char *arg);

//---ft_getenv.c
char	*ft_getenv(char **env, char *arg);

//---list_utils.c
int	exist_in_list(t_env *env, char *arg);

//---ft_env
int	ft_env(t_data *data);

//---ft_export
int		ft_export(t_list *list, char **args, t_data *data);
void	ft_print_sorted_export(t_list *list);

//---env_utils
t_list	*init_env(char **env);
t_list	*create_node_env(char *arg, int has_arg);
void	to_array_env(t_list *head, char **array);
char	**envlist_to_array(t_list *list);
void	ft_free_content(void *content);

//---error_handle
void	failed_malloc(t_data *data, char *str);

//---ft_cd
int		ft_cd(t_ast *node, char **envp, t_data *data);
int		ft_pwd(t_data *data);

//---errors_messages
void	message_error(char	*str, char *file, int type);
void	no_such_file(char *cmd, char *file);
void	export_error(char *str);

//---handle_signal
void    handle_sigstop(int sig);
void    handle_sigstop_heredoc(int sig);
void	handle_sigint_wait(int sig);

//---ft_execute_or
int		execute_or(t_data	*data, char **envp);

//---ft_execute_and
int		execute_and(t_data	*data, char **envp);

//---utils_bonus
int		wait_bonus(t_data *data, t_fds *fds);
void	free_fds_bonus(t_data	*data);
int		docs_bonus(t_ast *ast_root, t_fds **fds);

//--ft_child_sshel
void	ft_child_sshell(t_data *data, char **envp);

//---ft_execute_pipe
void ft_execute_pipe(t_data *data);

//---ft_update_position
void	update_positions(t_data *data);
void	update_from_position(t_ast *node, t_data *data);

//---ft_execute_sshell
void	ft_execute_sshell(t_data *data, char **envp);

//---ft_execute_cmd
void	ft_execute_cmd(t_data *data);

int		ft_execution(t_data *data);

//---init_files
int		fill_fd_file(t_data *data, t_ast *ast_root, int i);

//---exec_utils
void	secure_exit(t_data *data, int status);
void 	free_data(t_data *data);
void	free_all(void **ptr, size_t rows);
void	free_fds(t_fds **fds);
void	free_tree(t_ast **ast_root);
int		ft_arraylen(void **ptr);
int		init_pid(pid_t *pid, t_fds **fds);

//---ft_getters
char	**get_paths(char **env);
void	message_error(char	*str, char *file, int type);
void	get_sizes(t_ast *ast_root, t_fds **fds, int inside_sshell);

//---add_libft
int		ft_max(int a, int b);
void	secure_close(int *fd);
char	**ft_realloc_empty(char **args);

//---ft_here_doc
int	here_doc(char *lim, int **fd);

//---ft_child_cmd
void	ft_child_cmd(t_data	*data, char **envp);

//--ft_closing_all
void	ft_closing_all(t_fds **fds);
void	closing_files(t_fds **fds);

//		ft_execution
int		number_of_cmds(t_ast *ast_root);

//---ft_create_fds
void	ft_create_fds(t_data *data);

//---ft_create_fds_bonus
void	ft_create_fds_bonus(t_data *data);

int		ft_exec_tree(t_data	*data, char **envp);
void	ft_closing_all(t_fds **fds);
int		init_heredoc(t_data *data);

//--exec_tree_bonus
void	apply_redirs_subshell(t_data *data);

//--ft_echo
int		ft_echo(t_data *data);
int		ft_target_fd(t_data *data);

//--ft_exit_status
int		ft_exit(t_data *data);
int		ft_exit_status(int state, int write_, int exit_);

//--ft_unset
int		ft_unset(t_data *data);

//--WILDCARD
int		ft_match_asterisk(char *pattern, char *str);
int		ft_match_wildcard(char *pattern, char *name);
t_list	*get_wildcard_matches(char *pattern);
char	**insert_wildcard_args(char **old_args, t_list *files, int index);
void	unmask_wildcards(char *str);

#endif
