/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_copy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 10:07:25 by salahian          #+#    #+#             */
/*   Updated: 2025/05/01 10:34:26 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	copy_string(char **new, char **org, size_t i)
{
	int		j;
	
	j = 0;
	new[i] = ft_malloc(sizeof(char), ft_strlen(org[i]));
	while (org[i][j])
	{
		new[i][j] = org[i][j];
		j++;
	}
	new[i][j] = '\0';
}

void	**create_copy(t_ast_node *node)
{
	size_t	i;
	void	**copy_items;
	
	if (!node->children)
		return (NULL);
	i = 0;
	copy_items = ft_malloc(sizeof(void *), node->children->length);
	while (i < node->children->length)
	{
		copy_string((char **)copy_items, (char **)node->children->items, i);
		i++;
	}
	copy_items[i] = NULL;
	return (copy_items);
}