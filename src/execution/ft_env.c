# include "execution.h"

int	ft_env(t_data *data)
{
	t_list	*list;
	t_env	*env;

	list = data->env_list;
	if (!list)
		return (FAIL_STATUS);
	while(list)
	{
		env = (t_env *) list->content;
		if (env->has_arg)
			printf("%s\n", env->variable);
		list = list->next;
	}
	return (0);
}
