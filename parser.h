#ifndef PARSER_H
# define PARSER_H

# include "minishell.h"

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
}				t_ast_type;

typedef struct s_ast_node
{
	t_ast_type	type;
	t_array		*children;
	t_array		*redirect_list;
	char		*error_message;
}				t_ast_node;

// parser functions
t_ast_node		*parse_tokens(t_token *token);
t_ast_node		*creat_ast_node(int node_type);
void			add_child(t_ast_node *parent, void *new_child);
void			add_redirect(t_ast_node *parent, t_token *redir);
void			advance_token(t_token **token);
bool			is_redirction(t_token_type token_type);
t_ast_node		*compound_cmd(t_token **token);

// debuging functions
char			*get_value(int type);

// me
int	traverse_ast(t_ast_node *node, t_env **env);
#endif