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
#ifndef AST_H
# define AST_H

# include "libshell.h"

t_ast			*ft_ast_node_command(t_cmd *cmd);
t_ast			*ft_ast_generic_node(t_node_type type);
t_parser		*ft_init_parser(t_lexer *l);
t_cmd			*ft_create_command(char **av, t_redir *redirs, int count);
void			ft_parser_iter(t_parser *parser);

void			ft_init_simp_ctx(t_simp_ctx *ctx);
void			*ft_free_simp_ctx(t_simp_ctx *ctx);
int				ft_handle_word_token(t_parser *p, t_simp_ctx *ctx);
int				ft_handle_redir_token(t_parser *p, t_simp_ctx *ctx);
t_ast			*ft_finalize_simp_cmd(t_simp_ctx *ctx);
int				ft_isredir(t_parser *parser);
int				ft_proc_redir_loop(t_parser *p, t_list **lst, t_cmd *cmd);
t_redir			*ft_extract_redir_info(t_parser *p, t_label_redir label);
t_label_redir	ft_label_map(t_token_label label);

t_ast			*ft_parser(t_lexer *l);
t_ast			*ft_parse_and_or(t_parser *parser);
t_ast			*ft_parse_pipeline(t_parser *parser);
t_ast			*ft_parse_grain_with_redirs(t_parser *parser);
t_ast			*ft_parse_node_command(t_parser *parser);
t_ast			*ft_parse_subshell(t_parser *parser);
t_ast			*ft_parse_simple_command(t_parser *parser);
void			ft_parse_args(t_parser *parser, t_list **head);
char			**ft_lst_to_args(t_list **head, int size);
int				ft_handle_redirects(t_parser *parser, t_cmd *cmd_to_fill);
t_redir			*ft_create_redir(t_label_redir label, char *str);
t_redir			*ft_parse_single_redir(t_parser *parser);
void			ft_free_redir_struct_only(void *content);
void			ft_free_redir_content(void *content);
void			ft_copy_lst_to_array(t_list *head, t_redir *array);

void			ft_free_array(char **arr);
void			ft_free_cmd(t_cmd *cmd);
void			ft_free_ast(t_ast *root);

#endif
