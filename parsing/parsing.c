/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:58:12 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/06 13:45:00 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ast_node	*pipeline(t_token **token)
{
	t_ast_node	*pipe_node;
	t_ast_node	*cmd;

	pipe_node = creat_ast_node(AST_PIPELINE);
	while (true)
	{
		cmd = command(token);
		if (!cmd)
			return (NULL);
		add_child(pipe_node, cmd);
		if ((*token)->type != TOKEN_PIPE)
			break ;
		advance_token(token);
	}
	return (pipe_node);
}

static t_ast_node	*create_logic_node(t_token **token)
{
	t_ast_node	*logic;

	if ((*token)->type == TOKEN_AND)
		logic = creat_ast_node(AST_AND);
	else
		logic = creat_ast_node(AST_OR);
	advance_token(token);
	return (logic);
}

void	add_logic_node(t_token **token, t_ast_node *curr, t_ast_node *compound)
{
	curr = create_logic_node(token);
	add_child(compound, curr);
}

t_ast_node	*compound_cmd(t_token **token, t_ast_type type)
{
	static int	depth;
	t_ast_node	*compound;
	t_ast_node	*current;

	depth++;
	compound = creat_ast_node(type);
	while (*token && (*token)->type != TOKEN_EOF)
	{
		current = pipeline(token);
		if (!current || current->type == TOKEN_EOF)
			return (NULL);
		add_child(compound, current);
		if ((*token)->type == TOKEN_AND || (*token)->type == TOKEN_OR)
		{
			add_logic_node(token, current, compound);
			if ((*token)->type == TOKEN_EOF)
				return (NULL);
		}
		else
			break ;
	}
	depth--;
	if (depth == 0 && (*token)->type != TOKEN_EOF)
		return (NULL);
	return (compound);
}

t_ast_node	*parse_tokens(t_token *tokens)
{
	t_ast_node	*root;

	if (!paranteses_symetric(&tokens))
		return (NULL);
	root = compound_cmd(&tokens, AST_COMPOUNED_CMD);
	if (!root)
		return (NULL);
	return (root);
}
