# include "execution.h"


void	ft_pwd()
{
	char	buff[10000];

	if(getcwd(buff, 10000))
		printf("%s\n", buff);
}
