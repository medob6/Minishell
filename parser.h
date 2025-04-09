#ifndef PARSER_H
#define PARSER_H

// we shouldent have a header file called minishell , it dosnt make sense , soo for better readability we gona improve nameing 
#include "minishell.h"

typedef enum e_ast_type
{
	AST_COMPOUNED_CMD,
	AST_SIMPLE_CMD,
	AST_PIPELINE,	  // command | command | ...
	AST_AND,		  // command && command
	AST_OR,			  // command || command
	AST_SUBSHELL,	  // ( command_list )
	AST_ERROR		  // for syntax errors
}
t_ast_type;


typedef struct s_array
{
	void *data;
	int size;
	int capacity;
} t_array;

typedef struct s_ast_node {
	t_ast_type		type; // ls -l -e| cat -e
	t_array			*children;       // array of child t_ast_node*
	t_array			*redirect_list;  // array of redirection nodes
	char			*error_message;  // null if no error
}	t_ast_node;


// typedef enum {
//     SIMPLE_COMMAND,
//     PIPE,
//     REDIRECTION,
//     LOGICAL_OP,
//     SUBSHELL
// } NodeType;

// typedef struct ASTNode {
//     NodeType type;
//     char **command; // For SIMPLE_COMMAND
//     struct ASTNode *left; // For PIPE, LOGICAL_OP
//     struct ASTNode *right; // For PIPE, LOGICAL_OP
//     char *file; // For REDIRECTION
// } ASTNode;

#endif // AST_H