/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_variables.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 14:26:56 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 14:39:20 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

int	check_is_valid_split(char *tmp)
{
	int	i;

	i = 0;
	while (tmp && tmp[i])
	{
		if (tmp[i] == '$')
		{
			if (!valid(tmp[i + 1]))
				return (0);
		}
		i++;
	}
	return (1);
}

int	valid(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_');
}

int	is_valid_length(char *tmp, int flag)
{
	int	i;

	i = flag;
	if (!flag && !valid(tmp[i]))
		return (0);
	i = !(flag);
	while (tmp[i])
	{
		if (!((tmp[i] >= '0' && tmp[i] <= '9') || (tmp[i] >= 'A'
					&& tmp[i] <= 'Z') || (tmp[i] >= 'a' && tmp[i] <= 'z')
				|| tmp[i] == '_'))
			break ;
		i++;
	}
	return (i);
}

static char	*help_expand_the_value(char *str, char *value, int var_len)
{
	char	next;

	next = str[var_len];
	if (next == '\0' || !(next == '_' || (next >= '0' && next <= '9')
			|| (next >= 'a' && next <= 'z') || (next >= 'A' && next <= 'Z')))
	{
		if (str[var_len] != '\0')
			str = ft_strjoin(ft_strdup(value), &str[var_len]);
		else
			str = ft_strdup(value);
		return (str);
	}
	return (NULL);
}

char	*expand_the_value(char *str, t_env **env)
{
	t_env	*tmp;
	int		var_len;
	int		index;
	char	*old_str;

	old_str = str;
	str = &str[1];
	tmp = *env;
	while (tmp)
	{
		var_len = ft_strlen(tmp->key);
		if (ft_strncmp(str, tmp->key, var_len) == 0)
		{
			if (help_expand_the_value(str, tmp->value, var_len))
				return (help_expand_the_value(str, tmp->value, var_len));
		}
		tmp = tmp->next;
	}
	index = is_valid_length(str, 1);
	if (index)
		str = ft_strdup(&str[index]);
	else
		str = NULL;
	return (str);
}
