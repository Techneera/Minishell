#include "execution.h"

static void	bubble_sort(char **array, int size_array);
static int	ft_has_array_size(t_list *lst);
static int	print_export(char **array, t_data *data);

int	ft_print_sorted_export(t_list *list, t_data *data)
{
	char	**array;
	int		size_array;

	size_array = ft_has_array_size(list);
	array = ft_calloc(ft_has_array_size(list) + 1, sizeof(char *));
	if (!array)
	{
		perror("Failed calloc in fill_string");
		return (FAIL_STATUS);
	}
	to_array_env(list, array);
	bubble_sort(array, size_array);
	if (print_export(array, data))
		return (FAIL_STATUS);
	free(array);
	return (0);
}

static int	print_export(char **array, t_data *data)
{
	int	i;
	int	j;
	int	fd;

	fd = ft_target_fd(data);
	if (fd == -1)
		return (1);
	i = -1;
	while (array[++i])
	{
		j = -1;
		ft_putstr_fd("declare -x ", fd);
		while (array[i][++j] && array[i][j] != '=')
			ft_putchar_fd(array[i][j], fd);
		if (array[i][j])
		{
			ft_putstr_fd("=\"", fd);
			ft_putstr_fd(array[i] + j + 1, fd);
			ft_putstr_fd("\"", fd);
		}
		ft_putstr_fd("\n", fd);
	}
	return (0);
}

static void	bubble_sort(char **array, int size_array)
{
	int		i;
	char	*holder;

	while (size_array-- > 0)
	{
		i = 0;
		while (array[i + 1])
		{
			if (ft_strcmp(array[i], array[i + 1]) > 0)
			{
				holder = array[i];
				array[i] = array[i + 1];
				array[i + 1] = holder;
			}
			i++;
		}
	}
}

static int	ft_has_array_size(t_list *lst)
{
	int		i;
	t_env	*holder;

	i = 0;
	while (lst)
	{
		holder = (t_env *) lst->content;
		if (holder)
			i++;
		lst = lst->next;
	}
	return (i);
}
