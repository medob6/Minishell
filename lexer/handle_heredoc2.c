/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 12:27:20 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 15:03:43 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	print_err1(char *err, char *str)
{
	char	*buffer;
	char	*tmp;

	buffer = ft_strjoin("\nminishell: ", err);
	tmp = buffer;
	buffer = ft_strjoin(tmp, str);
	ft_free(tmp);
	tmp = buffer;
	buffer = ft_strjoin(tmp, "')\n");
	ft_free(tmp);
	ft_putstr_fd(buffer, 2);
	ft_free(buffer);
}

void	print_err(char *err, char *str)
{
	char	*buffer;
	char	*tmp;

	buffer = ft_strjoin("minishell: ", err);
	tmp = buffer;
	buffer = ft_strjoin(tmp, ": ");
	ft_free(tmp);
	tmp = buffer;
	buffer = ft_strjoin(tmp, str);
	ft_free(tmp);
	tmp = buffer;
	buffer = ft_strjoin(tmp, "\n");
	ft_free(tmp);
	ft_putstr_fd(buffer, 2);
	ft_free(buffer);
}

void	handle_error(char *str)
{
	print_err(strerror(errno), str);
	exit(1);
}

void	clean_exit(void)
{
	close(g_herdoc.fd_heredoc);
	close(g_herdoc.old_fd);
	ft_lstclear(garbage_list());
	exit(0);
}

void	process_input(char *delemeter)
{
	char	*line;
	char	*tmp;

	line = NULL;
	while (1)
	{
		if (line && is_delemeter(line, delemeter))
			break ;
		if (line)
		{
			tmp = ft_strjoin(line, "\n");
			write(g_herdoc.fd_heredoc, tmp, ft_strlen(tmp));
			ft_free(line);
		}
		line = readline("> ");
		if (!line)
			break ;
	}
	if (!line)
		print_err1("warning: here-document delimited by end-of-file (wanted `",
			delemeter);
	ft_free(line);
	clean_exit();
}
