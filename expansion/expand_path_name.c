/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_path_name.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:07:19 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 14:24:11 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

char	*expand_wildcard(char **field, char *str)
{
	char	*path;
	char	*pattern;
	char	*new_str;

	path = NULL;
	pattern = NULL;
	get_pattern_and_path(str, &path, &pattern);
	if (check_pattern(pattern))
		return (NULL);
	new_str = expand_wild(field, path, pattern);
	return (new_str);
}

int	get_len(char **str)
{
	int	count;

	count = 0;
	while (str && str[count])
		count++;
	return (count);
}

void	expand_path_name_cmd(t_expansion *expand)
{
	char	**tmp;
	size_t	i;

	i = 0;
	if (!expand->node->children)
		return ;
	while (i < expand->node->children->length)
	{
		tmp = ((t_str *)expand->node->children->items[i])->value;
		if (tmp && *tmp)
			call_expand_wildcard(expand->node->children, expand->field_cmd, i);
		i++;
	}
}

void	expand_path_name_red(t_expansion *exp)
{
	char			**tmp;
	size_t			i;
	t_token_type	type;

	i = 0;
	if (!exp->node->redirect_list)
		return ;
	while (i < exp->node->redirect_list->length)
	{
		type = ((t_str *)exp->node->redirect_list->items[i])->type;
		if (type == TOKEN_HEREDOC)
		{
			i++;
			continue ;
		}
		tmp = ((t_str *)exp->node->redirect_list->items[i])->value;
		if (tmp && *tmp)
			call_expand_wildcard(exp->node->redirect_list, exp->field_red, i);
		i++;
	}
}
