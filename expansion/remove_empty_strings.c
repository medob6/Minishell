/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_empty_strings.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 08:53:27 by salahian          #+#    #+#             */
/*   Updated: 2025/05/12 09:35:12 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

t_str	**remove_empty_string(t_array *child, int index)
{
	t_str	**new;
	size_t	i;
	int		j;

	i = 0;
	j = 0;
	new = ft_malloc(sizeof(t_str *), child->length + 1);
	while (i < child->length)
	{
		if (child->items[i] != child->items[index])
		{
			new[j] = ft_malloc(sizeof(t_str), 1);
			new[j]->value = ((t_str *)child->items[i])->value;
			new[j]->type = ((t_str *)child->items[i])->type;
			new[j]->fd = ((t_str *)child->items[i])->fd;
			j++;
		}
		i++;
	}
	new[j] = NULL;
	child->length--;
	return (new);
}

void	check_for_empty_strings(t_expansion *expand)
{
	size_t		i;
	char		**tmp;

	if (!expand->node->children)
		return ;
	i = 0;
	while (i < expand->node->children->length)
	{
		tmp = ((t_str *)expand->node->children->items[i])->value;
		if (tmp && tmp[0] == 0)
			expand->node->children->items = (void **)remove_empty_string(expand->node->children, i);
		i++;
	}
}

void	check_for_empty_strings_red(t_expansion *expand)
{
	size_t		i;
	char		**tmp;

	if (!expand->node->redirect_list)
		return ;
	i = 0;
	while (i < expand->node->redirect_list->length)
	{
		tmp = ((t_str *)expand->node->redirect_list->items[i])->value;
		if (tmp && tmp[0] == 0)
			((t_str *)expand->node->redirect_list->items[i])->value[0] = NULL;
		i++;
	}
}
