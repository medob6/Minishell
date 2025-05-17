/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lexem.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 12:14:34 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 14:46:04 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	check_quotes(char *cmd_line)
{
	return (check_quote(cmd_line) && check_quote(cmd_line));
}

int	handle_par(char **str, char *c, int *i)
{
	if (is_operator(c, *i) && c[*i] != '(' && c[*i] != ')'
		&& c[*i] == c[*i + 1])
	{
		*str = ft_malloc(1, 3);
		(*str)[0] = c[*i];
		(*str)[1] = c[*i];
		(*str)[2] = '\0';
		(*i) += 2;
		return (1);
	}
	*str = ft_malloc(1, 2);
	(*str)[0] = c[*i];
	(*str)[1] = '\0';
	(*i)++;
	return (1);
}

int	handle_in_the_qouts(char **str, char *cmd_line, int *i)
{
	int		start;
	char	quote;
	int		n;

	quote = cmd_line[*i];
	start = *i;
	n = 0;
	while (cmd_line[*i] == '\'' || cmd_line[*i] == '"')
	{
		*i = skip_quotes(cmd_line, *i, &n);
	}
	(void)n;
	*str = ft_malloc(*i - start + 1, 1);
	ft_strlcpy(*str, &cmd_line[start], *i - start + 1);
	return (1);
}

int	check_for_operation(char *cmd_line)
{
	int	i;

	i = 0;
	while (cmd_line[i])
	{
		if (cmd_line[i] == '|')
			return ((cmd_line[i + 1] == '|') + 1);
		else if (cmd_line[i] == '&' && cmd_line[i + 1] == '&')
			return (2);
		else if (cmd_line[i] == '>')
			return ((cmd_line[i + 1] == '>') + 1);
		else if (cmd_line[i] == '<')
			return ((cmd_line[i + 1] == '<') + 1);
		else if (cmd_line[i] == '(' || cmd_line[i] == ')')
			return (1);
		i++;
	}
	return (0);
}

int	handle_normal_words(char **str, char *cmd_line, int *i)
{
	int	start;

	start = *i;
	while (cmd_line[*i] && cmd_line[*i] != ' ' && cmd_line[*i] != '\t'
		&& !is_operator(cmd_line, *i))
		(*i)++;
	check_for_operation(cmd_line);
	*str = ft_malloc(*i - start + 1, 1);
	ft_strlcpy(*str, &cmd_line[start], *i - start + 1);
	return (1);
}
