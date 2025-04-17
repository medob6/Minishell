/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:49:52 by salahian          #+#    #+#             */
/*   Updated: 2025/04/17 10:54:36 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	check_for_operations(char *cmd_line, int i)
{
	if (cmd_line[i] == '|' && cmd_line[i + 1] == '|')
		return ('o');
	else if (cmd_line[i] == '&' && cmd_line[i + 1] == '&')
		return ('e');
	else if (cmd_line[i] == '>' && cmd_line[i + 1] == '>')
		return ('a');
	else if (cmd_line[i] == '<' && cmd_line[i + 1] == '<')
		return ('h');
	else if (cmd_line[i] == '|')
		return ('|');
	else if (cmd_line[i] == '>')
		return ('>');
	else if (cmd_line[i] == '<')
		return ('<');
	else if (cmd_line[i] == '$')
		return ('$');
	else if (cmd_line[i] == '*')
		return ('*');
	else if (cmd_line[i] == '(')
		return ('(');
	else if (cmd_line[i] == ')')
		return (')');
	return ('\0');
}

t_token	*create_token(char *value, t_token_type type)
{
	t_token	*new;

	new = ft_malloc(sizeof(t_token), 1);
	new->value = ft_strdup(value);
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

int	handle_operator(t_token **head, t_token **tail, char *s, char c)
{
	t_token	*new;

	new = NULL;
	if (c == 'o')
		new = create_token("||", TOKEN_OR);
	else if (c == '|')
		new = create_token("|", TOKEN_PIPE);
	else if (c == 'e')
		new = create_token("&&", TOKEN_AND);
	else if (c == '$')
		new = create_token(s, TOKEN_TO_EXPAND);
	else if (c == '*')
		new = create_token("*", TOKEN_WILDCARDS);
	else if (c == '(')
		new = create_token("(", TOKEN_PARENTESIS_OPEN);
	else if (c == ')')
		new = create_token(")", TOKEN_PARENTESIS_CLOSE);
	append_token(head, tail, new);
	return ((c == 'o' || c == 'a' || c == 'h' || c == 'e') + 1);
}

void	create_simple_token(t_token **head, t_token **tail, char *s)
{
	t_token	*new;

	new = create_token(s, TOKEN_WORD);
	append_token(head, tail, new);
}

int	handle_herdoc(char *delimiter)
{
	char	*line;
	int		fd;

	if (!delimiter)
		return (0);
	fd = open("/tmp/apah", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (0);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
			&& ft_strlen(line) == ft_strlen(delimiter))
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	return (1);
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

	if (!next)
		return (0);
	nc = check_for_operations(next, 0);
	if (nc == '\0')
	{
		append_redirect_arg(head, tail, c, next);
		return (1);
	}
	else
		append_empty_redirect(head, tail, c);
	return (0);
}

int	handle_heredoc_case(t_token **head, t_token **tail, char *next)
{
	if (next && check_for_operations(next, 0) == '\0')
	{
		handle_herdoc(next);
		append_token(head, tail, create_token(next, TOKEN_HEREDOC));
		return (1);
	}
	append_token(head, tail, create_token(NULL, TOKEN_HEREDOC));
	return (0);
}

void	check_the_string(t_token **head, t_token **tail, char **s, int *index)
{
	int		i;
	char	c;
	char	*next;

	i = 0;
	next = s[*index + 1];
	while (s[*index][i])
	{
		c = check_for_operations(s[*index], i);
		if (c)
		{
			if (c == 'h')
				*index += handle_heredoc_case(head, tail, next);
			else
			{
				i += handle_operator(head, tail, s[*index], c);
				if (c == '>' || c == '<' || c == 'a')
					*index += handle_redirection(head, tail, c, next);
			}
			return ;
		}
		i++;
	}
	create_simple_token(head, tail, s[*index]);
}

int	counter(char **s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

t_token	**create_tokens(char **str)
{
	int		i;
	t_token	**tokens;
	t_token	*head;
	t_token	*tail;

	head = NULL;
	tail = NULL;
	if (!str)
		return (NULL);
	tokens = ft_malloc(sizeof(t_token *), 2);
	i = 0;
	while (str[i])
	{
		check_the_string(&head, &tail, str, &i);
		i++;
	}
	append_token(&head, &tail, create_token(NULL, TOKEN_EOF));
	tokens[0] = head;
	tokens[1] = NULL;
	return (tokens);
}
