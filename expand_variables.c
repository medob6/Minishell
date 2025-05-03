/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:37:47 by salahian          #+#    #+#             */
/*   Updated: 2025/05/03 14:35:37 by mbousset         ###   ########.fr       */
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
	return ("");
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

int	check_for_spaces(char *str)
{
	int		i;

	i = 0;
	while (str && str[i])
	{
		if (str[i] == ' ')
			return (1);
		i++;
	}
	return (0);
}

int	check_the_word(t_array *child, t_env **env, int i, int flag, int split)
{
	char *old_str = ft_strdup("");
	char *tmp;
	int index = 0;
	char *str;

	if (flag)
		str = child->items[i];
	else
		str = ((t_token *)child->items[i])->value.str_value;
	while (str[index])
	{
		if (str[index] == '\'')
		{
			int start = index;
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
	if (flag)
	{
		if (split)
			child->items[i] = applicate_field_split(old_str);
		else
			child->items[i] = old_str;
	}
	else
	{
		if (split)
			((t_token *)child->items[i])->value.str_value = applicate_field_split(old_str);
		else
			((t_token *)child->items[i])->value.str_value = old_str;
		if (check_for_spaces(((t_token *)child->items[i])->value.str_value) 
		|| ((t_token *)child->items[i])->value.str_value[0] == '\0')
		{
			if (((t_token *)child->items[i])->value.fd_value != -1)
				close(((t_token *)child->items[i])->value.fd_value);
			((t_token *)child->items[i])->value.fd_value = AMBIGUOUS_REDIRECTION;
		}
	}
	return (1);
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
            check_the_word(node->children, env, i, 1, split);
        }
		i++;
	}
}

int	check_for_dollar_sign(char *s)
{
	int		i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '$')
			return (1);
		i++;
	}
	return (0);
}

char	*expand_heredoc(t_env **env, char *str)
{
	int		index;
	char	*old_str;
	char	*tmp;

	index = 0;
	old_str = ft_strdup("");
	while (str[index])
	{
		if (str[index] == '$')
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

void handle_heredoc_expansion(t_env **env, t_value *value)
{
    int fd1;
    int fd;
    char *line;
    char *new_str;

    fd1 = open("/tmp/temp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    line = get_next_line(value->fd_value);
    while (line)
    {
        if (check_for_dollar_sign(line))
        {
            new_str = expand_heredoc(env, line);
            write(fd1, new_str, ft_strlen(new_str));
        }
        else
            write(fd1, line, ft_strlen(line));
        free(line);
        line = get_next_line(value->fd_value);
    }
    close(fd1);
    fd = open("/tmp/temp", O_RDONLY);
    unlink("/tmp/temp");
    close(value->fd_value);
    value->fd_value = fd;
}

void expand_redirection(t_ast_node *node, t_env **env)
{
	size_t		i;
	int		split;
	char	*tmp;
	
	i = 0;
	split = 0;
	if (!node->redirect_list)
		return ;
	while (i < node->redirect_list->length)
	{
		if (((t_token *)node->redirect_list->items[i])->type == TOKEN_HEREDOC)
		{
			handle_heredoc_expansion(env, &(((t_token *)node->redirect_list->items[i])->value));
			i++;
			continue;
		}
		tmp = ((t_token *)node->redirect_list->items[i])->value.str_value;
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
            check_the_word(node->redirect_list, env, i, 0, split);
        }
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
	
	while ( node->children && i  < node->children->length)
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
