/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 14:46:20 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 14:46:23 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

void	expand_cmd(t_expansion *expand)
{
	size_t	i;
	char	*tmp;

	i = 0;
	if (!expand->node->children)
		return ;
	expand->field_cmd = create_field(expand->node);
	expand->str = ft_malloc(sizeof(t_str *), expand->node->children->length
			+ 1);
	while (i < expand->node->children->length)
	{
		expand->str[i] = create_t_str();
		tmp = (char *)expand->node->children->items[i];
		if (tmp)
			application_expansion(expand, tmp, i, 0);
		i++;
	}
	expand->str[i] = NULL;
	expand->node->children->items = (void **)expand->str;
}

void	expand_redirection(t_expansion *exp)
{
	size_t			i;
	char			*tmp;
	t_token_type	type;

	i = 0;
	if (!exp->node->redirect_list)
		return ;
	exp->field_red = create_field_red(exp->node);
	exp->str = ft_malloc(sizeof(t_str *), exp->node->redirect_list->length + 1);
	while (i < exp->node->redirect_list->length)
	{
		exp->str[i] = create_t_str();
		type = ((t_token *)exp->node->redirect_list->items[i])->type;
		if (type == TOKEN_HEREDOC)
		{
			handle_exp_in_heredoc(exp, &i);
			continue ;
		}
		tmp = ((t_token *)exp->node->redirect_list->items[i])->value.str_value;
		application_expansion(exp, tmp, i, 1);
		i++;
	}
	exp->str[i] = NULL;
	exp->node->redirect_list->items = (void **)exp->str;
}

t_expansion	*create_t_expand(t_ast_node *node, t_env **env)
{
	t_expansion	*expand;

	expand = ft_malloc(sizeof(t_expansion), 1);
	expand->env = env;
	expand->node = node;
	expand->field_cmd = NULL;
	expand->field_red = NULL;
	expand->str = NULL;
	return (expand);
}

void	expand_variables(t_ast_node *node, t_env **env)
{
	t_expansion	*expand;

	if (!node || !env || !*env)
		return ;
	expand = create_t_expand(node, env);
	expand_redirection(expand);
	expand_cmd(expand);
	expand_path_name_cmd(expand);
	expand_path_name_red(expand);
	check_for_empty_strings(expand);
	check_for_empty_strings_red(expand);
	removes_qouts_cmd(expand);
	removes_qouts_red(expand);
}

void	expand_pipeline(t_ast_node *n, t_env **env)
{
	size_t		i;
	t_expansion	*expand;

	i = 0;
	while (n->children && i < n->children->length)
	{
		if (((t_ast_node *)n->children->items[i])->type == AST_SIMPLE_CMD)
			expand_variables((t_ast_node *)n->children->items[i], env);
		else if (((t_ast_node *)n->children->items[i])->type == AST_SUBSHELL)
		{
			expand = create_t_expand((t_ast_node *)n->children->items[i], env);
			expand_redirection(expand);
		}
		i++;
	}
}
