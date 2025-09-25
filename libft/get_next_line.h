/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 13:56:13 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/08/02 19:05:00 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100000
# endif

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>

int		ft_is_nl(char *buff);
char	*get_next_line(int fd);
int		ft_len2nl(const char *line);
void	ft_offset(char *buff);
int		ft_init(char **line, int *bytes_read, char *buff);
char	*ft_strconcat(char *s1, char *s2);

#endif
