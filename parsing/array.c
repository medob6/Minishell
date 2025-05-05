/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:59:16 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/05 18:06:36 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "array.h"

t_array	*creat_array(void)
{
	t_array	*new_array;

	new_array = ft_calloc(1, sizeof(t_array));
	new_array->capacity = 2;
	new_array->length = 0;
	new_array->items = ft_calloc(new_array->capacity + 1, sizeof(void *));
	return (new_array);
}

void	array_extend(t_array *arr)
{
	void	**new_items;
	size_t	i;

	arr->capacity *= 2;
	new_items = ft_calloc(arr->capacity + 1, sizeof(void *));
	i = 0;
	while (i < arr->length)
	{
		new_items[i] = arr->items[i];
		i++;
	}
	new_items[i] = NULL;
	// ft_free(arr->items);
	arr->items = new_items;
}

void	array_push(t_array **list, void *new_item)
{
	if (!*list)
		*list = creat_array();
	if ((*list)->length >= (*list)->capacity)
		array_extend(*list);
	(*list)->items[(*list)->length] = new_item;
	(*list)->length++;
	(*list)->items[(*list)->length] = NULL;
}
