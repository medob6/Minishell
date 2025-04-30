/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:37:47 by salahian          #+#    #+#             */
/*   Updated: 2025/04/30 17:29:14 by salahian         ###   ########.fr       */
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


// change made here  //? here
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
		str = NULL;
	return (str);
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
	while (str[i])
	{
		if (str[i] == ' ')
			return (1);
		i++;
	}
	return (0);
}

int	check_if_qouted(char *str, int index)
{
	int		i;

	i = 0;
	while (i < index)
	{
		if (str[i] == '"')
			return (0);
		i++;
	}
	return (1);
}

//int	check_the_word(t_array *child, t_env **env, int i, int split)
//{
//	char *old_str = ft_strdup("");
//	char *tmp;
//	int index = 0;
//	char *str;

//	if (child)
//		str = child->items[i];
//	else
//		str = ((t_token *)child->items[i])->value.str_value;
//	while (str[index])
//	{
//		if (str[index] == '\'' && check_if_qouted(str, index))
//		{
//			int start = index;
//			index++;
//			while (str[index] && str[index] != '\'')
//				index++;
//			if (str[index] == '\'')
//				index++;
//			tmp = ft_strjoin(old_str, ft_substr(str, start, index - start));
//		}
//		else if (str[index] == '$')
//			tmp = handle_expansion(str, &index, env, old_str);
//		else
//		{
//			tmp = append_char(old_str, str[index]);
//			if (str[index] != '\0')
//				index++;
//		}
//		old_str = tmp;
//	}

//	if (child)
//	{
//		if (split)
//			child->items[i] = applicate_field_split(old_str);
//		else
//			child->items[i] = old_str;
//	}
//	else
//	{
//		if (split)
//			((t_token *)child->items[i])->value.str_value = applicate_field_split(old_str);
//		else
//			((t_token *)child->items[i])->value.str_value = old_str;
//		if (check_for_spaces(((t_token *)child->items[i])->value.str_value) 
//		|| ((t_token *)child->items[i])->value.str_value[0] == '\0')
//		{
//			if (((t_token *)child->items[i])->value.fd_value != -1)
//				close(((t_token *)child->items[i])->value.fd_value);
//			((t_token *)child->items[i])->value.fd_value = AMBIGUOUS_REDIRECTION;
//		}
//	}
//	return (1);
//}

int		search_for(char *str, char c)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
	}
	return (0);
}

static char	*handle_single_quote(char *str, int *index, char *old_str)
{
	int		start;
	char	*tmp;

	start = *index;
	(*index)++;
	while (str[*index] && str[*index] != '\'')
		(*index)++;
	if (str[*index] == '\'')
		(*index)++;
	tmp = ft_strjoin(old_str, ft_substr(str, start, *index - start));
	return (tmp);
}

static char	*handle_other_char(char *str, int *index, char *old_str)
{
	char	*tmp;

	tmp = append_char(old_str, str[*index]);
	if (str[*index] != '\0')
		(*index)++;
	return (tmp);
}

static char	*expand_loop(t_array *child, char *str, t_env **env)
{
	int		index;
	char	*old_str;
	char	*tmp;

	index = 0;
	old_str = ft_strdup("");
	while (str[index])
	{
		if (str[index] == '\'' && check_if_qouted(str, index))
			tmp = handle_single_quote(str, &index, old_str);
		else if (str[index] == '$')
		{
			tmp = handle_expansion(str, &index, env, old_str);
			//printf("here\n");
			//if (search_for(tmp, '"'))
				//child->expand_qout = 1;
		}
		else
			tmp = handle_other_char(str, &index, old_str);
		old_str = tmp;
	}
	return (old_str);
}

static void	update_child_value(t_array *child, int i, char *expanded, int split)
{
	if (split)
		child->items[i] = applicate_field_split(expanded);
	else
		child->items[i] = expanded;
}

static void	update_token_value(t_token *token, char *expanded, int split)
{
	if (split)
		token->value.str_value = applicate_field_split(expanded);
	else
		token->value.str_value = expanded;

	if (search_for(token->value.str_value, ' ') || token->value.str_value[0] == '\0')
	{
		if (token->value.fd_value != -1)
			close(token->value.fd_value);
		token->value.fd_value = AMBIGUOUS_REDIRECTION;
	}
}

