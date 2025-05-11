/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 11:37:47 by salahian          #+#    #+#             */
/*   Updated: 2025/05/11 15:51:08 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

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

static char	*help_expand_the_value(char *str, char *value, int var_len)
{
	char	next;

	next = str[var_len];
	if (next == '\0' || !(next == '_' || (next >= '0' && next <= '9') ||
		(next >= 'a' && next <= 'z') || (next >= 'A' && next <= 'Z')))
	{
		if (str[var_len] != '\0')
			str = ft_strjoin(ft_strdup(value), &str[var_len]);
		else
			str = ft_strdup(value);
		return (str);
	}
	return (NULL);
}

char  *expand_the_value(char *str, t_env **env)
{
	t_env	*tmp;
	int		var_len;
	int		index;
	char	*old_str;

	old_str = str;
	str = &str[1];
	tmp = *env;
	while (tmp)
	{
		var_len = ft_strlen(tmp->key);
		if (ft_strncmp(str, tmp->key, var_len) == 0)
		{
			if (help_expand_the_value(str, tmp->value, var_len))
				return (help_expand_the_value(str, tmp->value, var_len));
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


char *append_char(char *old_str, char c)
{
	char buf[2];
	char *tmp;

	buf[0] = c;
	buf[1] = '\0';
	tmp = ft_strjoin(old_str, buf);
	return (tmp);
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

static char	*copy_and_update(char **field, char *str, int *index, char **new)
{
	int		start;
	char	*tmp;

	start = *index;
	tmp = append(str[(*index)++]);
	*new = ft_strjoin(*new, update_field(field, tmp, start));
	return (tmp);
}

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
	while (str[index])
	{
		if (str[index] == '$' && (*field)[index] != '2')
		{
			start = index;
			tmp = handle_expansion(env, str, &index);
			new = ft_strjoin(new, update_field_exp(field, tmp, start));
			old_str = ft_strjoin(old_str, tmp);
		}
		else
			old_str = ft_strjoin(old_str, copy_and_update(field, str, &index, &new));
	}
	*field = new;
	return (old_str);
}


//static char	*help_check_the_word(char **field, t_env **env, char *str)
//{
//	int		start;
//	char	*tmp;
//	int		index;
//	char	*old_str;
//	char *new;
//	char	*hold;

//	index = 0;
//	old_str = ft_strdup("");
//	hold = ft_strdup("");
//	new = ft_strdup("");
//	while (str[index])
//	{
//		if (str[index] == '$' && (*field)[index] != '2')
//		{		
//			start = index;
//			tmp = handle_expansion(env, str, &index);
//			new = ft_strjoin(new, update_field_exp(field, tmp, start));
//			tmp = ft_strjoin(old_str, tmp);
//		}
//		else
//		{
//			start = index;
//			tmp = append(str[index++]);
//			new = ft_strjoin(new, update_field(field, tmp, start));
//			tmp = ft_strjoin(old_str, tmp);
//		}
//		old_str = tmp;
//		hold = new;
//	}
//	*field = hold;
//	return (old_str);
//}
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

char	*update_field_after_splitting(char *field, char *copy_exp)
{
	char	*new;
	int		i;
	int		j;
	int		count;
	
	i = 0;
	count = 0;
	while (copy_exp[i])
	{
		if ((copy_exp[i] == ' ' || copy_exp[i] == '\t') && field[i] == '1')
			count++;
		i++;
	}
	new = ft_calloc(sizeof(char), (ft_strlen(field) - count) + 1);
	j = 0;
	i = 0;
	while (copy_exp[i])
	{
		if ((copy_exp[i] == ' ' || copy_exp[i] == '\t') && field[i] == '1')
			i++;
		else
			new[j++] = field[i++];
	}
	new[j] = '\0';
	return (new);
}

static void	update_child_value(t_expansion *expand, int i, char *expanded, int field_split)
{
	char	*copy_exp;

	copy_exp = ft_strdup(expanded);
	if (field_split)
	{
		expand->str[i]->value = applicate_splitting(expanded, expand->field[i]);
		expand->field[i] = update_field_after_splitting(expand->field[i], copy_exp);
	}
	else
	{
		expand->str[i]->value[0] = expanded;
		expand->str[i]->value[1] = NULL;
	}
		
}


static void	update_token_value(t_expansion *expand, int i, char *expanded, int field_split)
{
	char	*copy_exp;

	copy_exp = ft_strdup(expanded);
	if (field_split)
	{
		expand->str[i]->value = applicate_splitting(expanded, expand->field[i]);
		expand->field[i] = update_field_after_splitting(expand->field[i], copy_exp);
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
		str = ((t_token *)expand->node->redirect_list->items[i])->value.str_value;
	
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
	str->value = ft_malloc(sizeof(char *), 2);
	if (!str->value)
		return (NULL);
	str->value[0] = NULL;
	str->fd = -1;
	str->type = -1;
	return (str);
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

void	application_expansion(t_expansion *expand, char *tmp, size_t i, int flag)
{
	int		split;

	split = 0;
	if (check_for_field_split(tmp))
	{
		split = 1;
		if (check_for_last_exp(expand->node) != -1)
		{
			if (flag)
				tmp = ((t_token *)expand->node->redirect_list->items[check_for_last_exp(expand->node)])->value.str_value;
			else
				tmp = (char *)expand->node->children->items[check_for_last_exp(expand->node)];	
		}
    	if (!check_the_last_arg(tmp))
        	split = 0;
	}
    check_the_word(expand, i, split);
}

void expand_cmd(t_expansion *expand)
{
	size_t		i;
	char	*tmp;
	
	i = 0;
	if (!expand->node->children)
		return ;
	expand->field = create_field(expand->node);
	expand->str = ft_malloc(sizeof(t_str *), expand->node->children->length + 1);
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

void expand_redirection(t_expansion *expand)
{
	size_t		i;
	char	*tmp;
	
	i = 0;
	if (!expand->node->redirect_list)
		return ;
	expand->field = create_field_red(expand->node);
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
			application_expansion(expand, tmp, i, 1);
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
	print_arguments(expand->node->redirect_list);
	//printf("here\n");
	return (1);
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
