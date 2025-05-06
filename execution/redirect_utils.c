/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:00:15 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/06 16:24:39 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

int	open_file(t_token *file_token)
{
	if (!file_token)
		return (-1);
	if (file_token->type == TOKEN_REDIRECT_IN)
		return (open(file_token->value.str_value, O_RDONLY));
	else if (file_token->type == TOKEN_REDIRECT_OUT)
		return (open(file_token->value.str_value, O_WRONLY | O_CREAT | O_TRUNC,
				0644));
	else if (file_token->type == TOKEN_APPEND)
		return (open(file_token->value.str_value, O_WRONLY | O_CREAT | O_APPEND,
				0644));
	else if (file_token->type == TOKEN_HEREDOC)
		return (file_token->value.fd_value);
	return (-1);
}

void	redirect(t_data *data, t_token *file_obj)
{
	int	fd;

	if (is_ambiguous_redirect(file_obj))
		exit_status(data, 1);
	fd = open_file(file_obj);
	if (fd != -1)
	{
		if (file_obj->type == TOKEN_APPEND
			|| file_obj->type == TOKEN_REDIRECT_OUT)
			dup2(fd, STDOUT_FILENO);
		else
			dup2(fd, STDIN_FILENO);
	}
	else
	{
		print_err(strerror(errno), file_obj->value.str_value);
		exit_status(data, 1);
	}
	close(fd);
}

bool	is_ambiguous_redirect(t_token *token)
{
	if (token->value.fd_value == AMBIGUOUS_REDIRECTION)
	{
		ft_putstr_fd("minishell: ambiguous redirect\n", 2);
		return (true);
	}
	return (false);
}

bool	open_output_redirect(t_data *data, t_token *token, int *last_idx,
		int idx)
{
	data->out_fd = open_file(token);
	if (data->out_fd < 0)
	{
		print_err(strerror(errno), token->value.str_value);
		return (false);
	}
	if (!isatty(data->out_fd))
		*last_idx = idx;
	return (true);
}
