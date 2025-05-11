/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:02:12 by salahian          #+#    #+#             */
/*   Updated: 2025/05/11 15:12:51 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
#define LEXER_H

#include "../minishell.h"

//typedef enum e_token_type
//{
//	TOKEN_WORD,
//	TOKEN_PARENTESIS_OPEN, //(
//	TOKEN_PARENTESIS_CLOSE,
//	TOKEN_AND,
//	TOKEN_OR,
//	TOKEN_PIPE,         // |
//	TOKEN_REDIRECT_IN,  // <
//	TOKEN_REDIRECT_OUT, // >
//	TOKEN_APPEND,       // >>
//	TOKEN_HEREDOC,      // <<
//	TOKEN_EOF
//}					t_token_type;

//typedef struct s_value
//{
//	char			*str_value;
//	int				fd_value;
//	int				theres_qouts;
//}					t_value;

//// TODO USE THIS STUCT FOR TOKENS INSTEAD
//typedef struct s_token
//{
//	t_value			value;
//	t_token_type	type;
//	struct s_token *prev; // The type of token
//	struct s_token *next; // Linked list to store multiple tokens
//}					t_token;

t_token				**create_tokens(char **str);
char				**lexer(char *cmd_line);
char	*get_name_heredoc(void);

#endif