/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:49:52 by salahian          #+#    #+#             */
/*   Updated: 2025/03/13 14:40:41 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int     counter(char **s)
{
    int     i;

    i = 0;
    while (s[i])
        i++;
    return (i);
}

void    check_the_string(t_token **token, char *s)
{
    
}

void	create_tokens(t_token **token, char **str)
{
	int		i;
	int		start;
	int		j;
	char	operator;

	token = malloc(sizeof(t_token *) * counter(str));
	init_token(*token);
	i = 0;
	j = 0;
	while (str[i])
	{
		check_the_string(token, str[i]);
		i++;
	}
	token[j] = NULL;
}


t_token     **token(char **str)
{
    
}