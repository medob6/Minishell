/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_empty_strings.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 08:53:27 by salahian          #+#    #+#             */
/*   Updated: 2025/05/01 11:11:15 by salahian         ###   ########.fr       */
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

void	check_for_empty_strings(t_ast_node *node)
{
	size_t		i;
	char		*tmp;

	if (!node->children)
		return ;
	i = 0;
	while (i < node->children->length)
	{
		tmp = (char *)node->children->items[i];
		if (tmp && tmp[0] == '\0')
			node->children->items = remove_empty_string(node->children, i);
		i++;
	}
}
