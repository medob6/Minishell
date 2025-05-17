/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 12:23:33 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 15:03:43 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

char	*get_name_heredoc(char *str)
{
	char	*new;

	new = ft_itoa((long)str);
	new = ft_strjoin("/tmp/", new);
	return (new);
}

int	handle_herdoc(char *delimiter)
{
	int	fd;

	fd = -1;
	if (!delimiter)
		return (0);
	read_from_herdoc(delimiter);
	fd = g_herdoc.old_fd;
	return (fd);
}

void	handle_heredoc_no_file_name(t_token **head, t_token **tail)
{
	if (!g_herdoc.no_file_name && *(get_last_status()) != 130)
	{
		append_token(head, tail, create_token(NULL, TOKEN_HEREDOC));
		g_herdoc.no_file_name = 1;
	}
	if (*(get_last_status()) != 130)
		*(get_last_status()) = 2;
}

int	handle_heredoc_case(t_token **head, t_token **tail, char *next)
{
	t_token	*new_token;
	int		fd;
	int		qouts;

	qouts = 0;
	if (next && check_for_operations(next, 0) == '\0')
	{
		if (check_for_q(next))
		{
			qouts = 1;
			next = removes_qouts_heredoc(next);
		}
		fd = handle_herdoc(next);
		new_token = create_token(next, TOKEN_HEREDOC);
		new_token->value.fd_value = fd;
		if (qouts)
			new_token->value.theres_qouts = 1;
		append_token(head, tail, new_token);
		if (g_herdoc.no_file_name && *(get_last_status()) != 130)
			*(get_last_status()) = 2;
		return (1);
	}
	handle_heredoc_no_file_name(head, tail);
	return (0);
}

int	handle_heredoc(t_token **head, t_token **tail, char *next)
{
	int	index;

	index = 0;
	g_herdoc.nbr_heredoc++;
	if (g_herdoc.nbr_heredoc > 16)
	{
		print_str_fd("bash: maximum here-document count exceeded\n", 2);
		g_herdoc.nbr_heredoc = 0;
		close_all_files(head);
		exit(2);
	}
	index = handle_heredoc_case(head, tail, next);
	return (index);
}
