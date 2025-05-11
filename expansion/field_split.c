/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 13:32:15 by salahian          #+#    #+#             */
/*   Updated: 2025/05/11 15:07:45 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

int	is_separator(char c, char *sep)
{
	int	j;

	j = 0;
	while (sep[j])
	{
		if (c == sep[j])
			return (1);
		j++;
	}
	return (0);
}

int	count_word(char *str, char *sep)
{
	int	i;
	int	count;
	int	in_word;

	i = 0;
	count = 0;
	in_word = 0;
	while (str && str[i])
	{
		if (is_separator(str[i], sep))
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

int	count_alpha(char *str, char *sep)
{
	int	in_word;
	int	i;
	int	alpha;

	alpha = 0;
	in_word = 0;
	i = 0;
	while (str[i])
	{
		if (is_separator(str[i], sep))
			return (alpha);
		else
			alpha++;
		i++;
	}
	return (alpha);
}

//char	**allocate_string(char **s, char *str, char *charset, int count)
//{
//	int	i;
//	int	alpha;

//	i = 0;
//	alpha = 0;
//	while (i < count)
//	{
//		alpha = count_alpha(str, charset);
//		str += alpha + 1;
//		s[i] = ft_calloc(sizeof(char), (alpha + 1));
//		if (s[i] == NULL)
//			return (free_all(s), NULL);
//		i++;
//	}
//	s[i] = NULL;
//	return (s);
//}

char **allocate_string(char **s, char *str, char *charset)
{
	int	i = 0;
	int	alpha = 0;
	int	word = 0;
	int	in_word = 0;

	while (str[i])
	{
		if (is_separator(str[i], charset))
		{
			if (in_word)
			{
				s[word] = ft_calloc(sizeof(char), alpha + 1);
				if (!s[word])
					return (free_all(s), NULL);
				word++;
				alpha = 0;
				in_word = 0;
			}
		}
		else
		{
			alpha++;
			in_word = 1;
		}
		i++;
	}
	if (in_word)
	{
		s[word] = ft_calloc(sizeof(char), alpha + 1);
		if (!s[word])
			return (free_all(s), NULL);
		word++;
	}
	s[word] = NULL;
	return (s);
}


char	**concat(char **s, char *str, char *charset)
{
	int	word;
	int	d;
	int	i;

	word = 0;
	d = 0;
	i = 0;
	while (str && str[i])
	{
		if (is_separator(str[i], charset))
		{
			if (d > 0)
			{
				s[word][d] = '\0';
				word++;
				d = 0;
			}
		}
		else
			s[word][d++] = str[i];
		i++;
	}
	if (d > 0)
		s[word][d] = '\0';
	return (s);
}

char	**field_splitting(char *str, char *charset)
{
	char	**s;
	char	*tmp;
	int		count;

	count = count_word(str, charset);
	s = ft_calloc(sizeof(char *), (count + 1 + !count));
	if (s == NULL)
		return (NULL);
	if (!count)
	{
		s[0] = ft_strdup(str);
		if (!s[0])
			return (free_all(s), NULL);
		return (s[1] = NULL, s);
	}
	tmp = str;
	s = allocate_string(s, str, charset);
	if (!s)
		return (NULL);
	s = concat(s, tmp, charset);
	return (s);
}
