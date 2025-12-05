#include "expansion.h"
#include "libft.h"
#include "execution.h"

void	mask_wildcards(char *str)
{
	int	i;

	if (!str)
		return ;
	i = 0;
	while (str[i])
	{
		if (str[i] == '*')
			str[i] = '\001';
		i++;
	}
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
	if (s[1] == '?')
	{
		*i += 2;
		return (ft_itoa(status));
	}
	if (ft_isdigit(s[1]))
	{
		*i += 2;
		return (ft_strdup(""));
	}
	return (ft_expand_env_var_aux(s, envp, i));
}

char	*expand_word(char *raw_str, char **envp, int status)
{
	t_exp_ctx	ctx;

	ft_init_exp_ctx(&ctx, raw_str, envp, status);
	while (ctx.raw[ctx.i])
	{
		if (ctx.state == EXP_GENERAL)
			ft_handle_general(&ctx);
		else if (ctx.state == EXP_SQUOTE)
			ft_handle_squote(&ctx);
		else if (ctx.state == EXP_DQUOTE)
			ft_handle_dquote(&ctx);
	}
	if (ctx.raw)
		free(ctx.raw);
	return (ctx.final);
}

void	ft_init_exp_ctx(t_exp_ctx *ctx, char *raw, char **env, int st)
{
	ctx->raw = raw;
	ctx->envp = env;
	ctx->status = st;
	ctx->final = ft_strdup("");
	ctx->i = 0;
	ctx->start = 0;
	ctx->state = EXP_GENERAL;
}
