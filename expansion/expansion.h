/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:05:10 by salahian          #+#    #+#             */
/*   Updated: 2025/05/11 15:19:48 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "../minishell.h"

//typedef struct s_string
//{
//	char	**value;
//	int		fd;
//	t_token_type	type;
//}	t_str;

//typedef struct s_expansion
//{
//	t_ast_node		*node;
//	t_env			**env;
//	char			**field; // null
//	t_str			**str;
//}	t_expansion;

char	**create_field(t_ast_node *node);
int		check_the_last_arg(char *tmp);
int		check_for_last_exp(t_ast_node *node);
int		check_for_field_split(char *tmp);
char	*append_char(char *old_str, char c);
void	expand_path_name_cmd(t_expansion *expand);
void	expand_path_name_red(t_expansion *expand);
void	removes_qouts_cmd(t_expansion *expand);
void	removes_qouts_red(t_expansion *expand);
int		check_for_next_one(char *str, int j);
int		take_inside_qout(char **s, char *str, int j);
void	check_for_empty_strings(t_expansion *expand);
int		search_for(char *str, char c);
char	**field_splitting(char *str, char *charset);
int		expand_ast(t_ast_node *node, t_env **env);
void	check_for_empty_strings_red(t_expansion *expand);
char	**applicate_splitting(char *str, char *field);
char	**create_field_red(t_ast_node *node);
char	*expand_the_value(char *str, t_env **env);

#endif