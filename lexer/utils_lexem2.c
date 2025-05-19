/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lexem2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 12:18:33 by salahian          #+#    #+#             */
/*   Updated: 2025/05/19 09:18:55 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	init_global_var(void)
{
	g_herdoc.exit_sign = 0;
	g_herdoc.fd_heredoc = -1;
	g_herdoc.nbr_heredoc = 0;
	g_herdoc.no_file_name = 0;
	g_herdoc.old_fd = -1;
}

t_env	**get_env_head_addres(t_env **address)
{
	static t_env	**head;

	if (address)
		head = address;
	return (head);
}

int	is_operator(const char *c, int i)
{
	return ((c[i] == '&' && c[i + 1] == '&') || (c[i] == '|' && c[i + 1] == '|')
		|| c[i] == '|' || c[i] == '<' || c[i] == '>' || c[i] == '('
		|| c[i] == ')');
}

int	check_quote(char *cmd_line)
{
	int		i;
	int		qout;
	char	c;

	i = 0;
	qout = 0;
	while (cmd_line[i])
	{
		if (ft_strchr("'\"", cmd_line[i]) != NULL)
		{
			c = cmd_line[i];
			qout++;
			i++;
			while (cmd_line[i] && cmd_line[i] != c)
				i++;
			if (cmd_line[i] == c)
			{
				qout--;
				i++;
			}
		}
		else
			i++;
	}
	return (qout == 0);
}
