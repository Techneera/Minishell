// #include "execution.h"

// void	init_env(t_data *data, char **env)
// {
// 	int		i;
// 	int		size;
// 	char	**new_env;

// 	i = 0;
// 	size = ft_arraylen((void **) env);
// 	new_env = ft_calloc(size + 1, sizeof(char *));
// 	if (!new_env)
// 		failed_malloc(data, "Calloc Error: new_env");
// 	while (env[i])
// 	{
// 		new_env[i] = ft_strdup(env[i]);
// 		if (!new_env[i])
// 		{
// 			ft_free_array(new_env);
// 			failed_malloc(data, "Strdup Error: new_env[i]");
// 		}
// 		i++;
// 	}
// 	data->envp = new_env;
// }

// void	increment_shlv(t_data *data)
// {
	
// }