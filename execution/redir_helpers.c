/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:21:16 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/17 11:30:13 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

void	perforem_subshell_redirs(t_data *data, int n)
{
	size_t		i;
	t_str		**redir_lst;
	t_ast_node	*subshell_node;

	i = 0;
	redir_lst = NULL;
	subshell_node = data->lst_cmd[n].subshell_node;
	if (subshell_node->redirect_list)
		redir_lst = (t_str **)subshell_node->redirect_list->items;
	if (n != data->cmd_nbr - 1)
		ft_close(data->fd[0]);
	if (data->old_fd != -1)
	{
		dup2(data->old_fd, STDIN_FILENO);
		ft_close(data->old_fd);
	}
	if (n != data->cmd_nbr - 1)
	{
		dup2(data->fd[1], STDOUT_FILENO);
		ft_close(data->fd[1]);
	}
	while (redir_lst && redir_lst[i])
		redirect(data, redir_lst[i++]);
}

void	perforem_redirections(t_data *data, int n)
{
	size_t	i;
	t_str	**redir_lst;

	i = 0;
	redir_lst = data->lst_cmd[n].redirlist;
	if (n != data->cmd_nbr - 1)
		ft_close(data->fd[0]);
	if (data->old_fd != -1)
	{
		dup2(data->old_fd, STDIN_FILENO);
		ft_close(data->old_fd);
	}
	if (n != data->cmd_nbr - 1)
	{
		dup2(data->fd[1], STDOUT_FILENO);
		ft_close(data->fd[1]);
	}
	while (redir_lst && redir_lst[i])
	{
		redirect(data, redir_lst[i]);
		i++;
	}
}

void	save_old_fd(int *old_fd, int *fd)
{
	ft_close(fd[1]);
	if (*old_fd >= 0)
		ft_close(*old_fd);
	*old_fd = fd[0];
}

void	reset_fds(t_data *data, int n)
{
	if (n != data->cmd_nbr - 1)
		ft_close(data->fd[0]);
	if (data->old_fd != -1)
	{
		dup2(data->old_fd, STDIN_FILENO);
		ft_close(data->old_fd);
	}
	if (n != data->cmd_nbr - 1)
	{
		if (data->out_fd != -1)
			data->out_fd = data->fd[1];
		else
			ft_close(data->fd[1]);
	}
}

void	ft_close_here_docs(t_str **redir_list)
{
	int	i;

	i = 0;
	while (redir_list && redir_list[i])
	{
		if (redir_list[i]->type == TOKEN_HEREDOC)
			ft_close(redir_list[i]->fd);
		i++;
	}
	return ;
}
