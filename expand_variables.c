/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:37:47 by salahian          #+#    #+#             */
/*   Updated: 2025/05/02 15:06:05 by salahian         ###   ########.fr       */
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

void	print_field(t_bit_mask *field, char *str)
{
	for (int i = 0; str[i]; i++)
		printf("%c (%d) ", str[i], field[i]);
	printf("\n");
}
void	update_field_expansion(t_bit_mask **field, char *old_str, char *exp, int len)
{
	int		size;
	int		i;
	int		j;
	int		old_i;
	t_bit_mask	*new_field;

	size = (ft_strlen(old_str) + ft_strlen(exp) - len);
	new_field = ft_malloc(sizeof(t_bit_mask), size + 1);
	i = 0;
	j = 0;
	while (old_str[i])
	{
		if (old_str[i] == '$')
			break ;
		new_field[j] = (*field)[j];
		j++;	
		i++;
	}
	old_i = i;
	i = 0;
	while (exp[i])
	{
		new_field[j] = (*field)[j] | EXPANDED;
		j++;
		i++;
	}
	old_i += len;
	while (old_str[old_i])
	{
		new_field[j] = (*field)[old_i];
		j++;
		old_i++;
	}
	new_field[j] = ORIGINAL;
	(*field) = new_field;
}


//int handle_expansion(char *str, t_env **env, char **old_str, t_bit_mask **field)
//{
//	int var_len;
//	//char *tmp;
//	char *exp;
//	char *var;
//	char buf[2];
//	(void)field;

//	var_len = is_valid_length(&str[1], 0);
//	if (var_len == 0)
//	{
//		buf[0] = '$';
//		buf[1] = '\0';
//		*old_str = ft_strjoin(*old_str, buf);
//		return (1);
//	}
//	var = ft_substr(str, 0, var_len + 1);
//	exp = expand_the_value(var, env);
//	*old_str = ft_strjoin(*old_str, exp);
//	return (var_len + 1);
//}

int handle_expansion(char *str, t_env **env, char **old_str, t_bit_mask **field)
{
	int var_len;
	char *exp;
	char *var;
	char buf[2];
	int old_len;

	var_len = is_valid_length(&str[1], 0);
	if (var_len == 0)
	{
		buf[0] = '$';
		buf[1] = '\0';
		*old_str = ft_strjoin(*old_str, buf);
		return (1);
	}
	//update_field_expansion(field, *old_str, exp, str, var_len);
	var = ft_substr(str, 0, var_len + 1);
	exp = expand_the_value(var, env);
	*old_str = ft_strjoin(*old_str, exp);

	//print_field(*field, *old_str);
	//printf("[%s]\n", *old_str);
	return (var_len + 1);
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
//	} *
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
		i++;
	}
	return (0);
}

static int	handle_single_quote(char *str, char **old_str)
{
	int		start;
	//char	*tmp;

	start = 0;
	start++;
	while (str[start] && str[start] != '\'')
		start++;
	if (str[start] == '\'')
		start++;
	*old_str = ft_strjoin(*old_str, ft_substr(str, 0, start));
	return (start);
}

static int handle_other_char(char *str, char **old_str)
{
	//char	*tmp;

	*old_str = append_char(*old_str, str[0]);
	if (str[0] != '\0')
		return (1);
	return (0);
}

//void	update_field(t_bit_mask **field, char *old_str, char *new_str, int len)
//{
//	int		i;
//	int		old_i;
//	t_bit_mask	*new_field;

//	i = 0;
//	new_field = ft_malloc(sizeof(t_bit_mask), ft_strlen(new_str) + 1);
//	while (new_str[i])
//	{
//		if (new_str[i] == old_str[i])
//			new_field[i] = (*field)[i];
//		else
//		{
//			old_i = i;
//			while (new_str[i] != old_str[len])
//			{
//				if ((*field)[old_i] == 0)
//					new_field[i] = EXPANDED;
//				else
//					new_field[i] = DOUBLE_QOUT | EXPANDED;
//				i++;
//			}
//			while (old_str && old_str[len])
//				new_field[i++] = (*field)[len++];
//		}
//		if (new_str[i] && old_str[len])
//			i++;
//	}
//	new_field[i] = ORIGINAL;
//	*field = new_field;
//}

//void	update_field(t_bit_mask **field, int old_len, int insert_start, int insert_len, char *new_str)
//{
//	t_bit_mask	*new_field;
//	int			i;

//	new_field = ft_malloc(sizeof(t_bit_mask), ft_strlen(new_str) + 1);
//	i = 0;

//	while (new_str[i])
//	{
//		if (i >= insert_start && i < insert_start + insert_len)
//			new_field[i] = DOUBLE_QOUT | EXPANDED;
//		else if (i < old_len)
//			new_field[i] = (*field)[i];
//		else
//			new_field[i] = ORIGINAL;
//		i++;
//	}
//	new_field[i] = ORIGINAL;
//	*field = new_field;
//}




