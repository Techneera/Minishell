#ifndef EXECUTION_H
# define EXECUTION_H

# define CMD_NOT_FOUND 127
# define FAIL_STATUS 1

#include "libshell.h"

extern int child_status;

typedef struct s_get
{
	int	n_files;
	int	n_pipes;
	int	n_cmds;
	int	n_docs;
}	t_get;

typedef struct s_pos
{
	int	file_id;
	int	doc_id;
}	t_pos;

typedef struct s_fds
{
	int		**pipe_fds;
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
}	t_data;


//---test_cmds
t_ast	*ft_cmd1();
t_ast	*ft_cmd2();
t_ast	*bonus_cmd();


//---init_files
int		fill_fd_file(t_fds **fds, t_ast *ast_root, int i);

//---exec_utils
void	 secure_exit(t_data *data, int status);
void	free_all(void **ptr, size_t rows);
void	free_fds(t_fds **fds);
void	free_tree(t_ast **ast_root);
int		ft_arraylen(void **ptr);
int		init_pid(pid_t *pid, t_fds **fds);
void	get_sizes(t_ast *ast_root, t_fds **fds);

//---ft_getters
char	*get_command_path(char **arg, char **env);
char	**get_paths(char **env);
void	message_error(char	*str, char *file, int type);

//---add_libft
int	ft_max(int a, int b);

//---ft_here_doc
int	here_doc(char *lim, int *fd);

//---ft_child_process
void	ft_child_process(t_data	*data, int i, char **envp);
void	apply_std_dup(t_data *data, int i);
void	apply_redirs_dup(t_data *data, t_ast **node);
void	heredoc_dup(t_ast **node, t_fds **fds, int r);

//--ft_closing_all
void	ft_closing_all(t_fds **fds);

//		ft_execution
int		number_of_cmds(t_ast *ast_root);
void	ft_create_fds(t_data *data);
int		ft_exec_tree(t_data	*data, int i, char **envp);
void	ft_closing_all(t_fds **fds);
void	init_heredoc(t_data *data);

//--exec_tree_bonus
void	apply_redirs_subshell(t_data *data);


#endif
