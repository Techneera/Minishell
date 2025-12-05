#include "execution.h"

static void	bubble_sort(char **array, int size_array);
static int	ft_has_array_size(t_list *lst);
static void	print_export(char **array);

void	ft_print_sorted_export(t_list *list)
{
	char	**array;
	int		size_array;

	size_array = ft_has_array_size(list);
	array = ft_calloc(ft_has_array_size(list) + 1, sizeof(char *));
	if (!array)
	{
		perror("Failed calloc in fill_string");
		return ;
	}
	to_array_env(list, array);
	bubble_sort(array, size_array);
	print_export(array);
	free(array);
}

static void	print_export(char **array)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (array[i])
	{
		j = 0;
		printf("declare -x ");
		while (array[i][j] && array[i][j] != '=')
		{
			printf("%c", array[i][j]);
			j++;
		}
		if (array[i][j])
			printf("=\"%s\"", array[i] + j + 1);
		printf("\n");
		i++;
	}
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
