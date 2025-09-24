/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 17:01:05 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/09/24 16:07:42 by nsaraiva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libshell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*line;

	(void)argc;
	(void)argv;
	(void)envp;
	while(1)
	{
		line = readline(PROMPT);
		if (!line)
			break ;
		printf("%s", line);
		free(line);
		line = NULL;
	}
	if (line)
		free(line);
}
