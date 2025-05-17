/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:02:12 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 12:39:41 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "../minishell.h"

t_token	**get_token(void);
t_token	**create_tokens(char **str);
t_token	*create_token(char *value, t_token_type type);
void	read_from_herdoc(char *delemeter);
void	append_token(t_token **head, t_token **tail, t_token *new);
char	check_for_operations(char *cmd_line, int i);
void	append_redirect_arg(t_token **head, t_token **tail, char c, char *next);
int		check_for_q(char *str);
void	handle_error(char *str);
int		handle_heredoc(t_token **head, t_token **tail, char *next);
int		is_delemeter(char *line, char *delemeter);
void	process_input(char *delemeter);
char	*removes_qouts_heredoc(char *str);
int		handle_operator(t_token **head, t_token **tail, char c);
void	close_all_files(t_token **head);
void	create_simple_token(t_token **head, t_token **tail, char *s);
char	**lexer(char *cmd_line);
int		handle_normal_words(char **str, char *cmd_line, int *i);
int		is_operator(const char *c, int i);
int		check_quote(char *cmd_line);
int		handle_par(char **str, char *c, int *i);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
int		check_quotes(char *cmd_line);
int		count_words(char const *s);
int		skip_quotes(const char *s, int i, int *in_word);

#endif