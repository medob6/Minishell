/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:49:52 by salahian          #+#    #+#             */
/*   Updated: 2025/03/17 15:36:13 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
    new->prev = NULL;
    return new;
}

void append_token(t_token **head, t_token **tail, t_token *new)
{
    if (!new) 
		return;
    if (!*head)
        *head = new;
    else
    {
        (*tail)->next = new;
        new->prev = *tail;
    }
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
 
int   check_is_valid(char **str)
{
  int   i;

  i = 0;
  while (str[i])
  {
    if (check_for_operations(str[i], 0) && str[i + 1] == NULL)
      return (0);
    if (check_for_operations(str[i], 0) && check_for_operations(str[i + 1], 0))
      return (0);
    i++;
  }
  return (1);
}
t_token **create_tokens(char **str)
{
    int i;
    t_token **tokens;
    t_token *head;
    t_token *tail;

	if (!str)
		return (NULL);
  if (!check_is_valid(str))
  {
    ft_putstr("appah: syntax error near unexpected token\n", 2);
    return (NULL);
  }
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
