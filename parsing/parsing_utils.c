/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:58:32 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/05 17:58:33 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_and_or(t_token_type type)
{
	if (type == TOKEN_AND || type == TOKEN_OR || type == TOKEN_EOF)
		return (true);
	return (false);
}

void	advance_token(t_token **token)
{
	if (token && *token)
		*token = (*token)->next;
}

void	add_child(t_ast_node *parent, void *new_child)
{
	array_push(&parent->children, new_child);
}

void	add_redirect(t_ast_node *parent, t_token *redir)
{
	array_push(&parent->redirect_list, redir);
}

bool	is_redirction(t_token_type token_type)
{
	if (token_type == TOKEN_REDIRECT_IN || token_type == TOKEN_APPEND
		|| token_type == TOKEN_REDIRECT_OUT || token_type == TOKEN_HEREDOC)
		return (true);
	return (false);
}
