/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:14:22 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/05 17:28:35 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	init_program_data(t_data *data, t_ast_node *pipeline, t_env *env)
{
	data->old_fd = -1;
	data->out_fd = 1;
	data->fd[0] = -1;
	data->fd[1] = -1;
	data->env = env;
	expand_pipeline(pipeline, &data->env);
	data->cmd_nbr = pipeline->children->length;
	data->lst_cmd = build_cmd_list(data->cmd_nbr,
			(t_ast_node **)pipeline->children->items, env);
	data->env = env;
}

bool	should_continue(int status, t_ast_node *op)
{
	if ((status == 0 && op->type == AST_AND) || (status != 0
			&& op->type == AST_OR))
		return (true);
	return (false);
}

void	skip_operators(size_t *i, int status, t_ast_node *root)
{
	t_ast_node	*op;

	while (true)
	{
		*i += 2;
		op = (t_ast_node *)root->children->items[*i];
		if (!op)
			break ;
		if (should_continue(status, op))
			break ;
	}
	(*i)++;
}
int	execute_cmd_line(t_ast_node *root, t_env *env)
{
	int			status;
	t_ast_node	*cmd;
	t_ast_node	*op;
	size_t		i;

	i = 0;
	status = 0;
	if (!root || !root->children || root->children->length == 0)
		return (0);
	while (i < root->children->length)
	{
		cmd = (t_ast_node *)root->children->items[i];
		if (cmd->type == AST_PIPELINE)
			status = execute_pipeline(cmd, env);
		if (++i < root->children->length)
		{
			op = (t_ast_node *)root->children->items[i];
			if (should_continue(status, op))
				i++;
			else
				skip_operators(&i, status, root);
		}
	}
	return (status);
}

int	execution(t_ast_node *root, t_env *env)
{
	int n;

	n = execute_cmd_line(root, env);
	return (n);
}