/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rluis-ya <rluis-ya@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 16:41:45 by rluis-ya          #+#    #+#             */
/*   Updated: 2025/12/06 16:41:45 by rluis-ya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EXPANSION_H
# define EXPANSION_H

# include "libshell.h"

typedef enum s_exp_state
{
	EXP_GENERAL,
	EXP_SQUOTE,
	EXP_DQUOTE
}	t_exp_state;

typedef struct s_exp_ctx
{
	char		*final;
	char		*raw;
	char		**envp;
	int			status;
	int			i;
	int			start;
	t_exp_state	state;
}	t_exp_ctx;

char	*expand_word(char *raw_str, char **envp, int status);
char	*ft_get_expanded_value(char *s, char **envp, int *i, int status);
char	*ft_strjoin_free_s1(char *s1, char *s2);
void	mask_wildcards(char *str);
void	ft_init_exp_ctx(t_exp_ctx *ctx, char *raw, char **env, int st);
char	*ft_expand_env_var_aux(char *s, char **envp, int *i);
void	ft_handle_general(t_exp_ctx *ctx);
void	ft_handle_squote(t_exp_ctx *ctx);
void	ft_handle_dquote(t_exp_ctx *ctx);
void	ft_general_dollar_aux(t_exp_ctx *ctx);
void	ft_general_char_aux(t_exp_ctx *ctx);
#endif
