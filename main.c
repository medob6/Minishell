#include "minishell.h"

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
	{
		prompt = ft_strdup(cwd);
	}
	free(cwd);
	return (prompt);
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
		if (!cmd_line) // Check if user pressed Ctrl+D (EOF)
			break ;
		rl_on_new_line();
		printf("line from prompt line is : %s\n", cmd_line);
		// here instead of printing input we should run it
		// tokenize(cmd_line);
		free(cmd_line); // Free allocated memory
	}
	// free_trash();
	rl_clear_history();
	return (0);
}
