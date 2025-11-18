#include "expansion.h"

char	*expand_word(char *raw_str, char **envp, int status)
{
	char		*final_str;
	t_exp_state	state;
	int			i;
	int			start;
	char		*chunk;
	char		tmp[2];

	final_str = ft_strdup("");
	i = 0;
	start = 0;
	state = EXP_GENERAL;
	while (raw_str[i])
	{
		if (state == EXP_GENERAL)
		{
			if (raw_str[i] == '\'')
			{
				state = EXP_SQUOTE;
				start = i + 1;
			}
			else if (raw_str[i] == '"')
			{
				state = EXP_DQUOTE;
				start = i + 1;
			}
			else if (raw_str == "$")
			{
				chunk = ft_get_expanded_value(&raw_str[i], envp, &i, status);
				final_str = ft_strjoin_free_s1(final_str, chunk);
				start = i;
				continue ;
			}
			else
			{
				tmp[0] = raw_str[i];
				tmp[1] = 0;
				final_str = ft_strjoin_free_s1(final_str, tmp);
			}
		}
		else if (state == EXP_SQUOTE)
		{
			if (raw_str[i] == '\'')
			{
				state = EXP_GENERAL;
				chunck = ft_substr(raw_str, start, i - start);
				final_str = ft_strjoin_free_s1(final_str, chunck);
				free(chunck);
				continue ;
			}
		}
		else if (state == EXP_DQUOTE)
		{
			if (raw_str[i] == '"')
			{
				state = EXP_GENERAL;
				chunk = ft_substr(raw_str, start, i - start);
				final_str = ft_strjoin_free_s1(final_str, chunk);
				free(chunk);
			}
			else if (raw_str[i] == '$')
			{
				chunk = ft_substr(raw_str, start, i - start);
				final_str = ft_strjoin_free_s1(final_str, chunk);
				free(chunk);
				chunk = ft_get_expanded_value(&raw_str[i], envp, &i, status);
				final_str = ft_strjoin_free_s1(final_str, chunk);
				start = i;
				continue;
			}
		}
		i++;
	}
	return (final_str);
}

char	*ft_strjoin_free_s1(char *s1, char *s2)
{
	char	*new_str;
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (s1);
	new_str = ft_strjoin(s1, s2);
	free(s1);
	return (new_str);
}

char	*ft_get_expanded_value(char *s, char **envp, int *i, int status)
{
	int		len;
	char	*var_name;
	char	*var_value;

	if (str[1] == '?')
	{
		*i += 2;
		return (ft_itoa(data->status)); 
	}
	len = 1;
	while (ft_isalnum(str[len]) || str[len] == '_')
		len++;
	if (len == 1)
	{
		*i += 1;
		return (ft_strdup("$"));
	}
	var_name = ft_substr(str, 1, len - 1);
	var_value = ft_getenv(var_name, envp);
	free(var_name);

	*i += len;

	if (!var_value)
		return (ft_strdup(""));
	return (ft_strdup(var_value));
}
