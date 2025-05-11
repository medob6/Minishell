/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:39:28 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/11 17:57:20 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

bool	init_cmd_structure(t_cmd *cmd, t_ast_node *node)
{
	if (!cmd || !node)
		return (false);
	cmd->is_built_in = 0;
	cmd->redirlist = NULL;
	cmd->is_subshell = false;
	if (node->redirect_list)
		cmd->redirlist = (t_str **)node->redirect_list->items;
	return (true);
}

bool	set_cmd_args_and_path(t_cmd *cmd, char **cmd_args, t_env *env)
{
	if (!cmd_args || !cmd_args[0])
	{
		cmd->args = NULL;
		cmd->path = NULL;
		return (false);
	}
	cmd->args = cmd_args;
	if (cmd_is_path(cmd_args[0]))
		cmd->path = ft_strdup(cmd_args[0]);
	else
		cmd->path = get_cmd_path(cmd_args[0], extract_envp(env));
	return (true);
}

void	handle_missing_cmd(t_cmd cmd, t_data *prg_data)
{
	if (!cmd.args)
		ft_putstr_fd("minishell: Command not found: \n", 2);
	else
		print_err("command not found", cmd.args[0]);
	exit_status(prg_data, 127);
}

bool	cmd_is_path(char *s)
{
	if (ft_strchr(s, '/'))
		return (true);
	return (false);
}