int	check_the_word(t_array *child, t_env **env, int i, int split)
{
	char	*str;
	char	*expanded;

	if (child)
		str = child->items[i];
	else
		str = ((t_token *)child->items[i])->value.str_value;
	
	expanded = expand_loop(child, str, env);
	if (child)
		update_child_value(child, i, expanded, split);
	else
		update_token_value(((t_token *)child->items[i]), expanded, split);
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
    			if (!check_the_last_arg(tmp))
				{
					//printf("%zu %zu\n", (size_t)check_the_last_arg(tmp), ft_strlen(tmp));
        			split = 0;
				}
			}
            check_the_word(node->children, env, i, split);
        }
		i++;
	}
}
// $a ls => "" ls

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
	char	*str;

	str = get_name_heredoc();
    fd1 = open(str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    line = get_next_line(value->fd_value);
    while (line)
    {
        if (search_for(line, '$'))
            write(fd1, expand_heredoc(env, line), ft_strlen(expand_heredoc(env, line)));
        else
            write(fd1, line, ft_strlen(line));
        free(line);
        line = get_next_line(value->fd_value);
    }
    close(fd1);
    fd = open(str, O_RDONLY);
    unlink(str);
    close(value->fd_value);
    value->fd_value = fd;
}

//void expand_redirection(t_ast_node *node, t_env **env)
//{
//	size_t		i;
//	int		split;
//	char	*tmp;
	
//	i = 0;
//	split = 0;
//	if (!node->redirect_list)
//		return ;
//	while (i < node->redirect_list->length)
//	{
//		if (((t_token *)node->redirect_list->items[i])->type == TOKEN_HEREDOC)
//		{
//			handle_heredoc_expansion(env, &(((t_token *)node->redirect_list->items[i])->value));
//			i++;
//			continue;
//		}
//		tmp = ((t_token *)node->redirect_list->items[i])->value.str_value;
//		if (tmp)
//		{
//			if (check_for_field_split(tmp))
//			{
//				split = 1;
//				if (check_for_last_exp(node) != -1)
//					tmp = (char *)node->children->items[check_for_last_exp(node)];
//    			if ((size_t)check_the_last_arg(tmp) != ft_strlen(tmp))
//        			split = 0;
//			}
//            check_the_word(node->redirect_list, env, i, split);
//        }
//		i++;
//	}
//}
bool is_here_doc(void *item)
{
	t_token *token = (t_token *)item;
	return (token->type == TOKEN_HEREDOC);
}


int should_disable_split(t_ast_node *node, int split)
{
	char *tmp;
	int last_exp_index;

	last_exp_index = check_for_last_exp(node);
	if (last_exp_index != -1)
	{
		tmp = (char *)node->children->items[last_exp_index];
		if ((size_t)check_the_last_arg(tmp) != ft_strlen(tmp))
			split = 0;
	}
	return (split);
}

void handle_redir_word(t_ast_node *node, t_env **env, size_t i)
{
	char *str;
	int split;

	split = 0;
	str = ((t_token *)node->redirect_list->items[i])->value.str_value;
	if (!str)
		return ;
	if (check_for_field_split(str))
	{
		split = 1;
		split = should_disable_split(node, split);
	}
	check_the_word(node->redirect_list, env, i, split);
}


void expand_redirection(t_ast_node *node, t_env **env)
{
	size_t i;

	if (!node->redirect_list)
		return ;
	i = 0;
	while (i < node->redirect_list->length)
	{
		if (is_here_doc(node->redirect_list->items[i]))
		{
			handle_heredoc_expansion(env, &(((t_token *)node->redirect_list->items[i])->value));
			i++;
			continue;
		}
		handle_redir_word(node, env, i);
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
	expand_cmd(node,env);// "*"
	expand_path_name_cmd(node);
	expand_path_name_red(node);
	check_for_empty_strings(node);
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
	//node->children->length = 2;	
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
