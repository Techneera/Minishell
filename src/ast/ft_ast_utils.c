#include "libshell.h"
#include "ast.h"
#include "lexer.h"

t_redir	*ft_create_redir(t_label_redir label, char *str)
{
	t_redir	*r;

	r = (t_redir *)ft_calloc(1, sizeof(t_redir));
	if (!r)
		return (free(str), NULL);
	r->label = label;
	r->file_name = str;
	return (r);
}

void	ft_free_redir_content(void *content)
{
	t_redir	*redir;

	redir = (t_redir *)content;
	if (!redir)
		return ;
	free(redir->file_name);
	free(redir);
}

void	ft_copy_lst_to_array(t_list *head, t_redir *array)
{
	int		i;
	t_list	*tmp;
	t_redir	*redir_to_arr;

	i = 0;
	tmp = head;
	while (tmp)
	{
		redir_to_arr = (t_redir *)tmp->content;
		array[i].label = redir_to_arr->label;
		array[i].file_name = redir_to_arr->file_name;
		i++;
		tmp = tmp->next;
	}
}
