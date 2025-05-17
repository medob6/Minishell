/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 12:42:36 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 12:42:41 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	append_empty_redirect(t_token **head, t_token **tail, char c)
{
	if (c == '>')
		append_token(head, tail, create_token(NULL, TOKEN_REDIRECT_OUT));
	else if (c == 'a')
		append_token(head, tail, create_token(NULL, TOKEN_APPEND));
	else
		append_token(head, tail, create_token(NULL, TOKEN_REDIRECT_IN));
}

int	handle_redirection(t_token **head, t_token **tail, char c, char *next)
{
	char	nc;

	if (next)
	{
		nc = check_for_operations(next, 0);
		if (nc == '\0')
		{
			append_redirect_arg(head, tail, c, next);
			return (1);
		}
		else
			append_empty_redirect(head, tail, c);
	}
	else
		append_empty_redirect(head, tail, c);
	return (0);
}

int	help_handle_redirection(t_token **head, t_token **tail, char *next, char c)
{
	int	index;

	index = 0;
	if (c == '>' || c == '<' || c == 'a')
		index = handle_redirection(head, tail, c, next);
	return (index);
}

void	check_the_string(t_token **head, t_token **tail, char **s, int *index)
{
	int		i;
	char	c;

	i = -1;
	while (s[*index][++i])
	{
		if (s[*index][i] == '\'' || s[*index][i] == '\"')
			return (create_simple_token(head, tail, s[*index]), (void)0);
		c = check_for_operations(s[*index], i);
		if (c)
		{
			if (c == '|' || c == '(')
				herdoc.nbr_heredoc = 0;
			if (c == 'h')
				*index += handle_heredoc(head, tail, s[*index + 1]);
			else
			{
				i += handle_operator(head, tail, c);
				*index += help_handle_redirection(head, tail, s[*index + 1], c);
			}
			return ;
		}
	}
	create_simple_token(head, tail, s[*index]);
}

t_token	**create_tokens(char **str)
{
	int		i;
	t_token	**tokens;
	t_token	**head;
	t_token	*tail;

	head = get_token();
	tail = NULL;
	if (!str)
		return (NULL);
	tokens = ft_malloc(sizeof(t_token *), 2);
	i = 0;
	while (str[i])
	{
		check_the_string(head, &tail, str, &i);
		i++;
	}
	append_token(head, &tail, create_token(NULL, TOKEN_EOF));
	tokens[0] = *head;
	tokens[1] = NULL;
	*(get_token()) = NULL;
	if (herdoc.exit_sign == 130)
		return (NULL);
	return (tokens);
}
