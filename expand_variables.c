/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:37:47 by salahian          #+#    #+#             */
/*   Updated: 2025/05/07 14:31:46 by salahian         ###   ########.fr       */
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

//void	print_field(t_bit_mask *field, char *str)
//{
//	for (int i = 0; str[i]; i++)
//		printf("%c (%d) ", str[i], field[i]);
//	printf("\n");
//}
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

//void	update_field_expansion(t_bit_mask **field, char *old_str, char *exp, int len)
//{
//	int		size;
//	int		i;
//	int		j;
//	int		old_i;
//	t_bit_mask	*new_field;

//	size = (ft_strlen(old_str) + ft_strlen(exp) - len);
//	new_field = ft_malloc(sizeof(t_bit_mask), size + 1);
//	i = 0;
//	j = 0;
//	while (old_str[i])
//	{
//		if (old_str[i] == '$')
//			break ;
//		new_field[j] = (*field)[j];
//		j++;	
//		i++;
//	}
//	old_i = i;
//	i = 0;
//	while (exp[i])
//	{
//		new_field[j] = (*field)[j] | EXPANDED;
//		j++;
//		i++;
//	}
//	old_i += len;
//	while (old_str[old_i])
//	{
//		new_field[j] = (*field)[old_i];
//		j++;
//		old_i++;
//	}
//	new_field[j] = ORIGINAL;
//	(*field) = new_field;
//}


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

//int handle_expansion(char *str, t_env **env, char **old_str, t_bit_mask **field)
//{
//	int var_len;
//	char *exp;
//	char *var;
//	char buf[2];
//	int old_len;

//	var_len = is_valid_length(&str[1], 0);
//	if (var_len == 0)
//	{
//		buf[0] = '$';
//		buf[1] = '\0';
//		*old_str = ft_strjoin(*old_str, buf);
//		return (1);
//	}
//	//update_field_expansion(field, *old_str, exp, str, var_len);
//	var = ft_substr(str, 0, var_len + 1);
//	exp = expand_the_value(var, env);
//	*old_str = ft_strjoin(*old_str, exp);

//	//print_field(*field, *old_str);
//	//printf("[%s]\n", *old_str);
//	return (var_len + 1);
//}


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

//static char	*handle_single_quote(char *str, int *index, char *old_str)
//{
//	int		start;
//	char	*tmp;

//	start = 0;
//	start++;
//	while (str[start] && str[start] != '\'')
//		start++;
//	if (str[start] == '\'')
//		start++;
//	*old_str = ft_strjoin(*old_str, ft_substr(str, 0, start));
//	return (start);
//}

//static int handle_other_char(char *str, char **old_str)
//{
//	//char	*tmp;

//	*old_str = append_char(*old_str, str[0]);
//	if (str[0] != '\0')
//		return (1);
//	return (0);
//}

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




//static char	*expand_loop(t_bit_mask **field, char *str, t_env **env)
//{
//	int		index;
//	char	*old_str;
//	char	*tmp;

//	index = 0;
//	old_str = ft_strdup("");
//	while (str[index])
//	{
//		if ((*field)[index] == 2)
//			tmp = handle_single_quote(str, &index, old_str);
//		else if (str[index] == '$')
//		{
//			index += handle_expansion(&str[index], env, &old_str, field);
//			//if (tmp)
//			//	update_field(field, str, tmp, index - old_index);
//			//printf("%s\n", old_str);
//		}
//		else
//			index += handle_other_char(&str[index], &old_str);
//	}
//	//print_field(*field, str);
//	printf("[%s]\n", old_str);
//	return (old_str);
//}

//void	update_field(t_bit_mask **field, char *str, char *exp, int len)
//{
//	t_bit_mask	*new_field;
//	int		i;
//	int		j;
//	int		old_i;
//	int		size;

//	size = (ft_strlen(str) + ft_strlen(exp)) - len;
//	new_field = ft_malloc(sizeof(t_bit_mask), size + 1);
//	i = 0;
//	j = 0;
//	while (str[i])
//	{
//		if (str[i] == '$')
//			break ;
//		new_field[j] = (*field)[i];
//		j++;	
//		i++;
//	}
//	old_i = i;
//	i = 0;
//	while (exp[i])
//	{
//		if ((*field)[old_i + 1] == 4)
//			new_field[j] = DOUBLE_QOUT | EXPANDED;
//		else
//			new_field[j] = EXPANDED;
//		j++;
//		i++;
//	}
//	old_i += (len + 1);
//	while (str[old_i])
//	{
//		new_field[j] = (*field)[old_i];
//		j++;
//		old_i++;
//	}
//	new_field[j] = ORIGINAL;
//	(*field) = new_field;
//}

