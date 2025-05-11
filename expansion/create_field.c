/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_field.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 14:14:20 by salahian          #+#    #+#             */
/*   Updated: 2025/05/11 18:32:43 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

void	handle_what_in_double(char **field, char **items, size_t index, int *j)
{
	field[index][*j] = '0';
	(*j)++;
	while (items[index][*j] && items[index][*j] != '"')
	{
		field[index][*j] = '4';
		(*j)++;
	}
	if (items[index][*j] == '"')
	{
		field[index][*j] = '0';
		(*j)++;
	}
}
void	handle_what_in_single(char **field, char **items, size_t index, int *j)
{
	field[index][*j] = '0';
	(*j)++;
	while (items[index][*j] && items[index][*j] != '\'')
	{
		field[index][*j] = '2';
		(*j)++;
	}
	if (items[index][*j] == '\'')
	{
		field[index][*j] = '0';
		(*j)++;
	}
}

void	handle_what_in_double_red(char **field, char **items, size_t index, int *j)
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
void	handle_what_in_single_red(char **field, char **items, size_t index, int *j)
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

void	create_single_field(char **field, char **items, size_t index)
{
	int		j;
	
	j = 0;
	field[index] = ft_malloc(sizeof(char), ft_strlen(items[index]) + 1);
	while (items[index][j])
	{
		if (items[index][j] == '"')
			handle_what_in_double(field, items, index, &j);
		else if (items[index][j] == '\'')
			handle_what_in_single(field, items, index, &j);
		else
		{
			field[index][j] = '0';
			if (items[index][j])
				j++;
		}
	}
	field[index][j] = '\0';
}

void	create_single_field_red(char **field, char **items, size_t index)
{
	int		j;
	
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

char **create_field(t_ast_node *node)
{
	char **field;
	size_t		i;

	i = 0;
	field = ft_malloc(sizeof(char *), node->children->length + 1);
	while (i < node->children->length)
	{
		create_single_field(field, (char **)node->children->items, i);
		i++;
	}
	//printf("%zu\n", i);
	field[i] = NULL;
	return (field);
}

char **create_field_red(t_ast_node *node)
{
	char **field;
	size_t		i;

	i = 0;
	field = ft_malloc(sizeof(char *), node->redirect_list->length + 1);
	while (i < node->redirect_list->length)
	{
		create_single_field_red(field, (char **)(((t_token *)node->redirect_list->items)->value.str_value), i);
		i++;
	}
	//printf("%zu\n", i);
	field[i] = NULL;
	return (field);
}
