/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:49:52 by salahian          #+#    #+#             */
/*   Updated: 2025/04/10 11:25:16 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	check_for_operations(char *cmd_line, int i)
// {
// 	if (cmd_line[i] == '|')
// 		return ((cmd_line[i + 1] == '|') ? 'o' : '|');
// 	else if (cmd_line[i] == '&')
// 		return ((cmd_line[i + 1] == '&') ? 'e' : '\0');
// 	else if (cmd_line[i] == '>')
// 		return ((cmd_line[i + 1] == '>') ? 'a' : '>');
// 	else if (cmd_line[i] == '<')
// 		return ((cmd_line[i + 1] == '<') ? 'h' : '<');
// 	else if (cmd_line[i] == '$')
// 		return ('$');
// 	else if (cmd_line[i] == '*')
// 		return ('*');
// 	else if (cmd_line[i] == '(')
// 		return ('(');
// 	else if (cmd_line[i] == ')')
// 		return (')');
// 	return ('\0');
// }

// t_token	*create_token(char *value, t_token_type type)
// {
// 	t_token	*new;

// 	new = ft_malloc(sizeof(t_token), 1);
// 	new->value = ft_strdup(value);
// 	new->type = type;
// 	new->next = NULL;
// 	new->prev = NULL;
// 	return (new);
// }

// void	append_token(t_token **head, t_token **tail, t_token *new)
// {
// 	if (!new)
// 		return ;
// 	if (!*head)
// 		*head = new;
// 	else
// 	{
// 		(*tail)->next = new;
// 		new->prev = *tail;
// 	}
// 	*tail = new;
// }

// int	handle_operator(t_token **head, t_token **tail, char c)
// {
// 	t_token	*new;

// 	new = NULL;
// 	if (c == 'o')
// 		new = create_token("||", TOKEN_OR);
// 	else if (c == '|')
// 		new = create_token("|", TOKEN_PIPE);
// 	else if (c == 'a')
// 		new = create_token(">>", TOKEN_APPEND);
// 	else if (c == '>')
// 		new = create_token(">", TOKEN_REDIRECT_OUT);
// 	else if (c == 'h')
// 		new = create_token("<<", TOKEN_HEREDOC);
// 	else if (c == '<')
// 		new = create_token("<", TOKEN_REDIRECT_IN);
// 	else if (c == 'e')
// 		new = create_token("&&", TOKEN_AND);
// 	else if (c == '$')
// 		new = create_token("$", TOKEN_DOLLAR);
// 	else if (c == '*')
// 		new = create_token("*", TOKEN_WILDCARDS);
// 	else if (c == '(')
// 		new = create_token("(", TOKEN_PARENTESIS_OPEN);
// 	else if (c == ')')
// 		new = create_token(")", TOKEN_PARENTESIS_CLOSE);
// 	append_token(head, tail, new);
// 	if (c == 'o' || c == 'a' || c == 'h' || c == 'e')
// 		return (2);
// 	return (1);
// }

// void	create_simple_token(t_token **head, t_token **tail, char *s)
// {
// 	t_token	*new;

// 	new = create_token(s, TOKEN_WORD);
// 	append_token(head, tail, new);
// }

// int	handle_herdoc(char *delimiter)
// {
// 	char	*line;
// 	int		fd;

// 	printf("%s\n", delimiter);
// 	fd = open("/tmp/apah", O_CREAT | O_WRONLY | O_TRUNC, 0644);
// 	if (fd < 0)
// 		return (0);
// 	line = NULL;
// 	ft_putstr("heredoc> ", 1);
// 	while (1)
// 	{
// 		line = get_next_line(0);
// 		if (line)
// 		{
// 			if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
// 				&& line[ft_strlen(delimiter)] == '\n' && ft_strlen(line) != 2)
// 				break ;
// 			write(fd, line, ft_strlen(line));
// 			if (line[ft_strlen(line) - 1] == '\n')
// 				ft_putstr("heredoc> ", 1);
// 			free(line);
// 		}
// 	}
// 	free(line);
// 	close(fd);
// 	return (1);
// }

// void	check_the_string(t_token **head, t_token **tail, char *s)
// {
// 	int		i;
// 	int		is_quote;
// 	char	c;

// 	i = 0;
// 	is_quote = 0;
// 	while (s[i])
// 	{
// 		if (s[i] == '\'' || s[i] == '"')
// 			is_quote = 1;
// 		c = check_for_operations(s, i);
// 		if (c && !is_quote)
// 		{
// 			i += handle_operator(head, tail, c);
// 			continue ;
// 		}
// 		i++;
// 	}
// 	if (!c)
// 		create_simple_token(head, tail, s);
// }

// int	counter(char **s)
// {
// 	int	i;

// 	i = 0;
// 	while (s[i])
// 		i++;
// 	return (i);
// }

// // int   check_is_valid(char **str)
// // {
// //   int   i;

// //   i = 0;
// //   while (str[i])
// //   {
// //     if (check_for_operations(str[i], 0) && str[i + 1] == NULL)
// //       return (0);
// //     if (check_for_operations(str[i], 0) && check_for_operations(str[i + 1],
// 		//0))
// //       return (0);
// //     i++;
// //   }
// //   return (1);
// // }
// t_token	**create_tokens(char **str)
// {
// 	int		i;
// 	t_token	**tokens;
// 	t_token	*head;
// 	t_token	*tail;

// 	if (!str)
// 		return (NULL);
// 	tokens = ft_malloc(sizeof(t_token *), (counter(str) + 1));
// 	head = NULL;
// 	tail = NULL;
// 	i = 0;
// 	while (str[i])
// 	{
// 		check_the_string(&head, &tail, str[i]);
// 		if (tail && tail->type == TOKEN_HEREDOC && str[i + 1])
// 		{
// 			handle_herdoc(str[i + 1]);
// 			append_token(&head, &tail, create_token(str[i + 1],
// 						TOKEN_WORD));
// 			i++;
// 		}
// 		i++;
// 	}
// 	append_token(&head, &tail, create_token(NULL, TOKEN_EOF));
// 	tokens[0] = head;
// 	tokens[1] = NULL;
// 	return (tokens);
// }
char	check_for_operations(char *cmd_line, int i)
{
	if (cmd_line[i] == '|')
		return ((cmd_line[i + 1] == '|') ? 'o' : '|');
	else if (cmd_line[i] == '&')
		return ((cmd_line[i + 1] == '&') ? 'e' : '\0');
	else if (cmd_line[i] == '>')
		return ((cmd_line[i + 1] == '>') ? 'a' : '>');
	else if (cmd_line[i] == '<')
		return ((cmd_line[i + 1] == '<') ? 'h' : '<');
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
	new->value = value ? ft_strdup(value) : NULL;
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
	t_token	*new = NULL;

	if (c == 'o')
		new = create_token("||", TOKEN_OR);
	else if (c == '|')
		new = create_token("|", TOKEN_PIPE);
	else if (c == 'a')
		new = create_token(">>", TOKEN_APPEND);
	else if (c == '>')
		new = create_token(">", TOKEN_REDIRECT_OUT);
	else if (c == 'h')
		new = create_token("<<", TOKEN_HEREDOC);
	else if (c == '<')
		new = create_token("<", TOKEN_REDIRECT_IN);
	else if (c == 'e')
		new = create_token("&&", TOKEN_AND);
	else if (c == '$')
		new = create_token("$", TOKEN_DOLLAR);
	else if (c == '*')
		new = create_token("*", TOKEN_WILDCARDS);
	else if (c == '(')
		new = create_token("(", TOKEN_PARENTESIS_OPEN);
	else if (c == ')')
		new = create_token(")", TOKEN_PARENTESIS_CLOSE);

	append_token(head, tail, new);
	return ((c == 'o' || c == 'a' || c == 'h' || c == 'e') ? 2 : 1);
}

void	create_simple_token(t_token **head, t_token **tail, char *s)
{
	t_token	*new = create_token(s, TOKEN_WORD);
	append_token(head, tail, new);
}

int	handle_herdoc(char *delimiter)
{
	char	*line;
	int		fd;

	fd = open("/tmp/apah", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		return (0);
	line = NULL;
	ft_putstr("heredoc> ", 1);
	while (1)
	{
		line = get_next_line(0);
		if (line)
		{
			if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
				&& line[ft_strlen(delimiter)] == '\n')
				break ;
			write(fd, line, ft_strlen(line));
			if (line[ft_strlen(line) - 1] == '\n')
				ft_putstr("heredoc> ", 1);
			free(line);
		}
	}
	free(line);
	close(fd);
	return (1);
}

void	check_the_string(t_token **head, t_token **tail, char *s, char *next)
{
	int		i = 0;
	char	c;

	while (s[i])
	{
		c = check_for_operations(s, i);
		if (c)
		{
			i += handle_operator(head, tail, c);
			if (c == 'h' && next)
			{
				handle_herdoc(next);
				append_token(head, tail, create_token(next, TOKEN_WORD));
			}
			return ;
		}
		i++;
	}
	create_simple_token(head, tail, s);
}

int	counter(char **s)
{
	int	i = 0;
	while (s[i])
		i++;
	return (i);
}

t_token	**create_tokens(char **str)
{
	int		i = 0;
	t_token	**tokens;
	t_token	*head = NULL;
	t_token	*tail = NULL;

	if (!str)
		return (NULL);
	tokens = ft_malloc(sizeof(t_token *), 2);
	while (str[i])
	{
		char *next = str[i + 1];
		check_the_string(&head, &tail, str[i], next);

		// If we handled heredoc inline, skip the next token
		if (tail && tail->type == TOKEN_HEREDOC && next)
			i++;
		i++;
	}
	append_token(&head, &tail, create_token(NULL, TOKEN_EOF));
	tokens[0] = head;
	tokens[1] = NULL;
	return (tokens);
}

// void print_tokens(t_token *head)
// {
//     while (head)
//     {
//         printf("%s %u\n", head->value, head->type);
//         head = head->next;
//     }
// }
// int	main(void)
// {
// 	char *input[] = {
// 		"echo", "hello", "&&", "cat", "<<", "EOF", NULL
// 	};

// 	t_token **tokens = create_tokens(input);
// 	if (tokens && tokens[0])
// 		print_tokens(tokens[0]);
// 	return (0);
// }