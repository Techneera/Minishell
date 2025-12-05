#include "execution.h"

char	**insert_wildcard_args(char **old_args, t_list *files, int index)
{
	char	**new_args;
	int		old_len;
	int		files_len;
	int		j;
	int		k;

	old_len = 0;
	while (old_args[old_len])
		old_len++;
	files_len = ft_lstsize(files);
	new_args = malloc(sizeof(char *) * (old_len + files_len + 1));
	if (!new_args)
		return (NULL);
	j = 0;
	while (j < index)
	{
		new_args[j] = old_args[j];
		j++;
	}
	while (files)
	{
		new_args[j++] = ft_strdup(files->content);
		files = files->next;
	}
	k = index + 1;
	while (old_args[k])
		new_args[j++] = old_args[k++];
	new_args[j] = NULL;
	free(old_args[index]); 
	free(old_args);
	return (new_args);
}

/* Helper to restore masked chars if no expansion occurred */
void	unmask_wildcards(char *str)
{
	int	i = 0;
	while (str[i])
	{
		if (str[i] == '\001')
			str[i] = '*';
		i++;
	}
}