#ifndef EXECUTION_H
# define EXECUTION_H

#include "../../include/libshell.h"
#include "libft.h"

typedef struct s_fds
{
	int	**pipe_fds;
	int	**heredoc_fds;
	int	*fd_files;
	int	file_id;
	int	doc_id;
	int	n_files;
	int	n_pipes;
	int	n_docs;
}	t_fds;

//---test_cmds
t_ast	*ft_cmd1();
t_ast	*ft_cmd2();
t_ast	*ft_cmd3();
t_ast	*ft_cmd4();
t_ast	*ft_cmd5();
t_ast	*ft_cmd6();
t_ast	*ft_cmd7();
t_ast	*ft_cmd8();
t_ast	*ft_cmd9();
t_ast	*ft_bash();


//---init_files
void	number_of_redirs(t_fds **fds, t_ast *ast_root);
int		fill_fd_file(t_fds **fds, t_ast *ast_root, int i);

//---exec_utils
void	free_all(void **ptr, size_t rows);
void	free_tree(t_ast *ast_root);
int		ft_arraylen(void **ptr);

//---ft_getters
char	*get_command_path(char **arg, char **env);
char	**get_paths(char **env);
void	message_error(char	*str, char *file, int type);

//---add_libft
int	ft_max(int a, int b);

//---ft_here_doc
void	here_doc(char *lim, int *fd);
#endif
