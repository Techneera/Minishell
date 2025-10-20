#ifndef AST_H
# define AST_H

t_ast	*ft_ast_new_cmd(t_cmd *cmd);
t_ast	*ft_ast_new_pipe(t_ast *l, t_ast *r);

#endif
