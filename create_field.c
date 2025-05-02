/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_field.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 14:14:20 by salahian          #+#    #+#             */
/*   Updated: 2025/05/01 15:22:23 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_what_in_double(t_bit_mask **field, char **items, size_t index, int *j)
{
	field[index][*j] = ORIGINAL;
	(*j)++;
	while (items[index][*j] && items[index][*j] != '"')
	{
		field[index][*j] = DOUBLE_QOUT;
		(*j)++;
	}
	if (items[index][*j] == '"')
	{
		field[index][*j] = ORIGINAL;
		(*j)++;
	}
}
void	handle_what_in_single(t_bit_mask **field, char **items, size_t index, int *j)
{
	field[index][*j] = ORIGINAL;
	(*j)++;
	while (items[index][*j] && items[index][*j] != '\'')
	{
		field[index][*j] = SINGLE_QOUT;
		(*j)++;
	}
	if (items[index][*j] == '\'')
	{
		field[index][*j] = ORIGINAL;
		(*j)++;
	}
}

void	create_single_field(t_bit_mask **field, char **items, size_t index)
{
	int		j;
	
	j = 0;
	field[index] = ft_malloc(sizeof(t_bit_mask), ft_strlen(items[index]) + 1);
	while (items[index][j])
	{
		if (items[index][j] == '"')
			handle_what_in_double(field, items, index, &j);
		else if (items[index][j] == '\'')
			handle_what_in_single(field, items, index, &j);
		else
		{
			field[index][j] = ORIGINAL;
			if (items[index][j])
				j++;
		}
	}
	field[index][j] = ORIGINAL;
}

t_bit_mask **create_field(t_ast_node *node)
{
	t_bit_mask **field;
	size_t		i;

	i = 0;
	field = ft_malloc(sizeof(t_bit_mask *), node->children->length);
	while (i < node->children->length)
	{
		create_single_field(field, (char **)node->children->items, i);
		i++;
	}
	field[i] = NULL;
	return (field);
}
