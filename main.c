#include "minishell.h"

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
                free(d->add);
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
	}
	else
		prompt = ft_strjoin(cwd, "$ ");
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

char *get_value(int type)
{
	if (type == TOKEN_WORD)
		return ("TOKEN_WORD");
	else if (type == TOKEN_WILDCARDS) // *
		return ("TOKEN_WILDCARDS");
	else if (type == TOKEN_PARENTESIS_OPEN) // (
		return ("TOKEN_PARENTESIS_OPEN");
	else if (type == TOKEN_PARENTESIS_CLOSE) // )
		return ("TOKEN_PARENTESIS_CLOSE");
	else if (type == TOKEN_AND)
		return ("TOKEN_AND");
	else if (type == TOKEN_OR)
		return ("TOKEN_OR");
	else if (type == TOKEN_PIPE) // |
		return ("TOKEN_PIPE");
	else if (type == TOKEN_REDIRECT_IN) // <
		return ("TOKEN_REDIRECT_IN");
	else if (type == TOKEN_REDIRECT_OUT) // >
		return ("TOKEN_REDIRECT_OUT");
	else if (type == TOKEN_APPEND) // >>
		return ("TOKEN_APPEND");
	else if (type == TOKEN_HEREDOC) // <<
		return ("TOKEN_HEREDOC");
	else if (type == TOKEN_TO_EXPAND)
		return ("TOKEN_TO_EXPAND");
	else if (type == TOKEN_EOF)
		return ("TOKEN_EOF");
	else
		return ("UNKNOWN_TOKEN");
}

void print_token(t_token *head)
{
    while (head)
    {
        printf("%s %s\n", head->value, get_value(head->type));
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
	//t_token		**h;

	cmd_line = NULL;
	cmd_line = malloc(100);
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
	rl_clear_history();
	return (0);
}
