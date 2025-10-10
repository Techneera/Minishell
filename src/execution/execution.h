#ifndef EXECUTION_H
# define EXECUTION_H

#include "../../include/libshell.h"
#include "libft.h"


typedef struct s_fds
{
	int	**pipe_fds;
	int	*fd_files;
	int	file_id;
	int	n_files;
	int	n_pipes;
}	t_fds;

//---test_cmds
t_ast	*ft_cmd1();
t_ast	*ft_cmd2();
t_ast	*ft_cmd3();
t_ast	*ft_cmd4();
t_ast	*ft_cmd5();

//---init_files
void	number_of_redirs(t_fds **fds, t_ast *ast_root);
int	fill_fd_file(t_fds **fds, t_ast *ast_root, int i, char **env);

//---exec_utils
void	free_all(void **ptr, size_t rows);
void	free_tree(t_ast *ast_root);
int	ft_arraylen(void **ptr);

//---ft_getters
char	*get_command_path(char **arg, char **env);
char	**get_paths(char **env);
char	*get_shell(char **env);
void	message_error(char	*str, char *file, char **env, int type);
#endif