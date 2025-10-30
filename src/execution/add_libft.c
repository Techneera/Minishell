#include "execution.h"

int	ft_max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

void	free_all(void **ptr, size_t rows)
{
	size_t	i;

	i = 0;
	if (!ptr)
		return ;
	while (i < rows)
	{
		if (ptr[i])
		{
			free(ptr[i]);
			ptr[i] = NULL;
		}
		i++;
	}
	free(ptr);
	ptr = NULL;
}

int	ft_arraylen(void **ptr)
{
	int	i;

	i = 0;
	if (!ptr)
		return (i);
	while (ptr[i])
		i++;
	return (i);
}
