/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:58:44 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/11 14:58:36 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ast_node	*creat_ast_node(int node_type)
{
	t_ast_node	*new_node;

	new_node = ft_calloc(1, sizeof(t_ast_node));
	new_node->children = NULL;
	new_node->field = NULL;
	new_node->redirect_list = NULL;
	new_node->str = NULL;
	new_node->type = node_type;
	return (new_node);
}

t_ast_node	*simple_command(t_token **token)
{
	t_ast_node	*simple_cmd;

	simple_cmd = creat_ast_node(AST_SIMPLE_CMD);
	while (true)
	{
		if (is_redirction((*token)->type))
		{
			if (!((*token)->value.str_value))
				return (NULL);
			add_redirect(simple_cmd, *token);
		}
		else if ((*token)->type == TOKEN_WORD)
			add_child(simple_cmd, (*token)->value.str_value);
		else
			break ;
		advance_token(token);
	}
	if ((!simple_cmd->children && !simple_cmd->redirect_list))
		return (NULL);
	return (simple_cmd);
}

t_ast_node	*command(t_token **token)
{
	t_ast_node	*command;

	command = subshell(token);
	if (command)
		return (command);
	return (simple_command(token));
}