char *handle_expansion(t_env **env, char *str, int *index)
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
		tmp = ft_strdup(buf);
		(*index)++;
		return (tmp);
	}
	var = ft_substr(str, *index, var_len + 1);
	exp = expand_the_value(var, env);
	*index += var_len + 1;
	return (exp);
}

//char *append_char(char *old_str, char c)
//{
//	char buf[2];
//	char *tmp;

//	buf[0] = c;
//	buf[1] = '\0';
//	tmp = ft_strjoin(old_str, buf);
//	return (tmp);
//}

char	*handle_what_in_single_qout(char *str, int *index)
{
	char	*tmp;
	int		start;

	start = *index;
	(*index)++;
	while (str[*index] && str[*index] != '\'')
		(*index)++;
	if (str[*index] == '\'')
		(*index)++;
	tmp = ft_substr(str, start, *index - start);
	return (tmp);
}


char	*update_field(char **field, char *tmp, int index)
{
	int		i;
	char	*old_new;
	
	old_new = ft_malloc(sizeof(char), ft_strlen(tmp) + 1);
	i = 0;
	while (tmp[i])
	{
		old_new[i] = (*field)[index];
		i++;
		index++;
	}
	old_new[i] = '\0';
	return (old_new);
}

char	*update_field_exp(char	**field, char *exp, int index)
{
	int		i;
	char	*tmp;

	tmp = ft_malloc(1, ft_strlen(exp) + 1);
	i = 0;
	while (exp[i])
	{
		if ((*field)[index] == '0')
			tmp[i] = '1';
		else
			tmp[i] = '5';
		i++;
	}
	tmp[i] = '\0';
	return (tmp);
}

char	*append(char s)
{
	char	buf[2];

	buf[0] = s;
	buf[1] = '\0';
	return (ft_strdup(buf));
}

char	*help_check_the_word(char **field, t_env **env, char *str)
{
	int		start;
	char	*tmp;
	int		index;
	char	*old_str;
	char *new;
	char	*hold;

	index = 0;
	old_str = ft_strdup("");
	hold = ft_strdup("");
	new = ft_strdup("");
	while (str[index])
	{
		if (str[index] == '$' && (*field)[index] != '2')
		{		
			start = index;
			tmp = handle_expansion(env, str, &index);
			new = ft_strjoin(new, update_field_exp(field, tmp, start));
			tmp = ft_strjoin(old_str, tmp);
		}
		else
		{
			start = index;
			tmp = append(str[index++]);
			new = ft_strjoin(new, update_field(field, tmp, start));
			tmp = ft_strjoin(old_str, tmp);
		}
		old_str = tmp;
		hold = new;
	}
	*field = hold;
	//printf("[%s]\n", *field);
	return (old_str);
}
//void	printl(char **s)
//{
//	int		i;

//	i = 0;
//	while (s[i])
//	{
//		printf("[%s]\n", s[i]);
//		i++;
//	}
//}

static void	update_child_value(t_expansion *expand, int i, char *expanded, int field_split)
{
	if (field_split)
	{
		expand->str[i]->value = applicate_splitting(expanded, expand->field[i]);
	}
	else
	{
		expand->str[i]->value[0] = expanded;
		expand->str[i]->value[1] = NULL;
	}
		
}


static void	update_token_value(t_expansion *expand, int i, char *expanded, int field_split)
{
	if (field_split)
	{
		expand->str[i]->value = applicate_splitting(expanded, expand->field[i]);
		expand->str[i]->fd = ((t_token *)expand->node->redirect_list->items[i])->value.fd_value;
		expand->str[i]->type = ((t_token *)expand->node->redirect_list->items[i])->type;
	}
	else
	{
		expand->str[i]->value[0] = expanded;
		expand->str[i]->value[1] = NULL;
		expand->str[i]->fd = ((t_token *)expand->node->redirect_list->items[i])->value.fd_value;
		expand->str[i]->type = ((t_token *)expand->node->redirect_list->items[i])->type;
	}
}

