/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   removes_qouts.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:17:31 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 14:43:14 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

int	take_inside_qouts(char **s, char *field, char *str, int j)
{
	char	c;
	int		i;

	c = str[j];
	j++;
	i = 1;
	while (str[j] && (str[j] != c && field[i] != '0'))
	{
		*s = append_char(*s, str[j]);
		i++;
		j++;
	}
	if (str[j] == c && field[i] == '0')
		j++;
	return (j);
}

char	*remove_q(char **field, char *str, int *index, size_t i)
{
	char	*new_str;
	int		j;
	int		start;

	j = 0;
	start = 0;
	new_str = ft_strdup("");
	while (str[j])
	{
		if ((str[j] == '\'' || str[j] == '"') && field[i][*index] == '0')
		{
			start = j;
			j = take_inside_qouts(&new_str, &field[i][*index], str, j);
			*index += j - start;
		}
		else
		{
			new_str = append_char(new_str, str[j++]);
			(*index)++;
		}
	}
	return (new_str);
}

void	removes_qouts_cmd(t_expansion *expand)
{
	size_t	i;
	char	**tmp;
	int		j;
	int		index;

	if (!expand->node->children)
		return ;
	i = 0;
	while (i < expand->node->children->length)
	{
		tmp = ((t_str *)expand->node->children->items[i])->value;
		if (tmp && *tmp)
		{
			j = 0;
			index = 0;
			while (tmp && tmp[j])
			{
				tmp[j] = remove_q(expand->field_cmd, tmp[j], &index, i);
				j++;
			}
		}
		((t_str *)expand->node->children->items[i])->value = tmp;
		i++;
	}
}

void	help_removes_qouts_red(t_expansion *expand, size_t i)
{
	char	**tmp;
	int		j;
	int		index;

	tmp = ((t_str *)expand->node->redirect_list->items[i])->value;
	if (tmp && *tmp)
	{
		j = 0;
		index = 0;
		while (tmp && tmp[j])
		{
			tmp[j] = remove_q(expand->field_red, tmp[j], &index, i);
			j++;
		}
	}
	((t_str *)expand->node->redirect_list->items[i])->value = tmp;
}

void	removes_qouts_red(t_expansion *expand)
{
	size_t			i;
	t_token_type	type;

	if (!expand->node->redirect_list)
		return ;
	i = 0;
	while (i < expand->node->redirect_list->length)
	{
		type = ((t_str *)expand->node->redirect_list->items[i])->type;
		if (type == TOKEN_HEREDOC)
		{
			i++;
			continue ;
		}
		help_removes_qouts_red(expand, i);
		i++;
	}
}
