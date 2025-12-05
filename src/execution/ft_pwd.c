#include "execution.h"

int	ft_pwd(t_data *data)
{
	char	*path;

	path = ft_getenv(data->envp, "PWD");
	if (path)
	{
		printf("%s\n", ft_getenv(data->envp, "PWD"));
		return (0);
	}
	return (FAIL_STATUS);
}
