/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexem.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:41:15 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 12:19:11 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

static char	*handle_qouts(char *cmd_line, int *i)
{
	char	*tmp;
	char	quote;
	int		start;

	tmp = NULL;
	quote = cmd_line[*i];
	start = (*i)++;
	while (cmd_line[*i] && cmd_line[*i] != quote)
		(*i)++;
	if (cmd_line[*i] == quote)
		(*i)++;
	tmp = ft_substr(cmd_line, start, *i - start);
	return (tmp);
}

static int	handle_word(char **str, char *cmd_line, int *i)
{
	char	*tmp;
	int		start;

	tmp = NULL;
	while (cmd_line[*i] && cmd_line[*i] != ' ' && cmd_line[*i] != '\t'
		&& !is_operator(cmd_line, *i))
	{
		if (cmd_line[*i] == '\'' || cmd_line[*i] == '"')
			tmp = ft_strjoin(tmp, handle_qouts(cmd_line, i));
		else
		{
			start = *i;
			while (cmd_line[*i] && cmd_line[*i] != ' ' && cmd_line[*i] != '\t'
				&& !is_operator(cmd_line, *i) && cmd_line[*i] != '\''
				&& cmd_line[*i] != '"')
				(*i)++;
			tmp = ft_strjoin(tmp, ft_substr(cmd_line, start, *i - start));
		}
	}
	*str = tmp;
	return (1);
}

int	handle_operators(char **str, char *cmd_line, int *i, int count)
{
	*str = ft_malloc(1, count + 1);
	ft_strlcpy(*str, &cmd_line[*i], count + 1);
	*i += count;
	return (1);
}

char	**help_lexer(char **str, char *cmd_line)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (cmd_line[i])
	{
		while (cmd_line[i] == ' ' || cmd_line[i] == '\t')
			i++;
		if (!cmd_line[i])
			break ;
		if (is_operator(cmd_line, i))
		{
			j += handle_par(&str[j], cmd_line, &i);
			continue ;
		}
		else
		{
			j += handle_word(&str[j], cmd_line, &i);
			continue ;
		}
	}
	str[j] = NULL;
	return (str);
}

char	**lexer(char *cmd_line)
{
	char	**str;
	int		total;

	if (!check_quotes(cmd_line))
	{
		print_str_fd("ERROR:\nbalanced brakets.\n", 2);
		return (NULL);
	}
	total = count_words(cmd_line);
	str = ft_malloc(sizeof(char *), (total + 1));
	return (help_lexer(str, cmd_line));
}
