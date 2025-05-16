/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_funcs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 20:13:44 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/16 22:28:06 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	get_length(t_str **strs)
{
	int	len;
	int	j;
	int	i;

	i = 0;
	len = 0;
	while (strs[i])
	{
		j = 0;
		while (strs[i]->value && strs[i]->value[j])
		{
			len++;
			j++;
		}
		i++;
	}
	return (len);
}

int	*get_last_status(void)
{
	static int	status;

	return (&status);
}

void	sig_exit(t_cmd *cmd, int cmd_nbr)
{
	int	i;

	i = 0;
	while (i < cmd_nbr)
	{
		if (cmd[i].exit_status == 130 || cmd[i].exit_status == 131)
		{
			write(1, "\n", 1);
			break ;
		}
		i++;
	}
}

int	update_status_sp_case(void)
{
	if (*get_last_status() != 0)
		return (*get_last_status());
	return (1);
}

int	execute_subshell(t_ast_node *subshell, t_env *env)
{
	int	status;

	status = execute_cmd_line(subshell, env);
	return (status);
}
