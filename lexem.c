/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexem.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:41:15 by salahian          #+#    #+#             */
/*   Updated: 2025/03/17 15:42:10 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_single_quotes(char *cmd_line)
{
	int	i;
	int	single_qout;

	i = 0;
	single_qout = 0;
	while (cmd_line[i])
	{
		if (cmd_line[i] == '\'')
		{
			single_qout++;
			i++;
			while (cmd_line[i] && cmd_line[i] != '\'')
				i++;
			if (cmd_line[i] == '\'')
			{
				single_qout--;
				i++;
			}
		}
		else
			i++;
	}
	return (single_qout == 0);
}

int	check_double_quotes(char *cmd_line)
{
	int	i;
	int	double_qout;

	i = 0;
	double_qout = 0;
	while (cmd_line[i])
	{
		if (cmd_line[i] == '"')
		{
			double_qout++;
			i++;
			while (cmd_line[i] && cmd_line[i] != '"')
				i++;
			if (cmd_line[i] == '"')
			{
				double_qout--;
				i++;
			}
		}
		else
			i++;
	}
	return (double_qout == 0);
}

int	check_quotes(char *cmd_line)
{
	return (check_single_quotes(cmd_line) && check_double_quotes(cmd_line));
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	count;
	size_t	i;

	count = ft_strlen(src);
	i = 0;
	if (dstsize != 0)
	{
		while ((src[i]) && (i < dstsize - 1))
		{
			dst[i] = src[i];
			i++;
		}
		dst[i] = '\0';
	}
	return (count);
}

int	get_next_qout(char *cmd_line, int i)
{
	int		j;
	char	c;

	j = i + 1;
	c = cmd_line[i];
	while (cmd_line[j])
	{
		if (cmd_line[j] == c)
			return (j);
		j++;
	}
	return (i);
}

int	count_words(char const *s)
{
	int	i = 0, count;
	int	q, in_word;

	i = 0, count = 0;
	in_word = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			q = get_next_qout((char *)s, i);
			if (q > i)
				i = q;
			in_word = 1;
		}
		else if (s[i] == ' ' || s[i] == '\t')
		{
			if (in_word)
			{
				count++;
				in_word = 0;
			}
		}
		else
			in_word = 1;
		i++;
	}
	if (in_word)
		count++;
	return (count);
}

int	handle_in_the_qouts(char **str, char *cmd_line, int *i)
{
	int		start;
	char	quote;

	quote = cmd_line[*i];
	start = *i;
	(*i)++;
	while (cmd_line[*i] && cmd_line[*i] != quote)
		(*i)++;
	if (cmd_line[*i] == quote)
		(*i)++;
	*str = malloc(*i - start + 1);
	ft_strlcpy(*str, &cmd_line[start], *i - start + 1);
	return (1);
}

int	handle_normal_words(char **str, char *cmd_line, int *i)
{
	int	start;

	start = *i;
	while (cmd_line[*i] && cmd_line[*i] != ' ' && cmd_line[*i] != '\t'
		&& cmd_line[*i] != '(' && cmd_line[*i] != ')')
		(*i)++;
	*str = malloc(*i - start + 1);
	ft_strlcpy(*str, &cmd_line[start], *i - start + 1);
	return (1);
}

char	**help_lexer(char **str, char *cmd_line)
{
	int	i = 0;
	int	j = 0;

	while (cmd_line[i])
	{
		while (cmd_line[i] == ' ' || cmd_line[i] == '\t')
			i++;
		if (!cmd_line[i])
			break;
		if (cmd_line[i] == '(' || cmd_line[i] == ')')
		{
			j += handle_par(&str[j], cmd_line[i], &i);
			continue;
		}
		if (cmd_line[i] == '\'' || cmd_line[i] == '"')
		{
			j += handle_in_the_qouts(&str[j], cmd_line, &i);
			continue;
		}
		j += handle_normal_words(&str[j], cmd_line, &i);
	}
	str[j] = NULL;
	return (str);
}

int	check_parenthesis(char *cmd_line)
{
	int	i;
	int	parenthesis;

	i = 0;
	parenthesis = 0;
	while (cmd_line[i])
	{
		if (cmd_line[i] == '(')
		{
			parenthesis++;
			i++;
			while (cmd_line[i] && cmd_line[i] != ')')
				i++;
			if (cmd_line[i] == ')')
			{
				parenthesis--;
				i++;
			}
		}
		else
			i++;
	}
	return (parenthesis == 0);
}

char	**lexer(char *cmd_line)
{
	char	**str;
	int		total;

	if (!check_quotes(cmd_line))
	{
		ft_putstr("ERROR:\nbalanced brakets.\n", 2);
		return (NULL);
	}
	if (!check_parenthesis(cmd_line))
	{
		ft_putstr("ERROR:\nbalanced paronthesis.\n", 2);
		return (NULL);
	}
	total = count_words(cmd_line);
	str = malloc(sizeof(char *) * (1024));
	return (help_lexer(str, cmd_line));
}

// int main(void)
// {
// 	char **s = lexer("(echo 'hello   world' | wc -c | ls *.c)");
// 	int i = 0;
// 	while (s[i])
// 	{
// 		printf("lexer[%d]: [%s]\n", i, s[i]);
// 		i++;
// 	}
// 	return (0);
// }
