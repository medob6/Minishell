/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lexem1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 12:17:06 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 14:46:51 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

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

int	skip_quotes(const char *s, int i, int *in_word)
{
	char	c;

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

void	help_count_words(char const *s, int *i, int *count, int *in_word)
{
	if (*in_word)
	{
		(*count)++;
		*in_word = 0;
	}
	if ((s[*i] == '|' && s[*i + 1] == '|') || (s[*i] == '&'
			&& s[*i + 1] == '&'))
		(*i)++;
	(*count)++;
	(*i)++;
}

void	check_if_in_word(int *in_word, int *count, int *i)
{
	if (*in_word)
	{
		(*count)++;
		*in_word = 0;
	}
	(*i)++;
}

int	count_words(char const *s)
{
	int	i;
	int	count;
	int	in_word;

	i = 0;
	count = 0;
	in_word = 0;
	while (s[i])
	{
		if (s[i] == ' ' || s[i] == '\t')
		{
			check_if_in_word(&in_word, &count, &i);
			continue ;
		}
		if (is_operator(s, i))
		{
			help_count_words(s, &i, &count, &in_word);
			continue ;
		}
		in_word = 1;
		i++;
	}
	if (in_word)
		count++;
	return (count);
}
