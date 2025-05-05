/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:58:22 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/05 17:58:23 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	paranteses_symetric(t_token **token)
{
	t_token	*temp_head;

	int count_open, count_close;
	count_open = 0;
	count_close = 0;
	temp_head = *token;
	while (temp_head->type != TOKEN_EOF)
	{
		if (temp_head->type == TOKEN_PARENTESIS_OPEN)
			count_open++;
		if (temp_head->type == TOKEN_PARENTESIS_CLOSE)
			count_close++;
		if (count_close > count_open)
			return (false);
		temp_head = (temp_head)->next;
	}
	if (count_close == count_open)
		return (true);
	return (false);
}

t_array	*append_redirections(t_array *redir1, t_array *redir2)
{
	size_t	i;

	if (!redir1)
		return (redir2);
	i = 0;
	while (i < redir1->length)
		array_push(&redir2, redir1->items[i++]);
	return (redir2);
}

t_ast_node	*flatten_compound_if_possible(t_ast_node *compouned)
{
	t_ast_node	*pipeline;
	t_ast_node	*child;
	t_array		*redir_list;

	if (!compouned || compouned->children->length != 1)
		return (compouned);
	pipeline = compouned->children->items[0];
	if (pipeline->children->length != 1)
		return (compouned);
	child = pipeline->children->items[0];
	if (child->type != AST_SUBSHELL && child->type != AST_SIMPLE_CMD)
		return (compouned);
	if (!compouned->redirect_list)
		return (child);
	redir_list = append_redirections(child->redirect_list,
			compouned->redirect_list);
	child->redirect_list = redir_list;
	return (child);
}

t_ast_node	*subshell(t_token **token)
{
	t_ast_node	*compouned;

	if ((*token)->type != TOKEN_PARENTESIS_OPEN)
		return (NULL);
	advance_token(token);
	compouned = compound_cmd(token, AST_SUBSHELL);
	if ((*token)->type != TOKEN_PARENTESIS_CLOSE)
		return (NULL);
	advance_token(token);
	while (true)
	{
		if (!is_redirction((*token)->type))
			break ;
		if (!(*token)->value.str_value)
			return (NULL);
		add_redirect(compouned, *token);
		advance_token(token);
	}
	return (flatten_compound_if_possible(compouned));
}
