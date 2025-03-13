/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 11:18:10 by salahian          #+#    #+#             */
/*   Updated: 2025/03/11 15:09:52 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_token(t_token *t)
{
	t->next = NULL;
	t->type = TOKEN_EOF;
	t->value = NULL;
}

// int	get_next_qout(char *cmd_line, int i)
// {
// 	int		j;
// 	char	c;

// 	c = cmd_line[i];
// 	j = i + 1;
// 	while (cmd_line[j])
// 	{
// 		if (cmd_line[j] == c)
// 			break ;
// 		j++;
// 	}
// 	if (cmd_line[j] == '\0')
// 		return (0);
// 	return (j);
// }

// int	check_qouts(char *cmd_line)
// {
// 	int		i;

// 	i = 0;
// 	while (cmd_line[i])
// 	{
// 		if (cmd_line[i] == '\"' || cmd_line[i] == '\'')
// 			i = get_next_qout(cmd_line, i);
// 		i++;
// 	}
// 	return (1);
// }

int get_next_qout(char *cmd_line, int i)
{
	int		j;
	char	c;

	c = cmd_line[i];
	j = i + 1;
	while (cmd_line[j])
	{
		if (cmd_line[j] == c)
			return (j); // return index of closing quote
		j++;
	}
	return (-1); // Not found
}

int check_qouts(char *cmd_line)
{
	int		i;
	int		next_quote;

	i = 0;
	while (cmd_line[i])
	{
		if (cmd_line[i] == '\"' || cmd_line[i] == '\'')
		{
			next_quote = get_next_qout(cmd_line, i);
			if (next_quote == -1)
				return (0); // unbalanced quote
			i = next_quote; // jump past closing quote
		}
		i++;
	}
	return (1);
}


char	check_for_operations(char *cmd_line, int i)
{
	if (cmd_line[i] == '|')
	{
		if (cmd_line[i + 1] == '|')
			return ('o');
		return ('|');
	}
	else if (cmd_line[i] == '&')
	{
		if (cmd_line[i + 1] == '&')
			return ('e');
		return ('\0');
	}
	else if (cmd_line[i] == '>')
	{
		if (cmd_line[i + 1] == '>')
			return ('a');
		return ('>');
	}
	else if (cmd_line[i] == '<')
	{
		if (cmd_line[i + 1] == '<')
			return ('h');
		return ('<');
	}
	else if (cmd_line[i] == '$')
		return ('$');
	return ('\0');
}

int	handle_pipe_or(t_token **token, char c, int j)
{
	if (c == 'o')
	{
		token[j] = malloc(sizeof(t_token));
		token[j]->value = ft_strdup("||");
		token[j]->type = TOKEN_OR;
		token[j - 1]->next = token[j];
		return (2);
	}
	token[j] = malloc(sizeof(t_token));
	token[j]->value = ft_strdup("|");
	token[j]->type = TOKEN_PIPE;
	token[j - 1]->next = token[j];
	return (1);
}

int	handle_app_rout(t_token **token, char c, int j)
{
	if (c == 'a')
	{
		token[j] = malloc(sizeof(t_token));
		token[j]->value = ft_strdup(">>");
		token[j]->type = TOKEN_APPEND;
		token[j - 1]->next = token[j];
		return (2);
	}
	token[j] = malloc(sizeof(t_token));
	token[j]->value = ft_strdup(">");
	token[j]->type = TOKEN_REDIRECT_OUT;
	token[j - 1]->next = token[j];
	return (1);
}

int	handle_hd_rinp(t_token **token, char c, int j)
{
	if (c == 'h')
	{
		token[j] = malloc(sizeof(t_token));
		token[j]->value = ft_strdup("<<");
		token[j]->type = TOKEN_HEREDOC;
		token[j - 1]->next = token[j];
		return (2);
	}
	token[j] = malloc(sizeof(t_token));
	token[j]->value = ft_strdup("<");
	token[j]->type = TOKEN_REDIRECT_IN;
	token[j - 1]->next = token[j];
	return (1);
}

int	create_token_operator(t_token **token, char c, int j)
{
	if (c == 'o' || c == '|')
		return (handle_pipe_or(token, c, j));
	else if (c == 'a' || c == '>')
		return (handle_app_rout(token, c, j));
	else if (c == 'h' || c == '<')
		return (handle_hd_rinp(token, c, j));
	else if (c == 'e')
	{
		token[j] = malloc(sizeof(t_token));
		token[j]->value = ft_strdup("&&");
		token[j]->type = TOKEN_END;
		token[j - 1]->next = token[j];
		return (2);
	}
	else if (c == '$')
	{
		token[j] = malloc(sizeof(t_token));
		token[j]->value = ft_strdup("$");
		token[j]->type = TOKEN_DOLLAR;
		token[j - 1]->next = token[j];
		return (1);
	}
	return (0);
}

int     counter(char const *s)
{
        int     i;
        int     count;
        int     in_word;

        i = 0;
        count = 0;
        in_word = 0;
        while (s[i])
        {
                if (s[i] == ' ' || s[i] == '\t' || s[i] == '\n')
                {
                        if (in_word)
                        {
                                count++;
                                in_word = 0;
                        }
                }
                else
                        in_word = 1;
                i++;
        }
        if (in_word)
                count++;
        return (count);
}


void	create_tokens(t_token **token, char *cmd_line)
{
	int		i;
	int		start;
	int		j;
	char	operator;
	char	**s;

	token = malloc(sizeof(t_token *) * counter(cmd_line));
	init_token(*token);
	i = 0;
	j = 0;
	while (cmd_line[i])
	{
		start = i;
		if (cmd_line[i] == '\"' || cmd_line[i] == '\'')
			i = get_next_qout(cmd_line, i);
		operator = check_for_operations(cmd_line, i);
		if (operator != '\0' || cmd_line[i] == ' ' || cmd_line[i] == '\t')
		{
			token[j] = malloc(sizeof(t_token));
			token[j]->value = ft_substr(cmd_line, start, i - start);
			token[j]->type = TOKEN_WORD;
			j++;
			i += create_token_operator(token, operator, j);
		}
		i++;
	}
	token[j] = NULL;
}

t_token	**tokenize(char *cmd_line)
{
	t_token **token;

	token = NULL;
	if (!check_qouts(cmd_line))
	{
		ft_putstr("ERROR:\nbalanced brakets.\n", 2);
		return (NULL);
	}
	create_tokens(token, cmd_line);
	return (token);
}