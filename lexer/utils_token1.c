/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_token1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 12:30:03 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 12:31:17 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	*create_token(char *value, t_token_type type)
{
	t_token	*new;

	new = ft_malloc(sizeof(t_token), 1);
	new->value.str_value = ft_strdup(value);
	new->value.fd_value = -1;
	new->value.theres_qouts = 0;
	new->type = type;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	append_token(t_token **head, t_token **tail, t_token *new)
{
	if (!new)
		return ;
	if (!*head)
		*head = new;
	else
	{
		(*tail)->next = new;
		new->prev = *tail;
	}
	*tail = new;
}

int	handle_operator(t_token **head, t_token **tail, char c)
{
	t_token	*new;

	new = NULL;
	if (c == 'o')
		new = create_token("||", TOKEN_OR);
	else if (c == '|')
		new = create_token("|", TOKEN_PIPE);
	else if (c == 'e')
		new = create_token("&&", TOKEN_AND);
	else if (c == '(')
		new = create_token("(", TOKEN_PARENTESIS_OPEN);
	else if (c == ')')
		new = create_token(")", TOKEN_PARENTESIS_ft_close);
	append_token(head, tail, new);
	return ((c == 'o' || c == 'a' || c == 'h' || c == 'e') + 1);
}

void	create_simple_token(t_token **head, t_token **tail, char *s)
{
	t_token	*new;

	new = create_token(s, TOKEN_WORD);
	append_token(head, tail, new);
}

void	append_redirect_arg(t_token **head, t_token **tail, char c, char *next)
{
	if (c == '>')
		append_token(head, tail, create_token(next, TOKEN_REDIRECT_OUT));
	else if (c == 'a')
		append_token(head, tail, create_token(next, TOKEN_APPEND));
	else
		append_token(head, tail, create_token(next, TOKEN_REDIRECT_IN));
}
