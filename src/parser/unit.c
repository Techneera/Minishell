#include "libshell.h"
#include "lexer.h"
#include <stdio.h>

int	main(void)
{
    char	*strs[] = {"ls -la", "echo \'hello\'", NULL};
    int		i;
    t_lexer	*l;

    i = 0;
    l = NULL;
    while (strs[i])
    {
	l = ft_state_lexer(ft_strdup(strs[i]));
	if (!l)
	    return (-1);
	printf("%s\n", l->input);
	printf("%ld\n", l->pos);
	printf("%ld\n", l->len);
	free(l->input);
	free(l);
	i++;
    }
    return (0);
}
