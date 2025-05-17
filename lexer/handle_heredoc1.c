/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 12:25:34 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 15:03:43 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	create_temp_file(char *del)
{
	char	*str;

	str = get_name_heredoc(del);
	g_herdoc.fd_heredoc = open(str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	g_herdoc.old_fd = open(str, O_RDONLY);
	unlink(str);
	if (g_herdoc.fd_heredoc == -1 || g_herdoc.old_fd == -1)
		handle_error(str);
}

int	get_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSTOPPED(status))
		return (WSTOPSIG(status) + 128);
	else if (WIFSIGNALED(status))
		return (WTERMSIG(status) + 128);
	return (1);
}

void	close_all_files(t_token **head)
{
	t_token	*tmp;

	tmp = *head;
	while (tmp)
	{
		if (tmp->value.fd_value > -1)
			close(tmp->value.fd_value);
		tmp = tmp->next;
	}
}

void	handle_signal(int sig)
{
	(void)sig;
	close(g_herdoc.fd_heredoc);
	close(g_herdoc.old_fd);
	close_all_files(get_token());
	ft_lstclear(garbage_list());
	exit(130);
}

void	read_from_herdoc(char *delemeter)
{
	pid_t	pid;
	int		status;

	if (g_herdoc.exit_sign == 130)
		return ;
	create_temp_file(delemeter);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, handle_signal);
		process_input(delemeter);
	}
	status = 0;
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, handler);
	g_herdoc.exit_sign = get_status(status);
	if (g_herdoc.exit_sign == 130)
		print_str_fd("\n", 2);
	*(get_last_status()) = g_herdoc.exit_sign;
	close(g_herdoc.fd_heredoc);
}