int	check_the_word(t_expansion *expand, int i, int split)
{
	char	*str;
	char	*expanded;

	if (expand->node->children)
		str = expand->node->children->items[i];
	else
		str = ((t_token *)expand->node->children->items[i])->value.str_value;
	
	expanded = help_check_the_word(&(expand->field[i]), expand->env, str);
	if (expand->node->children)
		update_child_value(expand, i, expanded, split);
	else
		update_token_value(expand, i, expanded, split);
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

//t_str	*create_t_str(void)
//{
//	t_str	*str;
	
//	str = ft_malloc(sizeof(t_str), 1);
//	*str->value = ft_malloc(sizeof(char *), 2);
//	str->fd = -1;
//	str->type = -1;
//	return (str);
//}

t_str	*create_t_str(void)
{
	t_str	*str;

	str = ft_malloc(sizeof(t_str), 1);
	if (!str)
		return (NULL);

	str->value = ft_malloc(sizeof(char *), 2); // allocate the string array
	if (!str->value)
		return (NULL);

	str->value[0] = NULL; // init to NULL so it's safe
	str->fd = -1;
	str->type = -1;
	return (str);
}


void expand_cmd(t_expansion *expand)
{
	size_t		i;
	int		split;
	char	*tmp;
	
	i = 0;
	split = 0;
	if (!expand->node->children)
		return ;
	expand->field = create_field(expand->node);
	expand->str = ft_malloc(sizeof(t_str *), expand->node->children->length + 1);
	while (i < expand->node->children->length)
	{
		expand->str[i] = create_t_str();
		tmp = (char *)expand->node->children->items[i];
		if (tmp)
		{
			if (check_for_field_split(tmp))
			{
				split = 1;
				if (check_for_last_exp(expand->node) != -1)
					tmp = (char *)expand->node->children->items[check_for_last_exp(expand->node)];
    			if (!check_the_last_arg(tmp))
        			split = 0;
			}
            check_the_word(expand, i, split);
        }
		i++;
	}
	expand->str[i] = NULL;
	expand->node->children->items = (void **)expand->str;
}
// $a ls => "" ls

//int	check_for_dollar_sign(char *s)
//{
//	int		i;

//	i = 0;
//	while (s[i])
//	{
//		if (s[i] == '$')
//			return (1);
//		i++;
//	}
//	return (0);
//}

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
			tmp = ft_strjoin(old_str, handle_expansion(env, str, &index));
		else
			tmp = append_char(old_str, str[index++]);
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

void expand_redirection(t_expansion *expand)
{
	size_t		i;
	int		split;
	char	*tmp;
	
	i = 0;
	split = 0;
	
	if (!expand->node->redirect_list)
		return ;
	expand->field = create_field(expand->node);
	expand->str = ft_malloc(sizeof(t_str *), expand->node->redirect_list->length + 1);
	while (i < expand->node->redirect_list->length)
	{
		expand->str[i] = create_t_str();
		if (((t_token *)expand->node->redirect_list->items[i])->type == TOKEN_HEREDOC)
		{
			handle_heredoc_expansion(expand->env, &(((t_token *)expand->node->redirect_list->items[i])->value));
			i++;
			continue;
		}
		tmp = ((t_token *)expand->node->redirect_list->items[i])->value.str_value;
		if (tmp)
		{
			if (check_for_field_split(tmp))
			{
				split = 1;
				if (check_for_last_exp(expand->node) != -1)
					tmp = ((t_token *)expand->node->redirect_list->items[check_for_last_exp(expand->node)])->value.str_value;
    			if (!check_the_last_arg(tmp))
        			split = 0;
			}
            check_the_word(expand, i, split);
        }
		i++;
	}
	expand->str[i] = NULL;
	expand->node->redirect_list->items = (void **)expand->str;
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

t_expansion	*create_t_expand(t_ast_node *node, t_env **env)
{
	t_expansion	*expand;
	
	expand = ft_malloc(sizeof(t_expansion), 1);
	expand->env = env;
	expand->node = node;
	expand->field = NULL;
	expand->str = NULL;
	return (expand);
}

void	print_arguments(t_array *args)
{
	if (!args)
	{
		printf("No arguments.\n");
		return;
	}

	printf("Arguments after expansion:\n");
	for (size_t i = 0; i < args->length; i++)
	{
		t_str *arg = (t_str *)args->items[i];
		if (arg && arg->value)
		{
			printf("  [%zu]: ", i);
			for (size_t j = 0; arg->value[j]; j++)
			{
				printf("|%s|", arg->value[j]);
				if (arg->value[j + 1])
					printf(", ");
			}
			printf("\n");
		}
		else
		{
			printf("  [%zu]: (empty or NULL)\n", i);
		}
	}
}

int	expand_variables(t_ast_node *node, t_env **env)
{
	t_expansion	*expand;
	
	if (!node || !env || !*env)
		return (0);
	//items = create_copy(node);
	expand = create_t_expand(node, env);
	expand_redirection(expand);
	expand_cmd(expand);// "*"
	expand_path_name_cmd(expand);
	expand_path_name_red(expand);
	check_for_empty_strings(expand);
	check_for_empty_strings_red(expand);
	removes_qouts_cmd(expand);
	removes_qouts_red(expand);
	print_arguments(expand->node->children);
	//printf("here\n");
	return (1);
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
			//expand_redirection((t_ast_node *)node->children->items[i],env);
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
