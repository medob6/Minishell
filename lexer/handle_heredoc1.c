/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nandle_heredoc1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 12:25:34 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 12:27:43 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	create_temp_file(char *del)
{
	char	*str;

	str = get_name_heredoc(del);
	herdoc.fd_heredoc = open(str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	herdoc.old_fd = open(str, O_RDONLY);
	unlink(str);
	if (herdoc.fd_heredoc == -1 || herdoc.old_fd == -1)
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
	close(herdoc.fd_heredoc);
	close(herdoc.old_fd);
	close_all_files(get_token());
	ft_lstclear(garbage_list());
	exit(130);
}

void	read_from_herdoc(char *delemeter)
{
	pid_t	pid;
	int		status;

	if (herdoc.exit_sign == 130)
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
	herdoc.exit_sign = get_status(status);
	if (herdoc.exit_sign == 130)
		print_str_fd("\n", 2);
	*(get_last_status()) = herdoc.exit_sign;
	close(herdoc.fd_heredoc);
}
