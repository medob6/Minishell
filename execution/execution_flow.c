/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_flow.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:06:20 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/13 08:32:37 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	execute_subshell(t_ast_node *subshell, t_env *env)
{
	int	status;

	status = execute_cmd_line(subshell, env);
	return (status);
}

// TODO add to header file
int	update_status_sp_case(void)
{
	if (*get_last_status() != 0)
		return (*get_last_status());
	return (1);
}

void	child(t_data *prg_data, int index)
{
	t_ast_node	*subshell_node;
	t_cmd		cmd;

	cmd = prg_data->lst_cmd[index];
	if (cmd.is_subshell)
	{
		perforem_subshell_redirs(prg_data, index);
		subshell_node = prg_data->lst_cmd[index].subshell_node;
		prg_data->lst_cmd[index].exit_status = execute_subshell(subshell_node, prg_data->env);
	}
	else if (!cmd.is_built_in)
	{
		perforem_redirections(prg_data, index);
		if (cmd.args == NULL)
			exit_status(prg_data, 0);
		execute_cmd(cmd, prg_data);
	}
	else
	{
		if (redirection_builtins(prg_data, index))
			prg_data->lst_cmd[index].exit_status = execute_built_in(cmd,
					prg_data);
		if (!ft_strcmp(cmd.args[0], "exit") && prg_data->cmd_nbr == 1)
		{
			printf("exit\n");
			if (prg_data->lst_cmd[index].exit_status != -1)
				exit_status(prg_data, prg_data->lst_cmd[index].exit_status);
			prg_data->lst_cmd[index].exit_status = update_status_sp_case();
		}
	}
	if ((!cmd.is_built_in || (prg_data->cmd_nbr > 1)))
		exit_status(prg_data, prg_data->lst_cmd[index].exit_status);
}

void	pipe_execution(t_data *prg_data)
{
	int	i;

	i = 0;
	while (i < prg_data->cmd_nbr)
	{
		if (i != prg_data->cmd_nbr - 1)
			pipe(prg_data->fd);
		if (!prg_data->lst_cmd[i].is_built_in || (prg_data->cmd_nbr > 1))
			prg_data->lst_cmd[i].pid = fork();
		if (prg_data->lst_cmd[i].pid == 0)
			child(prg_data, i);
		else
		{
			save_old_fd(&prg_data->old_fd, prg_data->fd);
			close_here_docs(prg_data->lst_cmd[i].redirlist);
		}
		i++;
	}
}

int	execute_built_in(t_cmd cmd, t_data *data)
{
	if (!ft_strcmp(cmd.args[0], "export"))
		return (ft_export(cmd.args, &data->env, data->out_fd));
	else if (!ft_strcmp(cmd.args[0], "echo"))
		return (ft_echo(cmd.args, data->out_fd));
	else if (!ft_strcmp(cmd.args[0], "cd"))
	{
		if (!cmd.args[2])
			return (ft_cd(cmd.args[1], &data->env));
		else
			print_err("cd", "too many arguments");
	}
	else if (!ft_strcmp(cmd.args[0], "pwd"))
		return (ft_pwd(&data->env, data->out_fd));
	else if (!ft_strcmp(cmd.args[0], "unset"))
		return (ft_unset(cmd.args, &data->env));
	else if (!ft_strcmp(cmd.args[0], "env"))
		return (ft_env(&data->env, data->out_fd));
	else if (!ft_strcmp(cmd.args[0], "exit"))
		return (ft_exit(cmd.args));
	return (1);
}

int	execute_pipeline(t_ast_node *pipeline, t_env *env)
{
	t_data	prg_data;
	int		status;

	status = 0;
	init_program_data(&prg_data, pipeline, env);
	pipe_execution(&prg_data);
	wait_for_prc(prg_data.lst_cmd, prg_data.cmd_nbr);
	status = prg_data.lst_cmd[prg_data.cmd_nbr - 1].exit_status;
	free_garbeg(&prg_data);
	return (status);
}
