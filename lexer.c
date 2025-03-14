/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:41:15 by salahian          #+#    #+#             */
/*   Updated: 2025/03/14 15:57:57 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlen(const char *s)
{
	size_t	n;

	n = 0;
	while (s && *s != '\0')
	{
		n++;
		s++;
	}
	return (n);
}

void    ft_putstr(char *s, int fd)
{
    write(fd, s, ft_strlen(s));
}

int check_single_quotes(char *cmd_line)
{
	int i;
	int single_qout;

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

int check_double_quotes(char *cmd_line)
{
	int i;
	int double_qout;

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

int check_quotes(char *cmd_line)
{
	return (check_single_quotes(cmd_line) && check_double_quotes(cmd_line));
}


// size_t ft_strlcpy(char *dst, const char *src, int start, int end)
// {
// 	size_t i = 0;
// 	while (start < end && src[start])
// 		dst[i++] = src[start++];
// 	dst[i] = '\0';
// 	return i;
// }

size_t  ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
        size_t  count;
        size_t  i;

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

int get_next_qout(char *cmd_line, int i)
{
	int j = i + 1;
	char c = cmd_line[i];
	while (cmd_line[j])
	{
		if (cmd_line[j] == c)
			return j;
		j++;
	}
	return i;
}

int count_words(char const *s)
{
	int i = 0, count = 0, in_word = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			int q = get_next_qout((char *)s, i);
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
	return count;
}

// char	**help_lexer(char **str, char *cmd_line)
// {
// 	int	i;
// 	int	start;
// 	int	j;

// 	i = 0;
// 	start = 0;
// 	j = 0;
// 	while (cmd_line[i])
// 	{
// 		while (cmd_line[i] == ' ' || cmd_line[i] == '\t')
// 			i++;
// 		if (!cmd_line[i])
// 			break;
// 		if (cmd_line[i] == '(')
// 			i += handle_par(str, cmd_line[i], &j);
// 		start = i;
// 		if (cmd_line[i] == '\'' || cmd_line[i] == '"')
// 			i = get_next_qout(cmd_line, i);
// 		while (cmd_line[i] && cmd_line[i] != ' ' && cmd_line[i] != '\t')
// 			i++;
// 		str[j] = malloc(i - start + 2);
// 		ft_strlcpy(str[j], cmd_line, start, i);
// 		if (str[j][ft_strlen(str[j]) - 1] == ')')
// 		{
// 			str[j][ft_strlen(str[j]) - 1] = '\0';
// 			j++;
// 			handle_par(str, cmd_line[i], &j);
// 		}
// 		j++;
// 	}
// 	str[j] = NULL;
// 	return (str);
// }

int	handle_par(char **str, char c, int *j)
{
	str[*j] = malloc(2);
	str[*j][0] = c;
	str[*j][1] = '\0';
	(*j)++;
	return (1);
}

char	**help_lexer(char **str, char *cmd_line)
{
	int	i = 0;
	int	start = 0;
	int	j = 0;

	while (cmd_line[i])
	{
		while (cmd_line[i] == ' ' || cmd_line[i] == '\t')
			i++;
		if (!cmd_line[i])
			break;
		if (cmd_line[i] == '(')
		{
			handle_par(str, '(', &j);
			i++;
		}
		start = i;
		if (cmd_line[i] == '\'' || cmd_line[i] == '"')
			i = get_next_qout(cmd_line, i);
		while (cmd_line[i] && cmd_line[i] != ' ' && cmd_line[i] != '\t' && cmd_line[i] != ')')
			i++;
		str[j] = malloc(i - start + 1);
		ft_strlcpy(str[j], &cmd_line[start], i - start + 1);
		j++;
		if (cmd_line[i] == ')')
		{
			handle_par(str, ')', &j);
			i++;
		}
	}
	str[j] = NULL;
	return (str);
}


int check_parenthesis(char *cmd_line)
{
	int i;
	int parenthesis;

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

char **lexer(char *cmd_line)
{
	char **str;
	int total;

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
	str = malloc(sizeof(char *) * (total + 1));
	return (help_lexer(str, cmd_line));
}

int main(void)
{
	char **s = lexer("(echo 'hello   world' | wc -c | ls *.c)");
	int i = 0;
	while (s[i])
	{
		printf("lexer[%d]: [%s]\n", i, s[i]);
		i++;
	}
	return 0;
}
