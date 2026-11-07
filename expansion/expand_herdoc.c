/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_herdoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 14:34:24 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 14:36:12 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expansion.h"

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

void	handle_heredoc_expansion(t_env **env, t_value *value)
{
	int		fd1;
	int		fd;
	char	*line;
	char	*str;

	str = get_name_heredoc(value->str_value);
	fd1 = open(str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	line = get_next_line(value->fd_value);
	while (line)
	{
		if (search_for(line, '$') && !value->theres_qouts)
			write(fd1, expand_heredoc(env, line), ft_strlen(expand_heredoc(env,
						line)));
		else
			write(fd1, line, ft_strlen(line));
		free(line);
		line = get_next_line(value->fd_value);
	}
	free(line);
	close(fd1);
	fd = open(str, O_RDONLY);
	unlink(str);
	close(value->fd_value);
	value->fd_value = fd;
}

void	update_heredoc_value(t_expansion *expand, size_t i)
{
	char			*str;
	int				fd;
	t_token_type	type;

	str = ((t_token *)expand->node->redirect_list->items[i])->value.str_value;
	fd = ((t_token *)expand->node->redirect_list->items[i])->value.fd_value;
	type = ((t_token *)expand->node->redirect_list->items[i])->type;
	expand->str[i]->value[0] = ft_strdup(str);
	expand->str[i]->value[1] = NULL;
	expand->str[i]->fd = fd;
	expand->str[i]->type = type;
}

void	handle_exp_in_heredoc(t_expansion *expand, size_t *i)
{
	handle_heredoc_expansion(expand->env,
		&(((t_token *)expand->node->redirect_list->items[*i])->value));
	update_heredoc_value(expand, *i);
	(*i)++;
}
