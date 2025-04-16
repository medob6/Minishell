/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexem.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:41:15 by salahian          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/04/09 15:10:13 by salahian         ###   ########.fr       */
=======
/*   Updated: 2025/04/15 16:06:27 by salahian         ###   ########.fr       */
>>>>>>> b83116466f80f973f0d96241032b11d51b679a10
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void    ft_putstr(char *s, int fd)
// {
//     write(fd, s, ft_strlen(s));
// }

<<<<<<< HEAD
int is_operator(const char *c, int *i)
{
	return (c[*i] == '&' || c[*i] == '|' ||
	c[*i] == '<' || c[*i] == '>'
	|| c[*i] == '$' || c[*i] == '*' || c[*i] == '(' || c[*i] == ')');
=======
int	is_operator(const char *c, int *i)
{
	return (c[*i] == '&' || c[*i] == '|' || c[*i] == '<' || c[*i] == '>'
		|| c[*i] == '*' || c[*i] == '(' || c[*i] == ')');
>>>>>>> b83116466f80f973f0d96241032b11d51b679a10
}

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

int	skip_quotes(const char *s, int i, int *in_word)
{
	int	q;

	if (s[i] == '\'' || s[i] == '"')
	{
		q = get_next_qout((char *)s, i);
		*in_word = 1;
		if (q > i)
			return (q);
	}
	return (i);
}

int	count_words(char const *s)
{
	int	i;
<<<<<<< HEAD
	int count;
    int q;
    int in_word;
=======
	int	count;
	int	in_word;
>>>>>>> b83116466f80f973f0d96241032b11d51b679a10

	i = 0;
	count = 0;
	in_word = 0;
	while (s[i])
	{
		i = skip_quotes(s, i, &in_word);
		if (is_operator(s, &i))
			count++;
		if ((s[i] == ' ' || s[i] == '\t' || is_operator(s, &i)) && in_word)
		{
<<<<<<< HEAD
			q = get_next_qout((char *)s, i);
			if (q > i)
				i = q;
			in_word = 1;
		}
        if (is_operator(s, &i))
            count++;
		if (s[i] == ' ' || s[i] == '\t' || is_operator(s, &i))
		{
			if (in_word)
			{
				count++;
				in_word = 0;
			}
=======
			count++;
			in_word = 0;
>>>>>>> b83116466f80f973f0d96241032b11d51b679a10
		}
		else
			in_word = 1;
		i++;
	}
	if (in_word)
		count++;
	return (count);
}

int	handle_par(char **str, char *c, int *i)
{
	(*i)++;
	if (is_operator(c, i))
	{
		(*i)--;
		*str = ft_malloc(1, 3);
		(*str)[0] = c[*i];
		(*str)[1] = c[*i];
		(*str)[2] = '\0';
		(*i) += 2;
		return (1);
	}
	(*i)--;
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

	quote = cmd_line[*i];
	start = *i;
	(*i)++;
	while (cmd_line[*i] && cmd_line[*i] != quote)
		(*i)++;
	if (cmd_line[*i] == quote)
		(*i)++;
	if (cmd_line[*i] != '\0' && cmd_line[*i + 1] == quote)
	{
		(*i)++;
		while (cmd_line[*i] && cmd_line[*i] != quote)
			(*i)++;
		if (cmd_line[*i] == quote)
			(*i)++;
	}
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
<<<<<<< HEAD
			return ((cmd_line[i + 1] == '|') ? 2 : 1);
		else if (cmd_line[i] == '&')
			return ((cmd_line[i + 1] == '&') ? 2 : 0);
		else if (cmd_line[i] == '>')
			return ((cmd_line[i + 1] == '>') ? 2 : 1);
		else if (cmd_line[i] == '<')
			return ((cmd_line[i + 1] == '<') ? 2 : 1);
		else if (cmd_line[i] == '$')
			return (1);
		else if (cmd_line[i] == '*')
			return (1);
		else if (cmd_line[i] == '(')
			return (1);
		else if (cmd_line[i] == ')')
=======
			return ((cmd_line[i + 1] == '|') + 1);
		else if (cmd_line[i] == '&')
		{
			if (cmd_line[i + 1] == '&')
				return (2);
			return (0);
		}
		else if (cmd_line[i] == '>')
			return ((cmd_line[i + 1] == '>') + 1);
		else if (cmd_line[i] == '<')
			return ((cmd_line[i + 1] == '<') + 1);
		else if (cmd_line[i] == '*' || cmd_line[i] == '(' || cmd_line[i] == ')')
>>>>>>> b83116466f80f973f0d96241032b11d51b679a10
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
		&& !is_operator(cmd_line, i))
		(*i)++;
	check_for_operation(cmd_line);
	*str = ft_malloc(*i - start + 1, 1);
	ft_strlcpy(*str, &cmd_line[start], *i - start + 1);
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
		if (is_operator(cmd_line, &i))
		{
			j += handle_par(&str[j], cmd_line, &i);
			continue ;
		}
		if (cmd_line[i] == '\'' || cmd_line[i] == '"')
		{
			j += handle_in_the_qouts(&str[j], cmd_line, &i);
			continue ;
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
		ft_print("ERROR:\nbalanced brakets.\n", 2);
		return (NULL);
	}
	if (!check_parenthesis(cmd_line))
	{
		ft_print("ERROR:\nbalanced paronthesis.\n", 2);
		return (NULL);
	}
	total = count_words(cmd_line);
	str = ft_malloc(sizeof(char *), (total + 1));
	return (help_lexer(str, cmd_line));
}

// int main(void)
// {
<<<<<<< HEAD
// 	char **s = lexer("echo hello&&wc -c");
=======
// 	char **s = lexer("$ls");
>>>>>>> b83116466f80f973f0d96241032b11d51b679a10
// 	int i = 0;
// 	while (s[i])
// 	{
// 		printf("lexer[%d]: [%s]\n", i, s[i]);
// 		i++;
// 	}
// 	return (0);
// }
