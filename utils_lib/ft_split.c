/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 16:19:12 by mbousset          #+#    #+#             */
/*   Updated: 2025/03/01 18:07:25 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(const char *str, char c)
{
	int	i;
	int	counter;
	int	in_word;

	i = 0;
	counter = 0;
	in_word = 0;
	while (str[i])
	{
		if ((str[i] != c) && !in_word)
		{
			in_word = 1;
			counter++;
		}
		else if (str[i] == c)
		{
			in_word = 0;
		}
		i++;
	}
	return (counter);
}

char	*fill_word(const char *src, int start, int end)
{
	char	*res;
	int		i;

	res = (char *)malloc(sizeof(char) * (end - start + 1));
	if (res == NULL)
		return (NULL);
	i = 0;
	while (i < (end - start))
	{
		res[i] = src[start + i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

void	free_all(char **p)
{
	int	i;

	i = 0;
	if (!p)
		return ;
	while (p[i])
	{
		free(p[i]);
		i++;
	}
	free(p);
}

static char	**fill_arr(char **arr, const char *str, char c)
{
	int	start;
	int	i;
	int	k;

	i = 0;
	k = 0;
	while (str[i])
	{
		while (str[i] && (str[i] == c))
			i++;
		start = i;
		while (str[i] && (str[i] != c))
			i++;
		if (start < i)
		{
			arr[k] = fill_word(str, start, i);
			if (arr[k++] == NULL)
			{
				free_all(arr);
				return (NULL);
			}
		}
	}
	arr[k] = NULL;
	return (arr);
}

char	**ft_split(const char *str, char c)
{
	char	**arr;

	arr = (char **)malloc(sizeof(char *) * (count_words(str, c) + 1));
	if (!arr)
		return (NULL);
	arr = fill_arr(arr, str, c);
	return (arr);
}
