/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_path_name.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 14:11:04 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 14:11:54 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

int	find_last_slash(char *str)
{
	int	j;
	int	last_slash;

	j = 0;
	last_slash = -1;
	while (str[j])
	{
		if (str[j] == '/')
		{
			last_slash = j;
			break ;
		}
		j++;
	}
	return (last_slash);
}

int	check_str(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != '*')
			return (0);
		i++;
	}
	return (1);
}

int	skip_wildcards(char **pattern, char **field)
{
	while (**pattern == '*' && (**field == '0' || **field == '1'))
	{
		(*pattern)++;
		(*field)++;
	}
	return (**pattern == '\0');
}

int	try_match_rest(char *field, char *pattern, char *name)
{
	while (*name)
	{
		if (match_pattern(field, pattern, name))
			return (1);
		name++;
	}
	return (0);
}

int	match_pattern(char *field, char *pattern, char *name)
{
	while (*pattern)
	{
		if (*pattern == '*' && (*field == '0' || *field == '1'))
		{
			if (skip_wildcards(&pattern, &field))
				return (1);
			return (try_match_rest(field, pattern, name));
		}
		else if (*name && *pattern == *name)
		{
			field++;
			pattern++;
			name++;
		}
		else
			return (0);
	}
	return (*name == '\0');
}
