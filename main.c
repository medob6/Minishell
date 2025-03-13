#include "minishell.h"

void    ft_putstr(char *s, int fd)
{
    write(fd, s, ft_strlen(s));
}

const char	*costruct_prompt(void)
{
	char	*cwd;
	char	*home;
	char	*prompt;
	char	*tmp;

	// just take the current working dir and $
	cwd = malloc(100);
	if (!cwd)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	getcwd(cwd, 100);
	home = getenv("HOME");
	if (home && !strncmp(cwd, home, strlen(home)))
	{
		tmp = ft_strjoin("~", cwd + strlen(home));
		prompt = ft_strjoin(tmp, "$ ");
		free(tmp);
	}
	else
		prompt = ft_strdup(cwd);
	free(cwd);
	return (prompt);
}

// void	free_trash(void)
// {
// 	HIST_ENTRY	**history_list;
// 	int			i;

// 	history_list = history_list();
// 	if (history_list)
// 	{
// 		i = 0;
// 		while (history_list[i])
// 		{
// 			free(history_list[i]->line);
// 			free(history_list[i]);
// 			i++;
// 		}
// 	}
// 	rl_clear_history();
// }
void	print_token(t_token **q)
{
	int	i;

	i = 0;
	while (q && q[i])
	{
		printf("%s %u\n", q[i]->value, q[i]->type);
		i++;
		q = &q[i]->next;
	}
}

void	print_lexer(char **s)
{
	int i = 0;
	while (s && s[i])
	{
		printf("token[%d]: [%s]\n", i, s[i]);
		free(s[i]);
		i++;
	}
	free(s);
}
int	main(void)
{
	char		*cmd_line;
	const char	*prompt;

	cmd_line = NULL;
	cmd_line = malloc(100);
	prompt = costruct_prompt();
	while (1)
	{
		cmd_line = readline(prompt);
		if (!cmd_line)
			break ;
		// q = tokenize(cmd_line);
		print_token(create_tokens(lexer(cmd_line)));
			//printf("line from prompt line is : %s\n", cmd_line);
		rl_on_new_line();
		free(cmd_line);
	}
	// free_trash();
	rl_clear_history();
	return (0);
}
