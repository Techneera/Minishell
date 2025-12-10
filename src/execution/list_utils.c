/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "execution.h"

int	exist_in_list(t_env *env, char *arg)
{
	int		i;

	i = 0;
	while (env->variable[i] && arg[i] && env->variable[i] == arg[i]
		&& env->variable[i] != '=' && arg[i] != '=')
		i++;
	if ((env->variable[i] == arg[i]) || (!env->variable[i] && arg[i] == '='
		) || (!arg[i] && env->variable[i] == '='))
		return (1);
	return (0);
}
