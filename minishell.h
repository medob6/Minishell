#ifndef MINISHELL_H
# define MINISHELL_H
# include "utils_lib/get_next_line.h"
# include "utils_lib/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>


typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE, // |
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_REDIRECT_IN,  // <
	TOKEN_REDIRECT_OUT, // >
	TOKEN_PARENTESIS_CLOSE,
	TOKEN_PARENTESIS_OPEN,
	TOKEN_APPEND,  // >>
	TOKEN_HEREDOC, // <<
	TOKEN_DOLLAR,  // $
	TOKEN_EOF
}			t_token_type;

typedef struct s_token
{
	char *value;          // The actual token string
	t_token_type type;    // The type of token
	struct s_token *next; // Linked list to store multiple tokens
}			t_token;

#endif
