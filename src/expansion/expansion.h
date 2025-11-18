#include "libshell.h"

typedef	enum	s_exp_state
{
	EXP_GENERAL,
	EXP_SQUOTE,
	EXP_DQUOTE
}	t_exp_state;

char	*expand_word(char *raw_str, char **envp);
char	*ft_get_expanded_value(char *s, char **envp, int *i);
char	*ft_strjoin_free_s1(char *s1, char *s2);
