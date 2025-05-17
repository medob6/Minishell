/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_field.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 14:19:42 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 14:21:20 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

char	*take_before_wildcard(char *field, char **org, int j, int *i)
{
	int		len;
	int		k;
	char	*res;

	len = 0;
	k = 0;
	while (k < j)
	{
		len += ft_strlen(org[k]);
		k++;
	}
	*i = len;
	res = ft_malloc(len + 1, 1);
	if (!res)
		return (NULL);
	ft_strlcpy(res, field, len + 1);
	return (res);
}

int	get_new_len(const char *exp)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (exp && exp[i])
	{
		if (exp[i] != ' ')
			count++;
		i++;
	}
	return (count);
}

char	*update_new(int len)
{
	char	*res;
	int		i;

	res = ft_malloc(1, len + 1);
	i = 0;
	while (i < len)
	{
		res[i] = '6';
		i++;
	}
	res[len] = '\0';
	return (res);
}

static char	*take_after_wildcard(char *field, char **org, int j, int *i)
{
	int		offset;
	int		total_len;
	char	*res;

	offset = *i + ft_strlen(org[j]);
	total_len = ft_strlen(field);
	if (offset >= total_len)
		return (ft_strdup(""));
	res = ft_malloc(total_len - offset + 1, 1);
	if (!res)
		return (NULL);
	ft_strlcpy(res, field + offset, total_len - offset + 1);
	return (res);
}

void	create_new_field(char **field, char **org, int j, char *exp)
{
	char	*before;
	char	*middle;
	char	*after;
	char	*new;
	int		i;

	i = 0;
	before = take_before_wildcard(*field, org, j, &i);
	middle = update_new(get_new_len(exp));
	after = take_after_wildcard(*field, org, j, &i);
	new = ft_strjoin(before, middle);
	before = new;
	new = ft_strjoin(before, after);
	*field = new;
}
