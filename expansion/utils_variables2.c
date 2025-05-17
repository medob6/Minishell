/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_variables2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 14:37:37 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 14:39:15 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

char	*help_check_the_word(char **field, t_env **env, char *str)
{
	char	*tmp;
	char	*old_str;
	char	*new;
	int		index;
	int		start;

	index = 0;
	old_str = ft_strdup("");
	new = ft_strdup("");
	while (str && str[index])
	{
		if (str[index] == '$' && (*field)[index] != '2')
		{
			start = index;
			tmp = handle_expansion(env, str, &index);
			new = ft_strjoin(new, update_field_exp(field, tmp, start));
			old_str = ft_strjoin(old_str, tmp);
		}
		else
			old_str = ft_strjoin(old_str, copy_and_update(field, str, &index,
						&new));
	}
	*field = new;
	return (old_str);
}

static void	update_child_value(t_expansion *exp, int i, char *expanded,
		int field_split)
{
	char	*copy_exp;

	copy_exp = ft_strdup(expanded);
	if (field_split)
	{
		exp->str[i]->value = applicate_splitting(expanded, exp->field_cmd[i]);
		exp->field_cmd[i] = update_field_after_splitting(exp->field_cmd[i],
				copy_exp);
	}
	else
	{
		exp->str[i]->value[0] = expanded;
		exp->str[i]->value[1] = NULL;
	}
}

static void	update_token_value(t_expansion *exp, int i, char *expanded,
		int field_split)
{
	char			*copy_exp;
	int				fd;
	t_token_type	type;

	copy_exp = ft_strdup(expanded);
	type = ((t_token *)exp->node->redirect_list->items[i])->type;
	fd = ((t_token *)exp->node->redirect_list->items[i])->value.fd_value;
	if (field_split)
	{
		exp->str[i]->value = applicate_splitting(expanded, exp->field_red[i]);
		exp->field_red[i] = update_field_after_splitting(exp->field_red[i],
				copy_exp);
		exp->str[i]->fd = fd;
		exp->str[i]->type = type;
	}
	else
	{
		exp->str[i]->value[0] = expanded;
		exp->str[i]->value[1] = NULL;
		exp->str[i]->fd = fd;
		exp->str[i]->type = type;
	}
}

int	check_the_word(t_expansion *exp, int i, int split, int flag)
{
	char	*str;
	char	*expanded;

	if (!flag)
		str = (char *)exp->node->children->items[i];
	else
		str = ((t_token *)exp->node->redirect_list->items[i])->value.str_value;
	if (!flag)
		expanded = help_check_the_word(&(exp->field_cmd[i]), exp->env, str);
	else
		expanded = help_check_the_word(&(exp->field_red[i]), exp->env, str);
	if (!flag)
		update_child_value(exp, i, expanded, split);
	else
		update_token_value(exp, i, expanded, split);
	return (1);
}

void	application_expansion(t_expansion *expand, char *tmp, size_t i,
		int flag)
{
	int		split;
	t_token	*str;

	if (!tmp)
		return ;
	split = 0;
	if (check_for_field_split(tmp))
	{
		split = 1;
		if (!flag)
			tmp = (char *)expand->node->children->items[i];
		else
		{
			str = (t_token *)expand->node->redirect_list->items[i];
			tmp = str->value.str_value;
		}
		if (!check_is_valid_split(tmp))
			split = 0;
	}
	check_the_word(expand, i, split, flag);
}
