/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 19:40:33 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/08/12 11:45:22 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LIBFT_H
# define LIBFT_H

//--------------------------INCLUDES--------------------------------------
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include "get_next_line.h"
# include "ft_printf.h"
//--------------------------STRUCTS---------------------------------------
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;
//--------------------------DEFINES---------------------------------------
# define TRUE 1
# define FALSE 0

int		ft_atoi(const char *nptr);
char	*ft_itoa(int n);
//--------------------------STRING MANIPULATION---------------------------
size_t	ft_strlen(const char *str);
void	ft_putstr(char *str);
void	ft_putchar(int c);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, unsigned int n);
int		ft_strlcat(char *dst, const char *src, unsigned int size);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
char	*ft_strnstr(const char *big, const char *little, unsigned int len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strdup(const char *s);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char *));
void	ft_putchar_fd(char c, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
char	**ft_split(char const *s, char c);
//--------------------------CHARACTER VERIFICATION------------------------
int		ft_tolower(int c);
int		ft_toupper(int c);
int		ft_isalpha(int c);
int		ft_isprint(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isdigit(int c);
int		ft_isspace(int c);
//--------------------------MEMORY MANIPULATION---------------------------
void	*ft_memcpy(void *dest, const void *src, unsigned int n);
int		ft_memcmp(const void *s1, const void *s2, unsigned int n);
void	*ft_memmove(void *dest, const void *src, unsigned int n);
void	*ft_memchr(const void *s, int c, unsigned int n);
void	*ft_memccpy(void *dest, const void *src, int c, unsigned int n);
void	ft_bzero(void *s, unsigned int n);
void	*ft_memset(void *s, int c, unsigned int n);
void	*ft_calloc(unsigned int nmemb, unsigned int size);
//--------------------------LIST MANIPULATION-----------------------------
t_list	*ft_lstnew(void *content);
int		ft_lstsize(t_list *lst);
void	ft_lstadd_front(t_list **lst, t_list *new);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstdelone(t_list *lst, void (*del)(void*));
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
#endif
