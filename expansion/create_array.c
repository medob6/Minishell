/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_array.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 14:17:31 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 14:18:57 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

static char	**split_new(char *str, int *count, int flag)
{
	char	**words;

	words = NULL;
	if (flag)
	{
		*count = 0;
		words = ft_split(str, ' ');
		while (words && words[*count])
			(*count)++;
	}
	else
		*count = 1;
	return (words);
}

static char	**append_old_arr(char **old, int *j, int i, int count)
{
	char	**new;

	new = ft_calloc(sizeof(char *), i + count + 1);
	while (old && old[*j])
	{
		new[*j] = ft_strdup(old[*j]);
		(*j)++;
	}
	return (new);
}

char	**append_to_arr(char **old, char *str, int flag)
{
	int		i;
	int		j;
	int		count;
	char	**new;
	char	**words;

	i = 0;
	while (old && old[i])
		i++;
	count = 0;
	words = split_new(str, &count, flag);
	new = ft_calloc(sizeof(char *), i + count + 1);
	j = 0;
	new = append_old_arr(old, &j, i, count);
	if (flag)
	{
		i = 0;
		while (words && words[i])
			new[j++] = ft_strdup(words[i++]);
	}
	else
		new[j++] = ft_strdup(str);
	new[j] = NULL;
	return (new);
}

char	**expand_string(char **new, char **field, char **str, int j)
{
	char	*exp;
	char	*old_field;
	char	**arr;

	arr = NULL;
	old_field = *field;
	exp = expand_wildcard(field, str[j]);
	if (exp)
	{
		arr = append_to_arr(new, exp, 1);
		create_new_field(field, str, j, exp);
	}
	else
	{
		*field = old_field;
		arr = append_to_arr(new, str[j], 0);
	}
	return (arr);
}

void	call_expand_wildcard(t_array *child, char **field, int i)
{
	char	**str;
	char	**new;
	int		j;

	str = ((t_str *)child->items[i])->value;
	new = ft_calloc(sizeof(char *), get_len(str) + 1);
	j = 0;
	while (str && str[j])
	{
		if (search_for(str[j], '*'))
			new = expand_string(new, &field[i], str, j);
		else
			new = append_to_arr(new, str[j], 0);
		j++;
	}
	((t_str *)child->items[i])->value = new;
}
