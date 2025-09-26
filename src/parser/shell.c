#include "libshell.h"

void	main_loop(t_token **token);
void	fill_token(t_token **token, char *line);

int	main(int argc, char **argv, char **envp)
{
	t_token	*token;

	token = NULL;
	main_loop(&token);
	(void)argc;
	(void)argv;
	(void)envp;
}

void	main_loop(t_token **token)
{
	char	*line;

	line = NULL;
	while(1)
	{
		line = readline(PROMPT);
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		fill_token(token, line);
		token_print(*token);
		ft_free_token(token);
		free(line);
		line = NULL;
	}
	if (*token)
		ft_free_token(token);
	if (line)
		free(line);
}

void	fill_token(t_token **token, char *line)
{
	char	*str_pointer;

	str_pointer = ft_strtok(line);
	if (token_add_back(token, str_pointer) == -1)
	{
		ft_free_token(token);
		exit(1);
	}
	while (str_pointer)
	{
		str_pointer = ft_strtok(NULL);
		if (str_pointer && token_add_back(token, str_pointer) == -1)
		{
			ft_free_token(token);
			exit(1);
		}
	}
}
