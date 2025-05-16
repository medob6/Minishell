
#include "execution/execution.h"

char	*costruct_prompt(t_env *env)
{
	char		*cwd;
	char		*home;
	static char	*prompt;
	char		*tmp;

	prompt = NULL;
	tmp = NULL;
	cwd = getcwd(NULL, 0);
	if (env && !cwd)
		cwd = expand_the_value("$PWD", &env);
	home = getenv("HOME");
	if (home && !ft_strncmp(cwd, home, ft_strlen(home)))
	{
		tmp = ft_strjoin("~", cwd + ft_strlen(home));
		prompt = ft_strjoin(tmp, "$ ");
	}
	else
		prompt = ft_strjoin(cwd, "$ ");
	ft_free(tmp);
	ft_free(cwd);
	return (prompt);
}


void	handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	main(int ac, char **av, char **envp)
{
	char		*cmd_line;
	const char	*prompt;
	t_token		**h;
	t_ast_node	*ast;
	static int	shlvl;
	t_env		*env;

	(void)ac;
	(void)av;
	ast = NULL;
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handler);
	shlvl++;
	env = create_the_main_list(envp, shlvl);
	rl_outstream = stderr;
	while (1)
	{
		ast = NULL;
		prompt = costruct_prompt(env);
		cmd_line = readline(prompt);
		if (!cmd_line)
			break ;
		if (*cmd_line)
			add_history(cmd_line);
		h = create_tokens(lexer(cmd_line));
		if (h)
			ast = parse_tokens(*h);
		if (!ast)
		{
			print_str_fd("minishell: syntax error near unexpected token\n" ,2);
			*get_last_status() = 2;
		}
		else
			execution(ast, env);
		free(cmd_line);
		rl_on_new_line();
	}
	ft_putendl_fd("exit",1);
	ft_lstclear(garbage_list());
	rl_clear_history();
	return (0);
}
