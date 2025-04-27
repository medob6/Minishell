/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:37:47 by salahian          #+#    #+#             */
/*   Updated: 2025/04/27 14:49:29 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	valid(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_');
}

int	is_valid_length(char *tmp, int flag)
{
	int i;

	i = flag;
	if (!flag && !valid(tmp[i]))
		return (0);
	i = !(flag);
	while (tmp[i])
	{
		if (!((tmp[i] >= '0' && tmp[i] <= '9') ||
			(tmp[i] >= 'A' && tmp[i] <= 'Z') ||
			(tmp[i] >= 'a' && tmp[i] <= 'z') ||
			tmp[i] == '_'))
			break;
		i++;
	}
	return (i);
}

char  *expand_the_value(char *str, t_env **env)
{
	t_env	*tmp;
	int		var_len;
	int		index;
	char	next;
	char	*old_str;

	old_str = str;
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
	index = is_valid_length(str, 1);
	if (index)
		str = ft_strdup(&str[index]);
	else
		str = ft_strdup("");
	return (old_str);
}

size_t	next_dollar(char *s)
{
	int		i;

	i = 1;
	while (s[i])
	{
		if (s[i] == '$')
			return (i);
		i++;
	}
	return (0);
}

char *handle_expansion(char *str, int *index, t_env **env, char *old_str)
{
	int var_len;
	char *tmp;
	char *exp;
	char *var;
	char buf[2];

	var_len = is_valid_length(&str[*index + 1], 0);
	if (var_len == 0)
	{
		buf[0] = '$';
		buf[1] = '\0';
		tmp = ft_strjoin(old_str, buf);
		(*index)++;
		return (tmp);
	}
	var = ft_substr(str, *index, var_len + 1);
	exp = expand_the_value(var, env);
	tmp = ft_strjoin(old_str, exp);
	*index += var_len + 1;
	return (tmp);
}

char *append_char(char *old_str, char c)
{
	char buf[2];
	char *tmp;

	buf[0] = c;
	buf[1] = '\0';
	tmp = ft_strjoin(old_str, buf);
	return (tmp);
}

char	*help_check_the_word(t_env **env, char *str, char *old_str, int index)
{
	int		start;
	char	*tmp;
	
	while (str[index])
	{
		if (str[index] == '\'')
		{
			start = index;
			index++;
			while (str[index] && str[index] != '\'')
				index++;
			if (str[index] == '\'')
				index++;
			tmp = ft_strjoin(old_str, ft_substr(str, start, index - start));
		}
		else if (str[index] == '$')
			tmp = handle_expansion(str, &index, env, old_str);
		else
		{
			tmp = append_char(old_str, str[index]);
			if (str[index] != '\0')
				index++;
		}
		old_str = tmp;
	}
	return (old_str);
}

void	check_the_word(t_array *child, t_env **env, int i, int split)
{
	char *old_str;
	int index;
	char *str;

	if (child)
		str = child->items[i];
	else
		str = ((t_token *)child->items[i])->value;
	old_str = ft_strdup("");
	index = 0;
	old_str = help_check_the_word(env, str, old_str, index);
	if (child)
	{
		if (split)
			child->items[i] = applicate_field_split(old_str);
		else
			child->items[i] = old_str;
	}
	else
	{
		if (split)
			((t_token *)child->items[i])->value = applicate_field_split(old_str);
		else
			((t_token *)child->items[i])->value = old_str;
	}
}

int	check_the_single_qout(char *s)
{
	int		i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'')
		{
			if (ft_strchr(&s[i], '$') != NULL)
				return (0);
		}
		i++;
	}
	return (1);
}

void expand_cmd(t_ast_node *node, t_env **env)
{
	size_t		i;
	int		split;
	char	*tmp;
	
	i = 0;
	split = 0;
	if (!node->children)
		return ;
	while (i < node->children->length)
	{
		tmp = (char *)node->children->items[i];
		if (tmp)
		{
			if (check_for_field_split(tmp))
			{
				split = 1;
				if (check_for_last_exp(node) != -1)
					tmp = (char *)node->children->items[check_for_last_exp(node)];
    			if ((size_t)check_the_last_arg(tmp) != ft_strlen(tmp))
        			split = 0;
			}
            check_the_word(node->children, env, i, split);
        }
		i++;
	}
}

void	help_expand_redirection(t_ast_node *node, t_env **env, int i, int *split)
{
	char	*tmp;

	tmp = ((t_token *)node->redirect_list->items[i])->value;
	if (tmp)
	{
		if (check_for_field_split(tmp))
		{
			*split = 1;
			if (check_for_last_exp_red(node) != -1)
				tmp = ((t_token *)node->redirect_list->items[check_for_last_exp(node)])->value;
    		if ((size_t)check_the_last_arg(tmp) != ft_strlen(tmp))
        		*split = 0;
		}
        check_the_word(node->redirect_list, env, i, *split);
    }
}

void expand_redirection(t_ast_node *node, t_env **env)
{
	size_t		i;
	int		split;
	
	i = 0;
	split = 0;
	if (!node->redirect_list)
		return ;
	while (i < node->redirect_list->length)
	{
		if (((t_token *)node->redirect_list->items[i])->type == TOKEN_HEREDOC)
		{
			i++;
			continue;
		}
		help_expand_redirection(node, env, i, &split);
		i++;
	}
}

int	expand_variables(t_ast_node *node, t_env **env)
{
	int expand;
	if (!node || !env || !*env)
		return (0);
	expand = 0;
	expand_redirection(node,env);
	expand_cmd(node,env);
	expand_path_name_cmd(node);
	expand_path_name_red(node);
	removes_qouts_cmd(node);
	removes_qouts_red(node);
	return (expand);
}

void expand_pipeline(t_ast_node *node, t_env **env)
{
	size_t i;
	i = 0;
	while (i  < node->children->length)
	{
		if (((t_ast_node *)node->children->items[i])->type == AST_SIMPLE_CMD)
			expand_variables((t_ast_node *)node->children->items[i],env);
		else if (((t_ast_node *)node->children->items[i])->type == AST_SUBSHELL)
		{
			expand_redirection((t_ast_node *)node->children->items[i],env);
			expand_ast((t_ast_node *)node->children->items[i],env);
		}
		i++;
	}
	
}
int	expand_ast(t_ast_node *node, t_env **env)
{
	size_t i;

	if (!node || !node->children)
		return (0);
	i = 0;
	while (i  < node->children->length)
	{
		if (((t_ast_node *)node->children->items[i])->type == AST_PIPELINE)
			expand_pipeline((t_ast_node *)node->children->items[i],env);
		i++;
	}
	return (1);
}
