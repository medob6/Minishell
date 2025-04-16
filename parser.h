#ifndef PARSER_H
# define PARSER_H

# include "array.h"
# include "minishell.h"

typedef enum e_ast_type
{
	AST_COMPOUNED_CMD,
	AST_CMD,
	AST_SIMPLE_CMD,
	AST_PIPELINE,
	AST_AND,      // command && command
	AST_OR,       // command || command
	AST_SUBSHELL, // ( command_list )
	AST_ERROR     // for syntax errors
}			t_ast_type;

typedef struct s_ast_node
{
	t_ast_type type;        // ls -l -e| cat -e
	t_array *children;      // array of child t_ast_node*
	t_array *redirect_list; // array of redirection nodes
	char *error_message;    // null if no error
}			t_ast_node;


char		*get_value(int type);

t_ast_node	*creat_ast_node(int node_type);
void		array_push(t_array **list, void *new_item);
void		add_child(t_ast_node *parent, void *new_child);
void		add_redirect(t_ast_node *parent, t_token *redir);

// Also, make sure to declare all other used functions:
t_ast_node	*parse_tokens(t_token *token);

void		advance_token(t_token **token);

bool		is_redirction(t_token_type token_type);


/* FUNCTIONS */
t_ast_node	*compound_cmd(t_token **token)
;

#endif