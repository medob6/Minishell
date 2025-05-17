/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lexem2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 12:18:33 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 12:18:49 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

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
