#include "minishell.h"

void    ft_putstr(char *s, int fd)
{
    write(fd, s, ft_strlen(s));
}

void    ft_lstclear(t_gar **lst)
{
        t_gar  *d;
        t_gar  *s;

        if (lst == NULL || *lst == NULL)
                return ;
        d = *lst;
        while (d)
        {
                s = d->next;
                free(d->addr);
                free(d);
                d = s;
        }
        *lst = NULL;
}

const char	*costruct_prompt(void)
{
	char	*cwd;
	char	*home;
	char	*prompt;
	char	*tmp;

	cwd = ft_malloc(1, 100);
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
		prompt = ft_strjoin(cwd, "$ ");
	return (prompt);
}


// const char	*costruct_prompt(void)
// {
// 	char	*cwd;
// 	char	*home;
// 	char	*prompt;
// 	char	*tmp;

// 	// just take the current working dir and $
// 	cwd = malloc(100);
// 	if (!cwd)
// 	{
// 		perror("ft_malloc");
// 		exit(EXIT_FAILURE);
// 	}
// 	getcwd(cwd, 100);
// 	home = getenv("HOME");
// 	if (home && !strncmp(cwd, home, strlen(home)))
// 	{
// 		tmp = ft_strjoin("~", cwd + strlen(home));
// 		prompt = ft_strjoin(tmp, "$ ");
// 		free(tmp);
// 	}
// 	else
// 		prompt = ft_strdup(cwd);
// 	return (prompt);
// }

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
void print_token(t_token *head)
{
    while (head)
    {
        printf("%s %u\n", head->value, head->type);
        head = head->next;
    }
}


void	print_lexer(char **s)
{
	int i = 0;
	while (s && s[i])
	{
		printf("token[%d]: [%s]\n", i, s[i]);
		i++;
	}
}
int	main(void)
{
	char		*cmd_line;
	const char	*prompt;
	t_token		**h;

	cmd_line = NULL;
	cmd_line = ft_malloc(1, 100);
	prompt = costruct_prompt();
	while (1)
	{
		cmd_line = readline(prompt);
		if (!cmd_line)
			break ;
		h = create_tokens(lexer(cmd_line));
		 if (h)
		 	print_token(*h);
		// char *line;
    	// printf("Testing standard input...\n");
    	// line = get_next_line(0); // This should print a prompt and wait for input
    	// if (line) {
        // 	printf("You typed: %s\n", line);
        // 	free(line);
    	// }
		//print_lexer(lexer(cmd_line));
		rl_on_new_line();
	}
	// free_trash();
	ft_lstclear(garbage_list());
	rl_clear_history();
	return (0);
}
