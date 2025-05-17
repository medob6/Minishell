/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 22:28:26 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/17 12:11:30 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution/execution.h"

t_herdoc	herdoc;

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
	print_str_fd("\n", 2);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	initialize_shell(t_env **env, char **envp, int *shlvl)
{
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, handler);
	(*shlvl)++;
	*env = create_the_main_list(envp, *shlvl);
	rl_outstream = stderr;
}

void	process_command(char *cmd_line, t_env *env)
{
	t_token		**h;
	t_ast_node	*ast;

	ast = NULL;
	if (*cmd_line)
		add_history(cmd_line);
	h = create_tokens(lexer(cmd_line));
	if (h)
		ast = parse_tokens(*h);
	if (!ast)
	{
		print_str_fd("minishell: syntax error near unexpected token\n", 2);
		*get_last_status() = 2;
	}
	else
		execution(ast, env);
}

//void	init_heredoc(t_herdoc *herdoc)
//{
//	herdoc->exit_sign = 0;
//	herdoc->fd_heredoc = 0;
//	herdoc->nbr_heredoc = 0;
//	herdoc->old_fd = 0;
//	herdoc->no_file_name = 0;
//}

int	main(int ac, char **av, char **envp)
{
	char		*cmd_line;
	const char	*prompt;
	static int	shlvl;
	t_env		*env;

	(void)ac;
	(void)av;
	initialize_shell(&env, envp, &shlvl);
	while (1)
	{
		//init_heredoc(&herdoc);
		ft_bzero(&herdoc, sizeof(t_herdoc));
		prompt = costruct_prompt(env);
		cmd_line = readline(prompt);
		if (!cmd_line)
			break ;
		process_command(cmd_line, env);
		free(cmd_line);
	}
	ft_putendl_fd("exit", 1);
	ft_lstclear(garbage_list());
	rl_clear_history();
	return (0);
}