static char	*expand_loop(t_bit_mask **field, char *str, t_env **env)
{
	int		index;
	char	*old_str;
	//char	*tmp;

	index = 0;
	old_str = ft_strdup("");
	while (str[index])
	{
		if ((*field)[index] == 2)
			index += handle_single_quote(&str[index], &old_str);
		else if (str[index] == '$')
		{
			index += handle_expansion(&str[index], env, &old_str, field);
			//if (tmp)
			//	update_field(field, str, tmp, index - old_index);
			//printf("%s\n", old_str);
		}
		else
			index += handle_other_char(&str[index], &old_str);
	}
	//print_field(*field, str);
	printf("[%s]\n", old_str);
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

int	check_the_word(t_ast_node *node, t_env **env, int i, int split)
{
	char	*str;
	char	*expanded;

	if (node->children)
		str = node->children->items[i];
	else
		str = ((t_token *)node->children->items[i])->value.str_value;
	
	expanded = expand_loop(&(node->field[i]), str, env);
	if (node->children)
		update_child_value(node->children, i, expanded, split);
	else
		update_token_value(((t_token *)node->children->items[i]), expanded, split);
	return (1);
}


//int	check_the_single_qout(char *s)
//{
//	int		i;

//	i = 0;
//	while (s[i])
//	{
//		if (s[i] == '\'')
//		{
//			if (ft_strchr(&s[i], '$') != NULL)
//				return (0);
//		}
//		i++;
//	}
//	return (1);
//}

void expand_cmd(t_ast_node *node, t_env **env)
{
	size_t		i;
	int		split;
	char	*tmp;
	
	i = 0;
	split = 0;
	if (!node->children)
		return ;
	node->field = create_field(node);
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
        			split = 0;
			}
            check_the_word(node, env, i, split);
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

char	*expand_heredoc(t_ast_node *node, t_env **env, char *str)
{
	int		index;
	char	*old_str;
	char	*tmp;

	index = 0;
	old_str = ft_strdup("");
	while (str[index])
	{
		if (str[index] == '$')
			index += handle_expansion(&str[index], env, &old_str, node->field);
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

void handle_heredoc_expansion(t_ast_node *node, t_env **env, t_value *value)
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
            write(fd1, expand_heredoc(node, env, line), ft_strlen(expand_heredoc(node, env, line)));
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

void expand_redirection(t_ast_node *node, t_env **env)
{
	size_t		i;
	int		split;
	char	*tmp;
	
	i = 0;
	split = 0;
	if (!node->redirect_list)
		return ;
	node->field = create_field(node);
	while (i < node->redirect_list->length)
	{
		if (((t_token *)node->redirect_list->items[i])->type == TOKEN_HEREDOC)
		{
			handle_heredoc_expansion(node, env, &(((t_token *)node->redirect_list->items[i])->value));
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
            check_the_word(node, env, i, split);
        }
		i++;
	}
}
//bool is_here_doc(void *item)
//{
//	t_token *token = (t_token *)item;
//	return (token->type == TOKEN_HEREDOC);
//}


//int should_disable_split(t_ast_node *node, int split)
//{
//	char *tmp;
//	int last_exp_index;

//	last_exp_index = check_for_last_exp(node);
//	if (last_exp_index != -1)
//	{
//		tmp = (char *)node->children->items[last_exp_index];
//		if ((size_t)check_the_last_arg(tmp) != ft_strlen(tmp))
//			split = 0;
//	}
//	return (split);
//}

//void handle_redir_word(t_ast_node *node, t_env **env, size_t i)
//{
//	char *str;
//	int split;

//	split = 0;
//	str = ((t_token *)node->redirect_list->items[i])->value.str_value;
//	if (!str)
//		return ;
//	if (check_for_field_split(str))
//	{
//		split = 1;
//		split = should_disable_split(node, split);
//	}
//	check_the_word(node->redirect_list, env, i, split);
//}


//void expand_redirection(t_ast_node *node, t_env **env)
//{
//	size_t i;

//	if (!node->redirect_list)
//		return ;
//	i = 0;
//	while (i < node->redirect_list->length)
//	{
//		if (is_here_doc(node->redirect_list->items[i]))
//		{
//			handle_heredoc_expansion(env, &(((t_token *)node->redirect_list->items[i])->value));
//			i++;
//			continue;
//		}
//		handle_redir_word(node, env, i);
//		i++;
//	}
//}

int	expand_variables(t_ast_node *node, t_env **env)
{
	int expand;
	if (!node || !env || !*env)
		return (0);
	expand = 0;
	//items = create_copy(node);
	expand_redirection(node,env);
	expand_cmd(node,env);// "*"
	expand_path_name_cmd(node);
	expand_path_name_red(node);
	check_for_empty_strings(node);
	//removes_qouts_cmd(node);
	//printf("here\n");
	//removes_qouts_red(node);
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
