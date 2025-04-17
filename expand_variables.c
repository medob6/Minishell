/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:37:47 by salahian          #+#    #+#             */
/*   Updated: 2025/04/17 16:40:11 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     ft_strcmp(const char *s1, const char *s2)
{
        size_t  i;

        i = 0;
        while ((s1[i] || s2[i]))
        {
                if (s1[i] != s2[i])
                        return ((unsigned char)s1[i] - (unsigned char)s2[i]);
                i++;
        }
        return (0);
}

int	expand_the_value(t_array *child, t_env **env, int i)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		if (ft_strcmp(&((char *)child->items[i])[1], tmp->key) == 0)
		{
			child->items[i] = ft_strdup(tmp->value);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int	expand_variables(t_ast_node *node, t_env **env)
{
	size_t		i;
	int		expand;
	char	*tmp;

	if (!node || !env || !*env)
		return (0);
	i = 0;
	expand = 0;
	while (i < node->children->length)
	{
		tmp = (char *)node->children->items[i];
		printf("%s\n", tmp);
		if (tmp && tmp[0] == '$')
		{
            if (expand_the_value(node->children, env, i))
	            expand = 1;
        }
		i++;
	}
	return (expand);
}

int	traverse_ast(t_ast_node *node, t_env **env)
{
	//size_t i;
    //t_ast_node *child;
    
	// if (!node)
	// 	return (1);
	// if (node->type == TOKEN_TO_EXPAND)
	    return (expand_variables(node, env));
	// if (node->children)
	// {
	// 	i = 0;
	// 	while (i < node->children->length)
	// 	{
	// 		child = (t_ast_node *)node->children->items[i];
	// 		if (traverse_ast(child, env) != 1)
	// 			return (0);
	// 		i++;
	// 	}
	// }
	// return (0);
}
