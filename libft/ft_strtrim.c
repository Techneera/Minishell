/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 18:59:03 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/04/11 19:16:39 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_found(char c, char const *set)
{
	while (*set)
	{
		if (*set == c)
			return (TRUE);
		set++;
	}
	return (FALSE);
}

static char	*ft_search(char const *s1, char const *set)
{
	while (*s1)
	{
		if (!ft_found(*s1, set))
			break ;
		s1++;
	}
	return ((char *)s1);
}

static char	*ft_rsearch(char const *s1, char const *set, size_t len)
{
	unsigned char	*end_s1;

	end_s1 = (unsigned char *) s1 + len - 1;
	while (len--)
	{
		if (!ft_found(*end_s1, set))
			return ((char *)end_s1 + 1);
		end_s1--;
	}
	return ((char *) end_s1);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*ptr;
	char	*start;
	int		diff;
	int		i;

	start = ft_search(s1, set);
	diff = ft_rsearch(s1, set, ft_strlen(s1)) - start ;
	if (diff < 0)
		return (ft_strdup(""));
	ptr = (char *) malloc(sizeof(char) * (diff + 1));
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < diff)
	{
		ptr[i] = start[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}
/*
#include <stdio.h>

int main()
{

	char	s1[] = "   \t\nTrim this\nDUCKER \t\n\t\n   ";
	char	s2[] = " \t\n";
	//char	res[] = "Trim this\nDUCKER";

//	char	s1[] = "1234Hello4231";
//	char	s2[] = "3124";
//	char	res[] = "Hello";
	char	*s3;

	s3 =  ft_strtrim(s1, s2);
	printf("%s\n", s3);
	free(s3);
}
char	*ft_strtrim(char const *s1, char const *set)
{
	char	*start;
	char	*end;
	char	*ptr;
	char	*ptr_start;
	size_t	n;

	start = ft_search(s1, set, ft_strchr, 1);
	end = ft_search(s1, set, ft_strrchr, 0);
	n = start - end;
	ptr = malloc(n);
	if(!ptr)
		return (NULL);
	ptr_start = ptr;
	while (n--)
	{
		*ptr = *start;
		ptr++;
		start++;
	}
	return (ptr_start);
}


static size_t	ft_lenset(char const *s1, char const *set)
{
	char	*ptr_start;
	char	*ptr_set;
	unsigned int	i;
	unsigned int	j;
	
	i = 0;
	j = 0;
	ptr_start = (char *) s1;
	ptr_set = (char *) set;
	while (*ptr_start)
	{
		ptr_set = (char *) set;
		while (*ptr_set)
		{
			if (*ptr_start == *ptr_set)
				i++;
			ptr_set++;
		}
		ptr_start++;
		j++;
	}
	return (j - i + 1);
}

static size_t	ft_setcmp(const char *s1, const char *set)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (s1[i])
	{
		j = 0;
		while (set[j])
		{
			if (s1[i] == set[j])
				return (FALSE);
			j++;
		}
		i++;
	}
	return (TRUE);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*ptr_malloc;
	char	*ptr_init;
	
	if (s1 == NULL)
	{
		ptr_malloc = malloc(1);
		if (!ptr_malloc)
			return (NULL);
		return (ptr_malloc);
	}
	if (set == NULL)
		return (ft_strdup(s1));
	ptr_malloc = (char *) malloc(sizeof(char) * ft_lenset(s1, set));
	if (!ptr_malloc)
		return (NULL);
	ptr_init = ptr_malloc;
	while (*s1)
	{
		if (ft_setcmp(s1,set))
		{
			*ptr_malloc = *s1;
			ptr_malloc++;
		}
		s1++;
	}
	*ptr_malloc = '\0';
	return (ptr_init);
}

#include <stdio.h>
int	main()
{
	char	s1[] = "   \t \n\n \t\t \n\nHello \t   Please\n Trim me!";
	char	s2[] = "HelloPleaseTrimme!";
	char	*ret = ft_strtrim(s1, " \n\t");
	if (ft_strcmp(s1,s2))
		printf("%s\tOK", ret);
	else
		printf("ERROR");
}
*/
