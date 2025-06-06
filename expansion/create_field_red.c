/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_field_red.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 12:43:37 by salahian          #+#    #+#             */
/*   Updated: 2025/05/18 11:01:56 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

int	check_for_next_one(char *str, int j)
{
	char	c;

	c = str[j];
	j++;
	while (str[j])
	{
		if (str[j] == c)
			return (1);
		j++;
	}
	return (0);
}

void	handle_what_in_double_red(char **field, char **items, size_t index,
		int *j)
{
	field[index][*j] = '0';
	(*j)++;
	while (items[0][*j] && items[0][*j] != '"')
	{
		field[index][*j] = '4';
		(*j)++;
	}
	if (items[0][*j] == '"')
	{
		field[index][*j] = '0';
		(*j)++;
	}
}

void	handle_what_in_single_red(char **field, char **items, size_t index,
		int *j)
{
	field[index][*j] = '0';
	(*j)++;
	while (items[0][*j] && items[0][*j] != '\'')
	{
		field[index][*j] = '2';
		(*j)++;
	}
	if (items[0][*j] == '\'')
	{
		field[index][*j] = '0';
		(*j)++;
	}
}

void	create_single_field_red(char **field, char **items, size_t index)
{
	int	j;

	j = 0;
	field[index] = ft_malloc(sizeof(char), ft_strlen(items[0]) + 1);
	while (items[0][j])
	{
		if (items[0][j] == '"')
			handle_what_in_double_red(field, items, index, &j);
		else if (items[0][j] == '\'')
			handle_what_in_single_red(field, items, index, &j);
		else
		{
			field[index][j] = '0';
			if (items[0][j])
				j++;
		}
	}
	field[index][j] = '\0';
}

char	**create_field_red(t_ast_node *node)
{
	char	**field;
	size_t	i;
	t_token	*token;
	char	*value;
	char	*arr[2];

	i = 0;
	field = ft_malloc(sizeof(char *), node->redirect_list->length + 1);
	while (i < node->redirect_list->length)
	{
		token = (t_token *)node->redirect_list->items[i];
		value = token->value.str_value;
		arr[0] = value;
		arr[1] = NULL;
		create_single_field_red(field, arr, i);
		i++;
	}
	field[i] = NULL;
	return (field);
}
