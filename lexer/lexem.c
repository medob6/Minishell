/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexem.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:41:15 by salahian          #+#    #+#             */
/*   Updated: 2025/05/13 17:19:53 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

// void    ft_putstr(char *s, int fd)
// {
//     write(fd, s, ft_strlen(s));
// }

static int is_operator(const char *c, int i)
{
	return ((c[i] == '&' && c[i + 1] == '&') || (c[i] == '|' && c[i + 1] == '|')
	|| c[i] == '|' || c[i] == '<' || c[i] == '>' || c[i] == '(' || c[i] == ')');
}

static int check_quote(char *cmd_line)
{
	int i;
	int qout;
	char c;

	i = 0;
	qout = 0;
	while (cmd_line[i])
	{
		if (ft_strchr("'\"",cmd_line[i]) != NULL)
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

static int check_quotes(char *cmd_line)
{
	return (check_quote(cmd_line) && check_quote(cmd_line));
}

size_t ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t count;
	size_t i;

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

int skip_quotes(const char *s, int i, int *in_word)
{
	char c;

	if (ft_strchr("'\"", s[i]) != NULL)
	{
		c = s[i];
		i++;
		while (s[i] && s[i] != c)
			i++;
		if (s[i] == c)
			i++;
		*in_word = 1;
	}
	return (i);
}

// int count_words(char const *s)
// {
// 	int i;
// 	int count;
// 	int in_word;

// 	i = 0;
// 	count = 0;
// 	in_word = 0;
// 	while (s[i])
// 	{
// 		//i = skip_quotes(s, i, &in_word);
// 		if (is_operator(s, i))
// 		{
// 			if (in_word)
// 			{
// 				count++;
// 				in_word = 0;
// 			}
// 			if (is_operator(s, i + 1) && (s[i + 1] == '|' || s[i + 1] == '&'))
// 				i++;
// 			in_word = 0;
// 			count++;
// 		}
// 		if ((s[i] == ' ' || s[i] == '\t'))
// 		{
// 			count++;
// 			in_word = 0;
// 		}
// 		else
// 			in_word = 1;
// 		i++;
// 	}
// 	if (in_word)
// 		count++;
// 	return (count);
// }

static void	help_count_words(char const *s, int *i, int *count, int *in_word)
{
	if (*in_word)
	{
		(*count)++;
		*in_word = 0;
	}
	if ((s[*i] == '|' && s[*i + 1] == '|') || (s[*i] == '&' && s[*i + 1] == '&'))
		(*i)++;
	(*count)++;
	(*i)++;
}

static void	check_if_in_word(int *in_word, int *count, int *i)
{
	if (*in_word)
	{
		(*count)++;
		*in_word = 0;
	}
	(*i)++;
}

static int count_words(char const *s)
{
	int i;
	int count;
	int in_word;

	i = 0;
	count = 0;
	in_word = 0;
	while (s[i])
	{
		if (s[i] == ' ' || s[i] == '\t')
		{
			check_if_in_word(&in_word, &count, &i);
			continue;
		}
		if (is_operator(s, i))
		{
			help_count_words(s, &i, &count, &in_word);
			continue;
		}
		in_word = 1;
		i++;
	}
	if (in_word)
		count++;
	return (count);
}



static int handle_par(char **str, char *c, int *i)
{
	if (is_operator(c, *i) && c[*i] != '(' && c[*i] != ')' && c[*i] == c[*i + 1])
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

int handle_in_the_qouts(char **str, char *cmd_line, int *i)
{
	int start;
	char quote;
	int	n;

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

static int check_for_operation(char *cmd_line)
{
	int i;

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

int handle_normal_words(char **str, char *cmd_line, int *i)
{
	int start;

	start = *i;
	while (cmd_line[*i] && cmd_line[*i] != ' ' && cmd_line[*i] != '\t'
	&& !is_operator(cmd_line, *i))
		(*i)++;
	check_for_operation(cmd_line);
	*str = ft_malloc(*i - start + 1, 1);
	ft_strlcpy(*str, &cmd_line[start], *i - start + 1);
	return (1);
}

// int handle_word(char **str, char *cmd_line, int *i)
// {
// 	int start;
// 	int n = 0;
// 	char quote;

// 	start = *i;

// 	if (cmd_line[*i] && cmd_line[*i] != ' ' && cmd_line[*i] != '\t' && !is_operator(cmd_line, *i))
// 	{
// 		while (cmd_line[*i] && cmd_line[*i] != ' ' && cmd_line[*i] != '\t' && !is_operator(cmd_line, *i))
// 			(*i)++;
// 	}
// 	if (cmd_line[*i] == '\'' || cmd_line[*i] == '"')
// 	{
// 		quote = cmd_line[*i];
// 		start = *i;
// 		while (cmd_line[*i] == '\'' || cmd_line[*i] == '"')
// 			*i = skip_quotes(cmd_line, *i, &n);
// 	}
// 	if (cmd_line[*i] && cmd_line[*i] != ' ' && cmd_line[*i] != '\t' && !is_operator(cmd_line, *i))
// 	{
// 		while (cmd_line[*i] && cmd_line[*i] != ' ' && cmd_line[*i] != '\t' && !is_operator(cmd_line, *i))
// 			(*i)++;
// 	}

// 	*str = ft_malloc(*i - start + 1, 1);
// 	ft_strlcpy(*str, &cmd_line[start], *i - start + 1);
// 	return (1);
// }

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
static int handle_word(char **str, char *cmd_line, int *i)
{
	char	*tmp;
	int		start;

	tmp = NULL;
	while (cmd_line[*i] && cmd_line[*i] != ' ' && cmd_line[*i] != '\t' && !is_operator(cmd_line, *i))
	{
		if (cmd_line[*i] == '\'' || cmd_line[*i] == '"')
			tmp = ft_strjoin(tmp, handle_qouts(cmd_line, i));
		else
		{
			start = *i;
			while (cmd_line[*i] && cmd_line[*i] != ' ' && cmd_line[*i] != '\t'
				&& !is_operator(cmd_line, *i) && cmd_line[*i] != '\'' && cmd_line[*i] != '"')
				(*i)++;
			tmp = ft_strjoin(tmp, ft_substr(cmd_line, start, *i - start));
		}
	}
	*str = tmp;
	return (1);
}

int handle_operators(char **str, char *cmd_line, int *i, int count)
{
	*str = ft_malloc(1, count + 1);
	ft_strlcpy(*str, &cmd_line[*i], count + 1);
	*i += count;
	return (1);
}

char **help_lexer(char **str, char *cmd_line)
{
	int i;
	int j;

	i = 0;
	j = 0;
	while (cmd_line[i])
	{
		while (cmd_line[i] == ' ' || cmd_line[i] == '\t')
			i++;
		if (!cmd_line[i])
			break;
		if (is_operator(cmd_line, i))
		{
			j += handle_par(&str[j], cmd_line, &i);
			continue;
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

// int	check_parenthesis(char *cmd_line)
// {
// 	int	i;
// 	int	parenthesis;

// 	i = 0;
// 	parenthesis = 0;
// 	while (cmd_line[i])
// 	{
// 		if (cmd_line[i] == '(')
// 		{
// 			parenthesis++;
// 			i++;
// 			while (cmd_line[i] && cmd_line[i] != ')')
// 				i++;
// 			if (cmd_line[i] == ')')
// 			{
// 				parenthesis--;
// 				i++;
// 			}
// 		}
// 		else
// 			i++;
// 	}
// 	return (parenthesis == 0);
// }

char **lexer(char *cmd_line)
{
	char **str;
	int total;

	if (!check_quotes(cmd_line))
	{
		print_str_fd("ERROR:\nbalanced brakets.\n", 2);
		return (NULL);
	}
	total = count_words(cmd_line);
	str = ft_malloc(sizeof(char *), (total + 1));
	return (help_lexer(str, cmd_line));
}
