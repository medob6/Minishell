/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_splitting.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:21:24 by salahian          #+#    #+#             */
/*   Updated: 2025/05/15 18:27:56 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

int	check_first_char(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_');
}

int	count_nbr_words(char *str, char *field)
{
	int	i;
	int	in_word;
	int	count;

	i = 0;
	in_word = 0;
	count = 0;
	while (str[i])
	{
		if ((str[i] != ' ' && str[i] != '\t') || field[i] != '1')
		{
			if (!in_word)
			{
				in_word = 1;
				count++;
			}
		}
		else
			in_word = 0;
		i++;
	}
	return (count);
}

static char	*allocat(const char *s, char *field, int *j)
{
	int		start;
	int		len;
	int		i;
	char	*w;

	while (s[*j] && (s[*j] == ' ' || s[*j] == '\t') && field[*j] == '1')
		(*j)++;
	start = *j;
	while (s[*j] && ((s[*j] != ' ' && s[*j] != '\t') || field[*j] != '1'))
		(*j)++;
	len = *j - start;
	w = ft_malloc(len + 1, sizeof(char));
	if (!w)
		return (NULL);
	i = 0;
	while (i < len)
	{
		w[i] = s[start + i];
		i++;
	}
	w[len] = '\0';
	return (w);
}

char	**applicate_splitting(char *str, char *field)
{
	int		count;
	char	**new;
	int		j;
	int		i;

	count = count_nbr_words(str, field);
	new = ft_calloc(count + 1, sizeof(char *));
	j = 0;
	i = 0;
	while (i < count)
	{
		new[i] = allocat(str, field, &j);
		if (!new[i])
			return (NULL);
		i++;
	}
	new[i] = NULL;
	return (new);
}

int	skip_single_quotes(const char *s, int i)
{
	char	c;

	if (ft_strchr("'", s[i]) != NULL)
	{
		c = s[i];
		i++;
		while (s[i] && s[i] != c)
			i++;
		if (s[i] == c)
			i++;
	}
	return (i);
}

int	check_for_field_split(char *tmp)
{
	int	i;
	int	dollar;

	i = 0;
	dollar = 0;
	while (tmp[i])
	{
		if (tmp[i] == '\'')
			i = skip_single_quotes(tmp, i);
		if (tmp[i] == '"')
		{
			i++;
			while (tmp[i] && tmp[i] != '"')
			{
				if (tmp[i] == '$')
					return (1);
				i++;
			}
		}
		if (tmp[i] == '$')
			dollar = 1;
		if (tmp[i])
			i++;
	}
	return (dollar);
}
