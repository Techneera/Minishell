#include "execution.h"

static void	to_array(t_list *head, char **array);
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
	to_array(list, array);
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
		while (array[i][j] != '=')
		{
			printf("%c", array[i][j]);
			j++;
		}
		printf("=\"%s\"\n", array[i] + j + 1);
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
		if (holder && holder->has_arg)
			i++;
		lst = lst->next;
	}
	return (i);
}

static void	to_array(t_list *head, char **array)
{
	int		i;
	t_list	*tmp;
	t_env	*redir_to_arr;

	i = 0;
	tmp = head;
	while (tmp)
	{
		redir_to_arr = (t_env *)tmp->content;
		if (redir_to_arr->has_arg)
		{
			array[i] = redir_to_arr->variable;
			i++;
		}
		tmp = tmp->next;
	}
}
