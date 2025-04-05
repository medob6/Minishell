#ifndef PARSER_H
#define PARSER_H

// we shouldent have a header file called minishell , it dosnt make sense , soo for better readability we gona improve nameing 
#include "minishell.h"

typedef enum e_ast_type {
    AST_COMMAND,
    AST_PIPE,
    AST_AND,
    AST_OR,
    AST_REDIRECT_IN,
    AST_REDIRECT_OUT,
    AST_HEREDOC,
    AST_APPEND,
    // Add other types as needed
} t_ast_type;

// Structure for AST nodes
typedef struct s_node {
    t_ast_type type;
    char *cmd_path;   // Path to the command
    char **args;      // Null-terminated array of arguments
    struct s_node *left;
    struct s_node *right;
} t_node;


#endif // AST_H