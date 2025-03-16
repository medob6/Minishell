#ifndef AST_H
#define AST_H 

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

// Function prototypes

// this are example of functions that we must creat

// t_node  *create_ast_node(t_ast_type type, const char *cmd_path);
// void    add_argument(t_node *node, const char *arg);
// void    free_ast(t_node *root);
// void    traverse_ast(const t_node *root, void (*func)(const t_node *));

#endif // AST_H