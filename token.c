/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:49:52 by salahian          #+#    #+#             */
/*   Updated: 2025/03/14 15:15:31 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	init_token(t_token *t)
// {
// 	t->next = NULL;
// 	t->type = TOKEN_EOF;
// 	t->value = NULL;
// }

// char	check_for_operations(char *cmd_line, int i)
// {
// 	if (cmd_line[i] == '|')
// 	{
// 		if (cmd_line[i + 1] == '|')
// 			return ('o');
// 		return ('|');
// 	}
// 	else if (cmd_line[i] == '&')
// 	{
// 		if (cmd_line[i + 1] == '&')
// 			return ('e');
// 		return ('\0');
// 	}
// 	else if (cmd_line[i] == '>')
// 	{
// 		if (cmd_line[i + 1] == '>')
// 			return ('a');
// 		return ('>');
// 	}
// 	else if (cmd_line[i] == '<')
// 	{
// 		if (cmd_line[i + 1] == '<')
// 			return ('h');
// 		return ('<');
// 	}
// 	else if (cmd_line[i] == '$')
// 		return ('$');
// 	return ('\0');
// }

// int	handle_pipe_or(t_token *token, t_token *last, char c)
// {
// 	if (c == 'o')
// 	{
// 		token = malloc(sizeof(t_token));
// 		token->value = ft_strdup("||");
// 		token->type = TOKEN_OR;
// 		if (!last)
// 			last = token;
// 		else
// 			last->next = token;
// 		return (2);
// 	}
// 	token = malloc(sizeof(t_token));
// 	token->value = ft_strdup("|");
// 	token->type = TOKEN_PIPE;
// 	if (!last)
// 		last = token;
// 	else
// 		last->next = token;
// 	return (1);
// }

// int	handle_app_rout(t_token *token, t_token *last, char c)
// {
// 	if (c == 'a')
// 	{
// 		token = malloc(sizeof(t_token));
// 		token->value = ft_strdup(">>");
// 		token->type = TOKEN_APPEND;
// 		if (!last)
// 			last = token;
// 		else
// 			last->next = token;
// 		return (2);
// 	}
// 	token = malloc(sizeof(t_token));
// 	token->value = ft_strdup(">");
// 	token->type = TOKEN_REDIRECT_OUT;
// 	if (!last)
// 		last = token;
// 	else
// 		last->next = token;
// 	return (1);
// }

// int	handle_hd_rinp(t_token *token, t_token *last, char c)
// {
// 	if (c == 'h')
// 	{
// 		token = malloc(sizeof(t_token));
// 		token->value = ft_strdup("<<");
// 		token->type = TOKEN_HEREDOC;
// 		if (!last)
// 			last = token;
// 		else
// 			last->next = token;
// 		return (2);
// 	}
// 	token = malloc(sizeof(t_token));
// 	token->value = ft_strdup("<");
// 	token->type = TOKEN_REDIRECT_IN;
// 	if (!last)
// 		last = token;
// 	else
// 		last->next = token;
// 	return (1);
// }

// int	create_token_operator(t_token *token, t_token *last, char c)
// {
// 	if (c == 'o' || c == '|')
// 		return (handle_pipe_or(token, last, c));
// 	else if (c == 'a' || c == '>')
// 		return (handle_app_rout(token, last, c));
// 	else if (c == 'h' || c == '<')
// 		return (handle_hd_rinp(token, last, c));
// 	else if (c == 'e')
// 	{
// 		token = malloc(sizeof(t_token));
// 		token->value = ft_strdup("&&");
// 		token->type = TOKEN_END;
// 		if (!last)
// 			last = token;
// 		else
// 			last->next = token;
// 		return (2);
// 	}
// 	else if (c == '$')
// 	{
// 		token = malloc(sizeof(t_token));
// 		token->value = ft_strdup("$");
// 		token->type = TOKEN_DOLLAR;
// 		if (!last)
// 			last = token;
// 		else
// 			last->next = token;
// 		return (1);
// 	}
// 	return (0);
// }

// int	counter(char **s)
// {
// 	int	i;

// 	i = 0;
// 	while (s[i])
// 		i++;
// 	return (i);
// }

// size_t	ft_strcpy(char *dst, char *src, size_t dstsize)
// {
// 	size_t	i;

// 	i = 0;
// 	if (dstsize != 0)
// 	{
// 		while ((src[i]) && (i < dstsize - 1))
// 		{
// 			dst[i] = src[i];
// 			i++;
// 		}
// 		dst[i] = '\0';
// 	}
// 	return (1);
// }

// // void	create_simple_token(t_token *token, t_token *last, char *s)
// // {
// // 	int	len;

// // 	len = ft_strlen(s);
// // 	printf("%s\n", s);
// // 	ft_strcpy(token->value, s, len);
// // 	token->type = TOKEN_WORD;
// // 	if (!last)
// // 		last = token;
// // 	else
// // 		last->next = token;
// // }

// void	create_simple_token(t_token *token, t_token *last, char *s)
// {
// 	int	len;

// 	len = ft_strlen(s);
// 	token = malloc(sizeof(t_token));
// 	token->value = malloc(len + 1);  // Allocate memory first
// 	if (!token->value)
// 		return ;

// 	printf("%s\n", s);
// 	ft_strcpy(token->value, s, len);
// 	token->type = TOKEN_WORD;

// 	if (!last)
// 		last = token;
// 	else
// 		last->next = token;
// }


// void	check_the_string(t_token *token, t_token *last, char *s)
// {
// 	int		i;
// 	int		is_qout;
// 	char	c;

// 	i = 0;
// 	is_qout = 0;
// 	while (s[i])
// 	{
// 		if (s[i] == '\'' || s[i] == '"')
// 			is_qout = 1;
// 		c = check_for_operations(s, i);
// 		if (c && !is_qout)
// 		{
// 			if (!create_token_operator(token, last, c))
// 				return ;
// 		}
// 		i++;
// 	}
// 	create_simple_token(token, last, s);
// }

// t_token	**create_tokens(char **str)
// {
// 	int		i;
// 	int		j;
// 	t_token	**token;

// 	token = malloc(sizeof(t_token *) * counter(str));
// 	// init_token(*token);
// 	i = 0;
// 	j = 0;
// 	//token[j]->next = NULL;
// 	while (str[i])
// 	{
// 		if (j)
// 			check_the_string(token[j], token[j - 1], str[i]);
// 		else
// 			check_the_string(token[j], NULL, str[i]);
// 		j++;
// 		i++;
// 	}
// 	token[j] = NULL;
// 	return (token);
// }

char check_for_operations(char *cmd_line, int i)
{
    if (cmd_line[i] == '|')
        return (cmd_line[i + 1] == '|') ? 'o' : '|';
    else if (cmd_line[i] == '&')
        return (cmd_line[i + 1] == '&') ? 'e' : '\0';
    else if (cmd_line[i] == '>')
        return (cmd_line[i + 1] == '>') ? 'a' : '>';
    else if (cmd_line[i] == '<')
        return (cmd_line[i + 1] == '<') ? 'h' : '<';
    else if (cmd_line[i] == '$')
        return '$';
	else if (cmd_line[i] == '*')
		return '*';
	else if (cmd_line[i] == '(')
		return '(';
	else if (cmd_line[i] == ')')
		return ')';
    return '\0';
}

t_token *create_token(char *value, t_token_type type)
{
    t_token *new;

	new = malloc(sizeof(t_token));
    new->value = strdup(value);
    new->type = type;
    new->next = NULL;
    return new;
}

void append_token(t_token **head, t_token **tail, t_token *new)
{
    if (!new) 
		return;
    if (!*head)
        *head = new;
    else
        (*tail)->next = new;
    *tail = new;
}

int handle_operator(t_token **head, t_token **tail, char c)
{
    t_token *new;

	new = NULL;
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
		new = create_token("&&", TOKEN_END);
    else if (c == '$')
		new = create_token("$", TOKEN_DOLLAR);
	else if (c == '*')
		new = create_token("*", TOKEN_WILDCARDS);
	else if (c == '(')
		new = create_token("(", TOKEN_PARENTESIS_OPEN);
	else if (c == ')')
		new = create_token(")", TOKEN_PARENTESIS_CLOSE);
    append_token(head, tail, new);
    if (c == 'o' || c == 'a' || c == 'h' || c == 'e')
		return (2);
	return (1);
}

void create_simple_token(t_token **head, t_token **tail, char *s)
{
    t_token *new = create_token(s, TOKEN_WORD);
    append_token(head, tail, new);
}

void check_the_string(t_token **head, t_token **tail, char *s)
{
    int i;
    int is_quote;
    char c;

	i = 0;
	is_quote = 0;
    while (s[i])
    {
        if (s[i] == '\'' || s[i] == '"')
			is_quote = 1;
        c = check_for_operations(s, i);
        if (c && !is_quote)
        {
            i += handle_operator(head, tail, c);
			continue ;
        }
        i++;
    }
	if (!c)
    	create_simple_token(head, tail, s);
}

int counter(char **s)
{
    int i = 0;
    while (s[i])
		i++;
    return i;
}

t_token **create_tokens(char **str)
{
    int i;
    t_token **tokens;
    t_token *head;
    t_token *tail;

	if (!str)
		return (NULL);
	tokens = malloc(sizeof(t_token *) * (counter(str) + 1));
	head = NULL;
	tail = NULL;
	i = 0;
    while (str[i])
    {
        check_the_string(&head, &tail, str[i]);
        i++;
    }

    tokens[0] = head;
    tokens[1] = NULL;
    return tokens;
}
