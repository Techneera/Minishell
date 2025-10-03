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
}	t_fds;

//---test_cmds
t_ast	*ft_cmd1();
t_ast	*ft_cmd2();
t_ast	*ft_cmd3();
t_ast	*ft_cmd4();
t_ast	*ft_cmd5();

//---init_files
void	number_of_redirs(t_fds **fds, t_ast *ast_root);
#endif