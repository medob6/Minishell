/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 14:41:15 by salahian          #+#    #+#             */
/*   Updated: 2025/03/13 14:28:20 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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


size_t ft_strlcpy(char *dst, const char *src, int start, int end)
{
	size_t i = 0;
	while (start < end && src[start])
		dst[i++] = src[start++];
	dst[i] = '\0';
	return i;
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

char	**help_lexer(char **str, char *cmd_line)
{
	int	i;
	int	start;
	int	j;

	i = 0;
	start = 0;
	j = 0;
	while (cmd_line[i])
	{
		while (cmd_line[i] == ' ' || cmd_line[i] == '\t')
			i++;
		if (!cmd_line[i])
			break;
		start = i;
		if (cmd_line[i] == '\'' || cmd_line[i] == '"')
			i = get_next_qout(cmd_line, i);
		while (cmd_line[i] && cmd_line[i] != ' ' && cmd_line[i] != '\t')
			i++;

		str[j] = malloc(i - start + 2);
		ft_strlcpy(str[j], cmd_line, start, i);
		j++;
	}
	str[j] = NULL;
	return (str);
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
	total = count_words(cmd_line);
	str = malloc(sizeof(char *) * (total + 1));
	return (help_lexer(str, cmd_line));
}

// int main(void)
// {
// 	char **s = lexer("                  'hello    ' i   am      here                         ");
// 	int i = 0;
// 	while (s[i])
// 	{
// 		printf("token[%d]: [%s]\n", i, s[i]);
// 		free(s[i]);
// 		i++;
// 	}
// 	free(s);
// 	return 0;
// }
