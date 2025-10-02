#ifndef EXECUTION_H
# define EXECUTION_H

#include "../../include/libshell.h"
#include "libft.h"

t_ast	*ft_cmd1();
t_ast	*ft_cmd2();
t_ast	*ft_cmd3();

typedef struct s_fds
{
	int	*pipe_fds;
	int	*fd_files;
	int	file_id;
}	t_fds;

#endif