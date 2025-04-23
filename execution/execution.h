#ifndef TOKEN_H
# define TOKEN_H

# include "../parser.h"

int			status = 0;
int			execute_cmd_line(t_ast_node *root, char **envp);

/* Structures */

// TODO: from stuct cmd remove ext_status , add redir list
typedef struct s_cmd
{
	char	*path;
	char	**args;
	pid_t	pid;
	int		exit_status;
	t_token	**redirlist;
}			t_cmd;

// TODO: from data struct remove uneccesery data like out_fd
/// should preserv old_fd
typedef struct s_data
{
	int		old_fd;
	int out_fd; // to remove
	int		fd[2];
	t_cmd	*lst_cmd;
	int		cmd_nbr;
	char	**envp;
}			t_data;

// here must be expansion func or header file

/* FUNCTIONS */
int			execution(t_ast_node *root, t_env *env);
char		*get_value(int type);
t_token		**create_tokens(char **str);
void		perforem_redirections(t_data *data, int n);

#endif