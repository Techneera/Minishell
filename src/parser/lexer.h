#ifndef LEXER_H
#define LEXER_H

t_lexer	*ft_state_lexer(const char *line);
t_token	*get_next_token(t_lexer *l);
t_token	*ft_handle_operator(t_lexer *l);
t_token	*ft_which_or(t_lexer *l);
t_token	*ft_which_and(t_lexer *l);
t_token	*ft_which_redir(t_lexer *l);
t_token	*ft_handle_word(t_lexer *l);

#endif
