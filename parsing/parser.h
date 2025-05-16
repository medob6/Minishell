/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:59:29 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/16 22:31:26 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../minishell.h"

// parser functions
t_ast_node	*parse_tokens(t_token *token);
t_ast_node	*creat_ast_node(int node_type);
void		add_child(t_ast_node *parent, void *new_child);
void		add_redirect(t_ast_node *parent, t_token *redir);
void		advance_token(t_token **token);
bool		is_redirction(t_token_type token_type);
t_ast_node	*compound_cmd(t_token **token, t_ast_type type);
bool		paranteses_symetric(t_token **token);


#endif