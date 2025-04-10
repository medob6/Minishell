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

/* Structures */
// typedef struct s_cmd
// {
// 	char	*path;
// 	char	**args;
// 	pid_t	pid;
// 	int		exit_status;
// }			t_cmd;

// typedef struct s_data
// {
// 	int		old_fd;
// 	int		out_fd;
// 	int		fd[2];
// 	t_cmd	*lst_cmd;
// 	int		cmd_nbr;
// 	char	**envp;
// 	int		ac;
// }			t_data;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_WILDCARDS, // *
	TOKEN_PARENTESIS_OPEN, //(
	TOKEN_PARENTESIS_CLOSE,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_PIPE,         // |
	TOKEN_REDIRECT_IN,  // <
	TOKEN_REDIRECT_OUT, // >
	TOKEN_APPEND,       // >>
	TOKEN_HEREDOC,      // <<
	TOKEN_DOLLAR,
	TOKEN_EOF
}			t_token_type;

typedef struct s_token
{
	char *value;          // The actual token string
	t_token_type type;
	struct s_token *prev;  // The type of token
	struct s_token *next; // Linked list to store multiple tokens
}			t_token;

typedef struct s_garbag
{
    void    *add;
    struct s_garbag *next;
}   t_gar;

void	ft_putstr(char *s, int fd);
t_token		**create_tokens(char **str);
char **lexer(char *cmd_line);
void    *ft_malloc(int size, int bytes);
t_gar   **garbage_list(void);
#endif
