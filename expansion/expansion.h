/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 15:05:10 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 14:44:41 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "../minishell.h"

int		check_pattern(char *s);
char	**create_field(t_ast_node *node);
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
void	check_for_empty_strings_red(t_expansion *expand);
char	**applicate_splitting(char *str, char *field);
char	**create_field_red(t_ast_node *node);
char	*expand_the_value(char *str, t_env **env);
int		match_pattern(char *field, char *pattern, char *name);
char	*help_check_string_get(char **field, char **new, char *get, int *i);
char	*help_expand_wild(struct dirent *dir, char *path, char *get,
			char *field);
void	get_pattern_and_path(char *str, char **path, char **pattern);
char	*expand_wild(char **field, char *path, char *get);
int		check_str(char *str);
int		find_last_slash(char *str);
char	*check_string_is_not_null(char *get);
void	call_expand_wildcard(t_array *child, char **field, int i);
char	**expand_string(char **new, char **field, char **str, int j);
char	*expand_wildcard(char **field, char *str);
void	create_new_field(char **field, char **org, int j, char *exp);
int		get_len(char **str);

char	*expand_the_value(char *str, t_env **env);
char	*handle_expansion(t_env **env, char *str, int *index);
int		is_valid_length(char *tmp, int flag);
char	*update_field_after_splitting(char *field, char *copy_exp);
char	*append(char s);
void	handle_exp_in_heredoc(t_expansion *expand, size_t *i);
void	update_heredoc_value(t_expansion *expand, size_t i);
void	handle_heredoc_expansion(t_env **env, t_value *value);
t_str	*create_t_str(void);
void	application_expansion(t_expansion *expand, char *tmp, size_t i,
			int flag);
int		check_is_valid_split(char *tmp);
char	*copy_and_update(char **field, char *str, int *index, char **new);
char	*update_field_exp(char **field, char *exp, int index);
int		valid(char c);
int		check_first_char(char c);
#endif