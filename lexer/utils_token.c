/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 12:28:56 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 12:31:46 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

char	check_for_operations(char *cmd_line, int i)
{
	if (cmd_line[i] == '|' && cmd_line[i + 1] == '|')
		return ('o');
	else if (cmd_line[i] == '&' && cmd_line[i + 1] == '&')
		return ('e');
	else if (cmd_line[i] == '>' && cmd_line[i + 1] == '>')
		return ('a');
	else if (cmd_line[i] == '<' && cmd_line[i + 1] == '<')
		return ('h');
	else if (cmd_line[i] == '|')
		return ('|');
	else if (cmd_line[i] == '>')
		return ('>');
	else if (cmd_line[i] == '<')
		return ('<');
	else if (cmd_line[i] == '(')
		return ('(');
	else if (cmd_line[i] == ')')
		return (')');
	return ('\0');
}

t_token	**get_token(void)
{
	static t_token	*head;

	return (&head);
}

char	*removes_qouts_heredoc(char *str)
{
	char	*new_str;
	int		j;

	j = 0;
	new_str = ft_strdup("");
	while (str && str[j])
	{
		if (str[j] == '\'' && check_for_next_one(str, j))
			j = take_inside_qout(&new_str, str, j);
		else if (str[j] == '"' && check_for_next_one(str, j))
			j = take_inside_qout(&new_str, str, j);
		else
			new_str = append_char(new_str, str[j++]);
	}
	return (new_str);
}

int	is_delemeter(char *line, char *delemeter)
{
	if (!ft_strcmp(line, delemeter))
		return (1);
	return (0);
}

int	check_for_q(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			return (1);
		i++;
	}
	return (0);
}
