#include "execution.h"

// TODO : tomorow i will test execution using the new file i have added in the makefile called execute_input and add functions from pipex

// add this :  execute command , exit_status
#include <sys/types.h>
#include <sys/wait.h>

void	free_pointers(char **p)
{
	int	i;

	i = 0;
	if (!p)
		return ;
	while (p[i])
	{
		ft_free(p[i]);
		i++;
	}
	ft_free(p);
}
void	ft_free(void *address)
{
	t_gar	**garbage_lst;
	t_gar	*prev;
	t_gar	*cur;

	garbage_lst = garbage_list();
	prev = NULL;
	cur = *garbage_lst;
	while (cur)
	{
		if (cur->addr == address)
		{
			if (prev)
				prev->next = cur->next;
			else
				*garbage_lst = cur->next;
			free(cur->addr);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
	// Optionally, print a warning if not found // oe do something
	return ;
}
void	free_garbeg(t_data *prg_data)
{
	int	i;

	i = 0;
	while (i < prg_data->cmd_nbr)
	{
		ft_free(prg_data->lst_cmd[i].path);
		free_pointers(prg_data->lst_cmd[i].args);
		i++;
	}
	ft_free(prg_data->lst_cmd);
	// get_next_line(-1);
}
void	exit_status(t_data *prg_data, int status)
{
	free_garbeg(prg_data);
	exit(status);
}

char	*get_path(char **paths)
{
	int	i;

	i = 0;
	while (paths && paths[i])
	{
		if (!ft_strncmp("PATH=", paths[i], 5))
			return (paths[i]);
		i++;
	}
	return (NULL);
}

char	*build_cmd_path(char *cmd, char *path)
{
	char	*cmd_path;
	char	*tmp;

	cmd_path = NULL;
	tmp = ft_strjoin(path, "/");
	cmd_path = ft_strjoin(tmp, cmd);
	ft_free(tmp);
	return (cmd_path);
}

int	is_dir(char *path)
{
	char	*new_path;
	int		ans;

	ans = 0;
	new_path = ft_strjoin(path, "/");
	if (access(new_path, F_OK) == 0)
		ans = 1;
	ft_free(new_path);
	return (ans);
}

char	*get_cmd_path(char *cmd, char **envp)
{
	char	*paths;
	char	**args;
	char	*cmd_path;
	int		i;

	i = 0;
	cmd_path = NULL;
	paths = get_path(envp);
	if (paths == NULL)
		return (NULL);
	args = ft_split(&paths[5], ':');
	while (args[i])
	{
		cmd_path = build_cmd_path(cmd, args[i]);
		if (access(cmd_path, F_OK) == 0 && !is_dir(cmd_path))
			break ;
		ft_free(cmd_path);
		cmd_path = NULL;
		i++;
	}
	free_pointers(args);
	return (cmd_path);
}

bool	cmd_is_path(char *s)
{
	if (ft_strchr(s, '/'))
		return (true);
	return (false);
}
void	wait_for_prc(t_cmd *cmd_list, int cmd_nbr)
{
	int	i;
	int	status;

	i = 0;
	if (cmd_nbr == 1 && cmd_list[0].is_built_in)
		return ;
	while (i < cmd_nbr)
	{
			waitpid(cmd_list[i].pid, &status, 0);
			cmd_list[i].exit_status = 0;
			if (WIFEXITED(status))
				cmd_list[i].exit_status = WEXITSTATUS(status);
			else if (WIFSTOPPED(status))
				cmd_list[i].exit_status = WSTOPSIG(status) + 128;
			else if (WIFSIGNALED(status))
				cmd_list[i].exit_status = WTERMSIG(status) + 128;
		i++;
	}
}

void	execute_cmd(t_cmd cmd, t_data *prg_data)
{
	char *new_path;
	int status;
	char **envp;

	status = 126;
	envp = extract_envp(prg_data->env);
	// HERE : EXPAND FOR THIS SINGAL CMD
	if (!cmd.path)
	{
		if (!cmd.args)
			ft_putstr_fd("minishell: Command not found: \n", 2);
		else
			print_err("command not found", cmd.args[0]);
		exit_status(prg_data, 127);
	}
	new_path = ft_strjoin(cmd.path, "/");
	if (access(cmd.path, F_OK) != 0)
	{
		print_err(strerror(errno), cmd.path);
		status = 127;
	}
	else if (access(new_path, F_OK) != 0)
	{
		execve(cmd.path, cmd.args, envp);
		print_err(strerror(errno), cmd.path);
	}
	else
		print_err(strerror(21), cmd.path);

	ft_free(new_path);
	exit_status(prg_data, status);
}
