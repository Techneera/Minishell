# include "execution.h"

int	ft_pwd()
{
	char	buff[10000];

	if(getcwd(buff, 10000))
	{
		printf("%s\n", buff);
		return (0);
	}
	return (FAIL_STATUS);
}
