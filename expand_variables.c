/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:37:47 by salahian          #+#    #+#             */
/*   Updated: 2025/04/19 14:53:43 by salahian         ###   ########.fr       */
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

int		is_valid(char *tmp)
{
	int		i;

	i = 1;
	if(!((tmp[i] >= 'A' && tmp[i] <= 'Z') ||
      	(tmp[i] >= 'a' && tmp[i] <= 'z') ||
      	tmp[i] == '_'))
		return 1;
	while (tmp[i])
	{
		if (!((tmp[i] >= '0' && tmp[i] <= '9') ||
      	(tmp[i] >= 'A' && tmp[i] <= 'Z') ||
      	(tmp[i] >= 'a' && tmp[i] <= 'z') ||
      	tmp[i] == '_'))
			return (i);
		i++;
	}
	return (0);
}

char  *expand_the_value(char *str, t_env **env)
{
	t_env	*tmp;
	int		var_len;
	int		index;
	char	next;

	str = &str[1];
	tmp = *env;
	while (tmp)
	{
		var_len = ft_strlen(tmp->key);
		if (ft_strncmp(str, tmp->key, var_len) == 0)
		{
			next = str[var_len];
			if (next == '\0' || !(next == '_' || (next >= '0' && next <= '9') ||
				(next >= 'a' && next <= 'z') || (next >= 'A' && next <= 'Z')))
			{
				if (str[var_len] != '\0')
					str = ft_strjoin(ft_strdup(tmp->value), &str[var_len]);
				else
					str = ft_strdup(tmp->value);
				return (str);
			}
		}
		tmp = tmp->next;
	}
	index = is_valid(str);
	if (index)
		str = ft_strjoin(ft_strdup(""), &str[index]);
	else
		str = ft_strdup("");
	return (str);
}

size_t	next_dollar(char *s)
{
	int		i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '$')
			return (i);
		i++;
	}
	return (i);
}

int		check_the_word(t_array *child, t_env **env, int i)
{
	char	*str;
	char	*old_str;
	char	*new_str;
	int		index;

	str = (char *)child->items[i];
	index = 0;
	old_str = NULL;
	while (str[index])
	{
		if (str[index] == '$')
		{
			new_str = expand_the_value(&str[index], env);
			if (new_str[0] != '\0')
				child->items[i] = new_str;
			else
				old_str = ft_strjoin(old_str, ft_substr(&str[index], index, next_dollar(&str[index]) + 2));
		}
		index++;
	}
	child->items[i] = ft_strjoin(old_str, new_str);
	return (1);
}


// int	expand_the_value(t_array *child, t_env **env, int i)
// {
// 	t_env	*tmp;
// 	int		index;

// 	tmp = *env;
// 	while (tmp)
// 	{
// 		if (ft_strncmp(&((char *)child->items[i])[1], tmp->key, ft_strlen(tmp->key)) == 0)
// 		{
// 			printf("%d\n", is_valid(&((char *)child->items[i])[1]));
// 			if (ft_strlen(&((char *)child->items[i])[1]) > ft_strlen(tmp->key) && is_valid(&((char *)child->items[i])[1]))
// 				child->items[i] = ft_strjoin(ft_strdup(tmp->value),
// 				&((char *)child->items[i])[ft_strlen(tmp->key) + 1]);
// 			else if (!is_valid(&((char *)child->items[i])[1]))
// 				child->items[i] = ft_strdup(tmp->value);
// 			else
// 				child->items[i] = ft_strdup("");
// 			return (1);
// 		}
// 		tmp = tmp->next;
// 	}
// 	index = 0;
// 	index = is_valid(&((char *)child->items[i])[1]);
// 	if (index)
// 		child->items[i] = ft_strjoin(ft_strdup(""),
// 		&((char *)child->items[i])[index]);
// 	else
// 		child->items[i] = ft_strdup("");
// 	return (0);
// }

int	valid(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_');
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
		if (tmp && tmp[0] == '$')
		{
            if (check_the_word(node->children, env, i))
	            expand = 1;
        }
		i++;
	}
	return (expand);
}

// int	traverse_ast(t_ast_node *node, t_env **env)
// {
// 	size_t i;
//     t_ast_node *child;
    
// 	if (!node)
// 		return (1);
// 	if (node->type == TOKEN_WORD)
// 	    return (expand_variables(node, env));
// 	if (node->children)
// 	{
// 		i = 0;
// 		while (i < node->children->length)
// 		{
// 			child = (t_ast_node *)node->children->items[i];
// 			if (traverse_ast(child, env) != 1)
// 				return (0);
// 			i++;
// 		}
// 	}
// 	return (0);
// }

int	traverse_ast(t_ast_node *node, t_env **env)
{
	size_t i;

	if (!node)
		return (1);

	// Expand only if this is a command node with arguments
	if (node->type == AST_SIMPLE_CMD)
		return expand_variables(node, env);

	// For other nodes, traverse their AST children
	if (node->children)
	{
		for (i = 0; i < node->children->length; i++)
		{
			t_ast_node *child = (t_ast_node *)node->children->items[i];
			if (!traverse_ast(child, env))
				return (0);
		}
	}
	return (1);
}
