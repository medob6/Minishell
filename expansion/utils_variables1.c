/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_variables1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 14:28:14 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 14:31:22 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

char	*append(char s)
{
	char	buf[2];

	buf[0] = s;
	buf[1] = '\0';
	return (ft_strdup(buf));
}

char	*append_char(char *old_str, char c)
{
	char	buf[2];
	char	*tmp;

	buf[0] = c;
	buf[1] = '\0';
	tmp = ft_strjoin(old_str, buf);
	return (tmp);
}

int	check_if_qouted(char *str, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		if (str[i] == '"')
			return (0);
		i++;
	}
	return (1);
}

int	search_for(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*handle_expansion(t_env **env, char *str, int *index)
{
	int		var_len;
	char	*tmp;
	char	*exp;
	char	*var;
	char	buf[2];

	var_len = is_valid_length(&str[*index + 1], 0);
	if (var_len == 0)
	{
		if (str[*index + 1] == '?')
		{
			tmp = ft_itoa(*get_last_status());
			(*index) += 2;
			return (tmp);
		}
		buf[0] = '$';
		buf[1] = '\0';
		tmp = ft_strdup(buf);
		(*index)++;
		return (tmp);
	}
	var = ft_substr(str, *index, var_len + 1);
	exp = expand_the_value(var, env);
	*index += var_len + 1;
	return (exp);
}
