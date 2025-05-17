/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_field1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 14:32:19 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 14:40:50 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

char	*handle_what_in_single_qout(char *str, int *index)
{
	char	*tmp;
	int		start;

	start = *index;
	(*index)++;
	while (str[*index] && str[*index] != '\'')
		(*index)++;
	if (str[*index] == '\'')
		(*index)++;
	tmp = ft_substr(str, start, *index - start);
	return (tmp);
}

char	*update_field(char **field, char *tmp, int index)
{
	int		i;
	char	*old_new;

	old_new = ft_malloc(sizeof(char), ft_strlen(tmp) + 1);
	i = 0;
	while (tmp[i])
	{
		old_new[i] = (*field)[index];
		i++;
		index++;
	}
	old_new[i] = '\0';
	return (old_new);
}

char	*update_field_exp(char **field, char *exp, int index)
{
	int		i;
	char	*tmp;

	tmp = ft_malloc(1, ft_strlen(exp) + 1);
	i = 0;
	while (exp[i])
	{
		if ((*field)[index] == '0')
			tmp[i] = '1';
		else
			tmp[i] = '5';
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}

char	*copy_and_update(char **field, char *str, int *index, char **new)
{
	int		start;
	char	*tmp;

	start = *index;
	tmp = append(str[(*index)++]);
	*new = ft_strjoin(*new, update_field(field, tmp, start));
	return (tmp);
}

char	*update_field_after_splitting(char *field, char *copy_exp)
{
	char	*new;
	int		i;
	int		j;
	int		count;

	i = 0;
	count = 0;
	while (copy_exp[i])
	{
		if ((copy_exp[i] == ' ' || copy_exp[i] == '\t') && field[i] == '1')
			count++;
		i++;
	}
	new = ft_calloc(sizeof(char), (ft_strlen(field) - count) + 1);
	j = 0;
	i = 0;
	while (copy_exp[i])
	{
		if ((copy_exp[i] == ' ' || copy_exp[i] == '\t') && field[i] == '1')
			i++;
		else
			new[j++] = field[i++];
	}
	new[j] = '\0';
	return (new);
}
