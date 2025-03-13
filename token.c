/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 13:49:52 by salahian          #+#    #+#             */
/*   Updated: 2025/03/13 18:07:31 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	init_token(t_token *t)
// {
// 	t->next = NULL;
// 	t->type = TOKEN_EOF;
// 	t->value = NULL;
// }

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

int	handle_pipe_or(t_token *token, t_token *last, char c)
{
	if (c == 'o')
	{
		token = malloc(sizeof(t_token));
		token->value = ft_strdup("||");
		token->type = TOKEN_OR;
		token->next = last;
		return (2);
	}
	token = malloc(sizeof(t_token));
	token->value = ft_strdup("|");
	token->type = TOKEN_PIPE;
	token->next = last;
	return (1);
}

int	handle_app_rout(t_token *token, t_token *last, char c)
{
	if (c == 'a')
	{
		token = malloc(sizeof(t_token));
		token->value = ft_strdup(">>");
		token->type = TOKEN_APPEND;
		token->next = last;
		return (2);
	}
	token = malloc(sizeof(t_token));
	token->value = ft_strdup(">");
	token->type = TOKEN_REDIRECT_OUT;
	token->next = last;
	return (1);
}

int	handle_hd_rinp(t_token *token, t_token *last, char c)
{
	if (c == 'h')
	{
		token = malloc(sizeof(t_token));
		token->value = ft_strdup("<<");
		token->type = TOKEN_HEREDOC;
		token->next = last;
		return (2);
	}
	token = malloc(sizeof(t_token));
	token->value = ft_strdup("<");
	token->type = TOKEN_REDIRECT_IN;
	token->next = last;
	return (1);
}

int	create_token_operator(t_token *token, t_token *last, char c)
{
	if (c == 'o' || c == '|')
		return (handle_pipe_or(token, last, c));
	else if (c == 'a' || c == '>')
		return (handle_app_rout(token, last, c));
	else if (c == 'h' || c == '<')
		return (handle_hd_rinp(token, last, c));
	else if (c == 'e')
	{
		token = malloc(sizeof(t_token));
		token->value = ft_strdup("&&");
		token->type = TOKEN_END;
		token->next = last;
		return (2);
	}
	else if (c == '$')
	{
		token = malloc(sizeof(t_token));
		token->value = ft_strdup("$");
		token->type = TOKEN_DOLLAR;
		token->next = last;
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

size_t  ft_strcpy(char *dst, char *src, size_t dstsize)
{
        size_t  i;

        i = 0;
        if (dstsize != 0)
        {
                while ((src[i]) && (i < dstsize - 1))
                {
                        dst[i] = src[i];
                        i++;
                }
                dst[i] = '\0';
        }
        return (1);
}

void	create_simple_token(t_token *token, t_token *last, char *s)
{
	int		len;
	
	len = ft_strlen(s);
	printf("%s\n", s);
	ft_strcpy(token->value, s, len);
	token->type = TOKEN_WORD;
	token->next = last;
}

void    check_the_string(t_token *token, t_token *last, char *s)
{
	int	i;
	int	is_qout;
	char	c;

	i = 0;
	is_qout = 0;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
			is_qout = 1;
		c = check_for_operations(s, i);
		if (c && !is_qout)
		{
			if (!create_token_operator(token, last, c))
				return ;
		}
		i++;
	}
	create_simple_token(token, last, s);
}

t_token		**create_tokens(char **str)
{
	int		i;
	int		j;
	t_token **token;

	token = malloc(sizeof(t_token *) * counter(str));
	//init_token(*token);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (j)
			check_the_string(token[j], token[j - 1], str[i]);
		else
			check_the_string(token[j], NULL, str[i]);
		j++;
		i++;
	}
	token[j] = NULL;
	return (token);
}
