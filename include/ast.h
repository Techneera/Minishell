#ifndef AST_H
# define AST_H

# include "libshell.h"

t_ast	*ft_ast_node(t_cmd *cmd);
t_cmd	*ft_create_cmd(char **av, t_redir *redirs);
t_redir	*ft_create_redir(t_label_redir label, char *str);
void	ft_free_cmd(t_cmd *cmd);
void	ft_free_node(t_ast *root);

#endif
