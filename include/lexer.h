#ifndef LEXER_H
#define LEXER_H

t_lexer	*ft_state_lexer(char *line);
t_token	*get_next_token(t_lexer *l);
t_token	*ft_handle_operator(t_lexer *l);
t_token	*ft_which_or(t_lexer *l);
t_token	*ft_which_and(t_lexer *l);
t_token	*ft_which_redir(t_lexer *l);
t_token	*ft_handle_word(t_lexer *l);
char	*ft_get_unquoted_str(t_lexer *l);
void	add_token_back(t_token **tok_label, t_token *new);
void	free_token_lst(t_token **tok_label, void (*del)(t_token *));
void	free_token(t_token *t);
void	free_lexer(t_lexer *l);

#endif
