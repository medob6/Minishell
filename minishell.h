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

typedef struct s_array
{
	void	**items;
	size_t	length;
	size_t	capacity;
}	t_array;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_PIPE,         // |
	TOKEN_REDIRECT_IN,  // <
	TOKEN_REDIRECT_OUT, // >
	TOKEN_APPEND,       // >>
	TOKEN_HEREDOC,      // <<
	TOKEN_DOLLAR,       // $
	TOKEN_EOF
}			t_token_type;

typedef struct s_token
{
	t_token_type type;    // The type of token
	char *value;          // The actual token string
	struct s_token *next; // Linked list to store multiple tokens
}			t_token;

#endif
