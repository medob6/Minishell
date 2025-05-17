/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:05:18 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/17 14:52:25 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

bool	open_input_redirect(t_data *data, t_str *token)
{
	data->out_fd = open_file(token);
	if (data->out_fd < 0)
	{
		print_err(strerror(errno), token->value[0]);
		return (false);
	}
	close(data->out_fd);
	data->out_fd = 1;
	return (true);
}

bool	handle_redirect_token(t_data *data, t_str *token, int *last_idx,
		int idx)
{
	if (data->out_fd != 1)
		close(data->out_fd);
	if (is_ambiguous_redirect(token))
	{
		data->out_fd = -1;
		return (false);
	}
	if (token->type == TOKEN_APPEND || token->type == TOKEN_REDIRECT_OUT)
		return (open_output_redirect(data, token, last_idx, idx));
	else
		return (open_input_redirect(data, token));
}

bool	process_redirections(t_data *data, t_str **redir_lst, int *last_idx)
{
	size_t	i;

	i = 0;
	while (redir_lst && redir_lst[i])
	{
		if (!handle_redirect_token(data, redir_lst[i], last_idx, i))
			return (false);
		i++;
	}
	return (true);
}

bool	reopen_last_output(t_data *data, t_str **redir_lst, int last_idx)
{
	if (last_idx != -1)
	{
		close(data->out_fd);
		data->out_fd = open_file(redir_lst[last_idx]);
	}
	return (true);
}

bool	redirection_builtins(t_data *data, int n)
{
	t_str	**redir_lst;
	int		last_not_atty;

	last_not_atty = -1;
	redir_lst = data->lst_cmd[n].redirlist;
	reset_fds(data, n);
	if (!process_redirections(data, redir_lst, &last_not_atty))
	{
		data->lst_cmd[n].exit_status = 1;
		return (false);
	}
	return (reopen_last_output(data, redir_lst, last_not_atty));
}
