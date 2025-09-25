/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:03:19 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/07/01 09:10:22 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_putnbr(int args)
{
	int	tmp;

	tmp = 0;
	if (args == INT_MIN)
		return (write(1, "-2147483648", 11));
	if (args < 0)
	{
		tmp += ft_putletter('-');
		args *= -1;
	}
	if (args >= 10)
	{
		tmp += ft_putnbr(args / 10);
		tmp += ft_putletter(args % 10 + '0');
	}
	if (args <= 9)
		tmp += ft_putletter(args + '0');
	return (tmp);
}

static int	ft_puthexa(unsigned long args, int flag)
{
	int		i;
	char	*choose;

	if (flag)
		choose = "0123456789abcdef";
	else
		choose = "0123456789ABCDEF";
	i = 0;
	if (args > 15)
		i += ft_puthexa(args / 16, flag);
	i += ft_putletter(choose[args % 16]);
	return (i);
}

static int	ft_putaddr(void *args)
{
	unsigned long	addr;

	if (!args)
		return (write(1, "(nil)", 5));
	addr = (unsigned long) args;
	return (ft_putletter('0') + ft_putletter('x') + ft_puthexa(addr, 1));
}

static int	ft_specifiers(va_list args, const char **c)
{
	if (**c == 's')
		return (ft_putword(va_arg(args, char *)));
	else if (**c == 'd' || **c == 'i')
		return (ft_putnbr(va_arg(args, int)));
	else if (**c == 'c')
		return (ft_putletter(va_arg(args, int)));
	else if (**c == 'p')
		return (ft_putaddr(va_arg(args, void *)));
	else if (**c == 'x' )
		return (ft_puthexa(va_arg(args, unsigned int), 1));
	else if (**c == 'X')
		return (ft_puthexa(va_arg(args, unsigned int), 0));
	else if (**c == 'u')
		return (ft_putunsigned(va_arg(args, unsigned int)));
	else if (**c == '%')
		return (ft_putletter('%'));
	else
		return (0);
}

int	ft_printf(const char *fmt, ...)
{
	va_list	args;
	int		i;

	i = 0;
	va_start(args, fmt);
	while (fmt && *fmt)
	{
		if (*fmt == '%' && *(fmt + 1) != '\0')
		{
			fmt++;
			i += ft_specifiers(args, &fmt);
		}
		else
			i += ft_putletter(*fmt);
		fmt++;
	}
	va_end(args);
	return (i);
}
/*
int	main(void)
{
	//char buff[] = "print this";
	//printf("%s\n", buff);
	//ft_printf("%s\n", buff);
	//ft_putnbr(INT_MAX);
	//ft_putnbr(INT_MIN);
	//ft_puthexa(69,1);
	printf("%p\n", (void *)12389);
	ft_printf("%p", (void *)12389);
}
*/
/*
#include <stdio.h>
int	main(int argc, char **argv)
{
	int				i;
	int				a;
	int				*i_ptr;
	unsigned int	b;
	char			c;
	char			answer;
	char			*s_ptr;
	char			choice;

	i = 0;
	(void) argc;
	printf("type '8' when prompting the asked data type to break any loop\n");
	while (1)
	{
		a = 0;
		b = 0;
		c = 'c';
		i_ptr = &a;
		s_ptr = &c;
		answer = 'y';
		printf("choose a data type (c) (s) (p) (d) (i) (u) (x) (X) :\n");
		scanf(" %c", &choice);
		while (choice == 'c' && c != '8')
		{
			printf("char:\n");
			scanf(" %c", &c);
			printf(", %i\n", printf("%c", c));
			ft_printf(", %i\n", ft_printf("%c", c));
		}
		while (choice == 's' && answer == 'y')
		{
			if (i >= argc)
				i = 0;
			printf(", %i\n", printf("%s", argv[i]));
			ft_printf(", %i\n", ft_printf("%s", argv[i]));
			i++;
			printf("wanna check another word? (y) (n)\n");
			scanf(" %c", &answer);
		}
		while (choice == 'p' && c != '8' && a != 8)
		{
			printf("int, char:\n");
			scanf(" %i %c", &a, &c);
			printf(", %i\n", printf("%p, %p", i_ptr, s_ptr));
			ft_printf(", %i\n", ft_printf("%p, %p", i_ptr, s_ptr));
		}
		while ((choice == 'i' || choice == 'd') && a != 8)
		{
			printf("int:\n");
			scanf(" %i", &a);
			printf(", %i\n", printf("%i", a));
			ft_printf(", %i\n", ft_printf("%i", a));
		}
		while (choice == 'u' && b != 8)
		{
			printf("unsigned int:\n");
			scanf(" %u", &b);
			printf(", %i\n", printf("%u", b));
			ft_printf(", %i\n", ft_printf("%u", b));
		}
		while (choice == 'x' && a != 8)
		{
			printf("int:\n");
			scanf(" %i", &a);
			printf(", %i\n", printf("%x", a));
			ft_printf(", %i\n", ft_printf("%x", a));
		}
		while (choice == 'X' && a != 8)
		{
			printf("int:\n");
			scanf(" %i", &a);
			printf(", %i\n", printf("%X", a));
			ft_printf(", %i\n", ft_printf("%X", a));
		}
	}
}
*/
