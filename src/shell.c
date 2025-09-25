#include "libshell.h"

void	main_loop(char ***args);

int	main(int argc, char **argv, char **envp)
{
	char	**args;

	args = NULL;
	main_loop(&args);
	(void)argc;
	(void)argv;
	(void)envp;

}

void	free_all_char(char ***str)
{
	int	i;

	i = 0;
	if (!*str)
		return ;
	while (*(str[i]))
	{
		free(*(str[i]));
		*(str[i]) = NULL;
		i++;
	}
	free(**str);
	**str = NULL;
}

int	number_of_args(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return (0);
	while (args[++i])
		;
	return (i);
}

char	**ft_argvdup(char **argv)
{
	char	**new_argv;
	int		n_args;
	int		i;

	i = 0;
	n_args = number_of_args(argv);
	new_argv = ft_calloc(n_args, sizeof(char *));
	if (!new_argv)
		return (NULL);
	while (n_args-- > 0)
	{
		new_argv[i] = ft_strdup(argv[i]);
		if (!new_argv[i])
		{
			free_all_char(&new_argv);
			return (NULL);
		}
		i++;
	}
	return (new_argv);
}

char	**ft_realloc(char ***argv, char *str)
{
	int		i;
	int		n_args;
	char	**tmp_argv;

	i = 0;
	n_args = number_of_args(*argv);
	if (!str)
		return (NULL);
	if (!(*argv))
	{
		tmp_argv = ft_calloc(2, sizeof(char *));
		if (!tmp_argv)
			return (NULL);
		tmp_argv[0] = ft_strdup(str);
		if(!tmp_argv[0])
			return (free_all_char(argv), free_all_char(&tmp_argv), NULL);
		tmp_argv[1] = NULL;
		return (tmp_argv);
	}
	tmp_argv = ft_calloc(n_args + 2, sizeof(char *));
	if (!tmp_argv)
		return (free_all_char(argv), free_all_char(&tmp_argv), NULL);
	while (n_args-- > 0)
	{
		tmp_argv[i] = ft_strdup(*(argv[i]));
		if (!tmp_argv[i++])
			return (free_all_char(argv), free_all_char(&tmp_argv), NULL);
	}
	tmp_argv[i] = ft_strdup(str);
	if (!tmp_argv[i])
		return (free_all_char(argv), free_all_char(&tmp_argv), NULL);
	tmp_argv[i + 1] = NULL;
	free_all_char(argv);
	return (tmp_argv);
}

void	print_all_str(char **str)
{
	int	i;

	i = -1;	
	if (!str)
		return ;
	while (str[++i])
		printf("%s\n", str[i]);
}

void	main_loop(char ***args)
{
	char	*token;
	char	*line;

	line = NULL;
	token = NULL;
	while(1)
	{
		line = readline(PROMPT);
		if (!line)
		{
			printf("exit\n");
			break ;
		}
		token = ft_strtok(line, " ");
		*args = ft_realloc(args, ft_strdup(token));
		if (!args)
			exit(1);
		while (token)
		{
			token = ft_strtok(NULL, " ");
			*args = ft_realloc(args, ft_strdup(token));
			if (!args)
				exit(1);
		}
		printf("\n");
		print_all_str(*args);
		free_all_char(args);
		free(line);
		line = NULL;
	}
	if (line)
		free(line);
}
