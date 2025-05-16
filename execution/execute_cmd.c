/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:44:10 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/16 23:20:47 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	exit_status(t_data *prg_data, int status)
{
	free_garbeg(prg_data);
	exit(status);
}

void	wait_for_prc(t_cmd *cmd_list, int cmd_nbr)
{
	int	i;
	int	status;

	i = 0;
	if (cmd_nbr == 1 && cmd_list[0].is_built_in)
		return ;
	signal(SIGINT, SIG_IGN);
	while (i < cmd_nbr)
	{
		waitpid(cmd_list[i].pid, &status, 0);
		signal(SIGINT, handler);
		cmd_list[i].exit_status = 0;
		if (WIFEXITED(status))
			cmd_list[i].exit_status = WEXITSTATUS(status);
		else if (WIFSTOPPED(status))
			cmd_list[i].exit_status = WSTOPSIG(status) + 128;
		else if (WIFSIGNALED(status))
			cmd_list[i].exit_status = WTERMSIG(status) + 128;
		i++;
	}
	sig_exit(cmd_list, cmd_nbr);
}

int	exec_cmd(t_cmd cmd, char **envp, char *new_path)
{
	if (access(cmd.path, F_OK) != 0)
	{
		print_err(strerror(errno), cmd.path);
		return (127);
	}
	else if (access(new_path, F_OK) != 0)
	{
		execve(cmd.path, cmd.args, envp);
		print_err(strerror(errno), cmd.path);
	}
	else
		print_err(strerror(21), cmd.path);
	return (126);
}

void	execute_cmd(t_cmd cmd, t_data *prg_data)
{
	char	*new_path;
	char	**envp;
	int		status;

	envp = extract_envp(prg_data->env);
	if (!cmd.path)
		handle_missing_cmd(cmd, prg_data);
	new_path = ft_strjoin(cmd.path, "/");
	status = exec_cmd(cmd, envp, new_path);
	ft_free(new_path);
	exit_status(prg_data, status);
}
