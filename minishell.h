#ifndef MINISHELL_H
# define MINISHELL_H
# include "utils_lib/get_next_line.h"
# include "utils_lib/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>

# define AMBIGUOUS_REDIRECTION -5

typedef struct s_g_data
{
	int				status;
	char			*prompt;
}					GlobalData;

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
	TOKEN_PARENTESIS_OPEN, //(
	TOKEN_PARENTESIS_CLOSE,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_PIPE,         // |
	TOKEN_REDIRECT_IN,  // <
	TOKEN_REDIRECT_OUT, // >
	TOKEN_APPEND,       // >>
	TOKEN_HEREDOC,      // <<
	TOKEN_EOF
}					t_token_type;

// void print_ast(t_ast_node *node, int depth);
// typedef struct s_token
// {
// 	char *value; // The actual token string
// 	t_token_type	type;
// 	struct s_token *prev; // The type of token
// 	struct s_token *next; // Linked list to store multiple tokens
// }					t_token;

// typedef enum s_bit_mask
//{
//	ORIGINAL = 0,
//	EXPANDED = 1,
//	SINGLE_QOUT = 2,
//	DOUBLE_QOUT = 4
//}	t_bit_mask;

typedef struct s_value
{
	char			*str_value;
	int				fd_value;
	int				theres_qouts;
}					t_value;

// TODO USE THIS STUCT FOR TOKENS INSTEAD
typedef struct s_token
{
	t_value			value;
	t_token_type	type;
	struct s_token *prev; // The type of token
	struct s_token *next; // Linked list to store multiple tokens
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
// ls | fk && kd | ls -l -e
typedef struct s_ast_node
{
	t_ast_type		type;
	t_array *children; // {ls , -l , -e}
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
// void print_ast(t_ast_node *node, int depth);
t_ast_node			*subshell(t_token **token);
t_ast_node			*command(t_token **token);
bool				paranteses_symetric(t_token **token);
void print_ast(t_ast_node *node, int depth);
char				*get_value_ast(int type);
void				print_token(t_token *head);
void				*ft_malloc(int size, int bytes);
t_gar				**garbage_list(void);
int					ft_print(char *c, int fd);
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
char				*get_value(int type);

int					expand_ast(t_ast_node *node, t_env **env);
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
char				*get_name_heredoc(void);

// export a='"'  export b='*'  echo "$a$b$a"
// error message = "$a$b$a" and i get ""*"" --> "*"

int					check_value(char *number);
t_env				*create_the_main_list(char **envp, int shlvl);
bool				is_correct_nbr(char *number);
int					ft_atoi(char *str);
int					*get_last_status(void);
#endif