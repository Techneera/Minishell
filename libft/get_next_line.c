/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 21:22:08 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/07/11 15:41:34 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	ft_clean_zero(char *buff)
{
	int	i;

	i = 0;
	while (buff[i])
	{
		buff[i] = '\0';
		i++;
	}
}

static void	ft_verify(int br, char *buff)
{
	int	i;

	i = 0;
	if (br <= BUFFER_SIZE)
	{
		while (i < br)
			i++;
		while (buff[i])
		{
			buff[i] = '\0';
			i++;
		}
	}
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1];
	char		*line;
	int			bytes_read;

	if (fd < 0 || BUFFER_SIZE <= 0 || !ft_init(&line, &bytes_read, buffer))
		return (NULL);
	while (bytes_read && !ft_is_nl(&buffer[0]))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1 || (bytes_read == 0 && !*line))
		{
			if (bytes_read == -1)
				ft_clean_zero(buffer);
			return (free(line), NULL);
		}
		if (bytes_read != 0)
		{
			ft_verify(bytes_read, buffer);
			line = ft_strconcat(line, buffer);
			if (!line)
				return (free(line), NULL);
		}
	}
	ft_offset(buffer);
	return (line);
}
