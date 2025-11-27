#include "execution.h"

void	ft_export(t_list *list)
{
	if (!list)
		return ;
	ft_print_sorted_export(list);
}
