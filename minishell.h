/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:12:17 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/19 09:19:07 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "utils_lib/get_next_line.h"
# include "utils_lib/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_garbag
{
	void			*addr;
	struct s_garbag	*next;
}					t_gar;

typedef struct s_env
{
	char			*key;
	char			*value;
	bool			value_set;
	struct s_env	*next;
}					t_env;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PARENTESIS_OPEN,
	TOKEN_PARENTESIS_ft_close,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}					t_token_type;

typedef struct s_value
{
	char			*str_value;
	int				fd_value;
	int				theres_qouts;
}					t_value;

typedef struct s_token
{
	t_value			value;
	t_token_type	type;
	struct s_token	*prev;
	struct s_token	*next;
}					t_token;

typedef struct s_array
{
	void			**items;
	size_t			length;
	size_t			capacity;
}					t_array;

typedef struct s_string
{
	char			**value;
	int				fd;
	t_token_type	type;
}					t_str;

typedef enum e_ast_type
{
	AST_COMPOUNED_CMD,
	AST_CMD,
	AST_SIMPLE_CMD,
	AST_PIPELINE,
	AST_AND,
	AST_OR,
	AST_SUBSHELL,
	AST_ERROR
}					t_ast_type;

typedef struct s_ast_node
{
	t_ast_type		type;
	t_array			*children;
	t_array			*redirect_list;
}					t_ast_node;

typedef struct s_expansion
{
	t_ast_node		*node;
	t_env			**env;
	char			**field_cmd;
	char			**field_red;
	t_str			**str;
}					t_expansion;

typedef struct s_heredoc
{
	int				exit_sign;
	int				no_file_name;
	int				fd_heredoc;
	int				old_fd;
	int				nbr_heredoc;
}					t_herdoc;

extern t_herdoc		g_herdoc;

t_env				**get_env_head_addres(t_env **address);
int					print_str_fd(char *s, int fd);
t_ast_node			*subshell(t_token **token);
t_ast_node			*command(t_token **token);
bool				paranteses_symetric(t_token **token);
void				print_ast(t_ast_node *node, int depth);
char				*get_value_ast(int type);
void				print_token(t_token *head);
void				*ft_malloc(int size, int bytes);
t_gar				**garbage_list(void);
t_env				*create_the_main_list(char **envp, int shlvl);

t_array				*creat_array(void);
void				array_extend(t_array *arr);
void				array_push(t_array **list, void *new_item);

t_ast_node			*parse_tokens(t_token *token);
t_ast_node			*creat_ast_node(int node_type);
void				add_child(t_ast_node *parent, void *new_child);
void				add_redirect(t_ast_node *parent, t_token *redir);
void				advance_token(t_token **token);
bool				is_redirction(t_token_type token_type);
t_ast_node			*compound_cmd(t_token **token, t_ast_type token_type);

// debuging functions

int					is_valid_identifier(char *s);
int					ft_export(char **args, t_env **env, int fd);
size_t				ft_strlcpy(char *dst, const char *src, size_t dstsize);
void				print_err(char *err, char *str);
void				ft_free(void *address);
int					check_for_next_one(char *str, int j);
int					take_inside_qout(char **s, char *str, int j);
char				*append_char(char *old_str, char c);
t_token				**create_tokens(char **str);
char				**lexer(char *cmd_line);
char				*expand_the_value(char *str, t_env **env);
char				*get_name_heredoc(char *str);
int					match_pattern(char *field, char *pattern, char *name);

int					check_value(char *number);
t_env				*create_the_main_list(char **envp, int shlvl);
bool				is_correct_nbr(char *number);
int					ft_atoi(char *str);
int					*get_last_status(void);
void				handler(int sig);
void				ft_lstclear(t_gar **lst);
void				panic(char *err);
void				update_shlvl(t_env *env_list);
int					check_value(char *number);
bool				is_correct_nbr(char *number);
void				free_env_list(t_env *env);
t_env				*create_new(char *s, int sep);
void				add_or_update_env(char *arg, int sep, t_env **env);
void				handle_new_env(t_env **env, char *arg, int sep);
void				handle_existing_env(t_env *existing, char *arg, int sep);
void				handle_existing_env(t_env *existing, char *arg, int sep);
void				handle_new_env(t_env **env, char *arg, int sep);
void				add_or_update_env(char *arg, int sep, t_env **env);
void				swap_env_nodes(t_env *a, t_env *b);
int					find_equal_pos(char *s);
int					check_is_okey(char *s);
t_env				*update_existing_env(t_env *env, char *arg, int sep);
void				add_the_new(t_env **env, t_env *new);
void				ft_close(int fd);
int					ft_pwd(t_env **env_list, int fd);
void				init_global_var(void);
t_env				**get_env_head_addres(t_env **address);

#endif