/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_empty_strings.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 08:53:27 by salahian          #+#    #+#             */
/*   Updated: 2025/05/06 16:01:50 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	**remove_empty_string(t_array *child, int index)
{
	void	**new;
	size_t	i;
	int		j;
	int		happend;

	i = 0;
	j = 0;
	happend = 0;
	new = ft_malloc(sizeof(void *), child->length);
	while (i < child->length)
	{
		if (child->items[i] != child->items[index])
		{
			happend = 1;
			new[j] = child->items[i];
			j++;
		}
		i++;
	}
	if (happend)
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
		if (tmp && *tmp[0] == '\0')
			expand->node->children->items = remove_empty_string(expand->node->children, i);
		i++;
	}
}
