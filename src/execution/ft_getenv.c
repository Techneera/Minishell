/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "execution.h"

static char	*ft_env_value(char *env);

char	*ft_getenv(char **env, char *arg)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!env)
		return (NULL);
	if (!arg)
		return (NULL);
	while (env[i])
	{
		while (env[i][j] && arg[j] && env[i][j] == arg[j]
				&& env[i][j] != '=')
			j++;
		if ((env[i][j] == arg[j]) || (
			!arg[j] && env[i][j] == '='))
			return (ft_env_value(env[i]));
		i++;
	}
	return (NULL);
}

static char	*ft_env_value(char *env)
{
	int	i;

	i = 0;
	if (!env)
		return (NULL);
	while (env[i] && env[i] != '=')
		i++;
	if (env[i] == '=')
		i++;
	return (env + i);
}
