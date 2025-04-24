#include "execution.h"

// change linked list to  arr

int	lst_size(t_env *head)
{
	int	i;

	i = 0;
	while (head)
	{
		i++;
		head = head->next;
	}
	return (i);
}

char	**extract_envp(t_env *env)
{
	t_env	*head;
	char	*tmp;
	char	**envp;
	int		i;

	head = env;
	envp = ft_malloc(sizeof(char **), lst_size(env) + 1);
	i = 0;
	while (head)
	{
		tmp = ft_strjoin(head->key, "=");
		envp[i++] = ft_strjoin(tmp, head->value);
		head = head->next;
	}
	envp[i] = NULL;
	return (envp);
}
bool cmd_built_in(char *cmd)
{
	if (!ft_strcmp(cmd,"export"))
		return (true);
	else if (!ft_strcmp(cmd,"echo"))
		return (true);
	else if (!ft_strcmp(cmd,"cd"))
		return (true);
	else if (!ft_strcmp(cmd,"pwd"))
		return (true);
	else if (!ft_strcmp(cmd,"unset"))
		return (true);
	else if (!ft_strcmp(cmd,"env"))
		return (true);
	else if (!ft_strcmp(cmd,"exit"))
		return (true);

	return (false);
}
bool	parse_cmd(t_cmd *cmd, t_ast_node *node, char **envp)
{
	char	**cmd_args;

	cmd_args = NULL;
	cmd->is_built_in = 0;
	cmd->redir_ars_nbr = 0;
	cmd->redirlist = NULL;
	if ((t_token **)node->children)
		cmd_args = (char **)node->children->items;
	if ((t_token **)node->redirect_list)
	{
		cmd->redirlist = (t_token **)node->redirect_list->items;
		cmd->redir_ars_nbr = node->redirect_list->length;
	}
	if (!cmd_args || cmd_args[0] == NULL)
	{
		if (cmd_args && !cmd_args[0])
			ft_free(cmd_args);
		cmd->args = NULL;
		cmd->path = NULL;
		return (false);
	}
	cmd->args = cmd_args;
	if (cmd_is_path(cmd_args[0]))
		cmd->path = ft_strdup(cmd_args[0]);
	else
		cmd->path = get_cmd_path(cmd_args[0], envp);
	if (cmd_built_in(cmd_args[0]))
		cmd->is_built_in = 1;
	return (true);
}

t_cmd	*parse_cmd_list(int cmd_nbr, t_ast_node **cmd_node, char **envp)
{
	t_cmd	*cmd_lst;
	int		i;

	cmd_lst = ft_calloc(cmd_nbr, sizeof(t_cmd));
	i = 0;
	while (i < cmd_nbr)
	{
		if (cmd_node[i]->type == AST_SIMPLE_CMD)
		{
			parse_cmd(&cmd_lst[i], cmd_node[i], envp);
		}
		// else if (cmd_node[i]->type = AST_SUBSHELL)
		// {
		// TODO: here i should fork for the subshell  ... I should
		// 	execute_cmd_line(cmd_node, envp);
		// TODO: perform redirections after subshell
		// }
		i++;
	}
	return (cmd_lst);
}

// this is redir handeling
int	open_file(t_token *file_token)
{
	if (file_token->type == TOKEN_REDIRECT_IN)
		return (open(file_token->value, O_RDONLY));
	else if (file_token->type == TOKEN_REDIRECT_OUT)
		return (open(file_token->value, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	else if (file_token->type == TOKEN_APPEND)
		return (open(file_token->value, O_WRONLY | O_CREAT | O_APPEND, 0644));
	else if (file_token->type == TOKEN_HEREDOC)
		return (atoi(file_token->value));
	return (-1);
}


// the problem is when a built in redirect to a file it dup it to stdout so any write that happens to stdout becomes written in that file that cuse a problem ,  i should write directly to th target file
void	redirect(t_token *file_obj)
{
	int	fd;


	fd = open_file(file_obj);
	if (fd != -1)
	{
		if (file_obj->type == TOKEN_APPEND || file_obj->type == TOKEN_REDIRECT_OUT)
			dup2(fd, STDOUT_FILENO);
		else
			dup2(fd, STDIN_FILENO);
	}
	else
	{
		print_err(strerror(errno), file_obj->value);
			//TODO  FIX heredoc value is not  a path
		// exit_status(data,1);
	}
	close(fd);
}

// in perform redirctions i should change outfd if the command is built-in to  
void	perforem_redirections(t_data *data, int n)
{
	size_t		i;
	t_token	**redir_lst;

	i = 0;
	redir_lst = data->lst_cmd[n].redirlist;
	if (n != data->cmd_nbr - 1)
		close(data->fd[0]);
	if (data->old_fd != -1)
	{
		dup2(data->old_fd, STDIN_FILENO);
		close(data->old_fd);
	}

	while (i < data->lst_cmd[n].redir_ars_nbr)
	{
		redirect(redir_lst[i]);
		i++;
	}
	if (n != data->cmd_nbr - 1)
	{
		dup2(data->fd[1], STDOUT_FILENO);
		close(data->fd[1]);
	}
}

void	parent(int *old_fd, int *fd)
{
	close(fd[1]);
	if (*old_fd >= 0)
		close(*old_fd);
	*old_fd = fd[0];
}

//TODO FIX built-ins runing

void execute_built_in(t_cmd cmd,t_data *data)
{
	(void)data;
	if (!ft_strcmp(cmd.args[0], "export"))
		return ;
	else if (!ft_strcmp(cmd.args[0],"echo"))
	{
		// printf("outfd = %d\n",data->out_fd);
		ft_echo(cmd.args, data->out_fd); // i have implemented echo alone
		// write(data->fd[1],"hmikkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk\n",40);
	}
	else if (!ft_strcmp(cmd.args[0], "cd" ) )
		return ;
	else if (!ft_strcmp(cmd.args[0],"pwd" ) )
		return ;
	else if (!ft_strcmp(cmd.args[0],"unset") )
		return ;
	else if (!ft_strcmp(cmd.args[0],"env") )
		return ;
	else if (!ft_strcmp(cmd.args[0],"exit"))
		return ;
	return;
}


void	redirection_builtins(t_data *data, int n)
{
	size_t		i;
	t_token	**redir_lst;

	i = 0;
	redir_lst = data->lst_cmd[n].redirlist;

	// here where i conect the pipes
	data->old_fd = data->fd[0];
	/// up is very important
	if (data->old_fd != -1)
	{
		// dup2(data->old_fd, STDIN_FILENO); // i dont use input inbuilt-ins
		close(data->old_fd);
	}

	while (i < data->lst_cmd[n].redir_ars_nbr)
	{
		if (data->out_fd != 1)
			close(data->out_fd);
		// TODO I am here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (redir_lst[i]->type == TOKEN_APPEND || redir_lst[i]->type == TOKEN_REDIRECT_OUT)
			data->out_fd = open_file(redir_lst[i]);
		i++;
	}
	if (n != data->cmd_nbr - 1)
	{
		if (data->out_fd != -1)
			data->out_fd = data->fd[1]; // here if there was no
		else
			close(data->fd[1]);
			
	}
	// parent(&data->old_fd, data->fd);
}
void	child(t_data *prg_data, int index)
{
	//TODO extract envp .... i thinks
	// expand
	if (!prg_data->lst_cmd[index].is_built_in)
	{
		perforem_redirections(prg_data, index);
		execute_cmd(prg_data->lst_cmd[index], prg_data);
		exit_status(prg_data, 1);
	}
	else
	{
		redirection_builtins(prg_data, index);
		execute_built_in(prg_data->lst_cmd[index], prg_data);
	}
	
	
}

void	pipe_exec(t_data *prg_data)
{
	int	i;

	i = 0;
	while (i < prg_data->cmd_nbr)
	{
		printf("i = %d\n",i);
		if (i != prg_data->cmd_nbr - 1)
			pipe(prg_data->fd);
		if (!prg_data->lst_cmd[i].is_built_in)
			prg_data->lst_cmd[i].pid = fork();
		// else
		// {
		// 	// i should run built in here 
		// 	printf("pid = %d ,this is a built-in\n",prg_data->lst_cmd[i].pid);
		// }
		if (prg_data->lst_cmd[i].pid == 0)
			child(prg_data, i);
		else
			parent(&prg_data->old_fd, prg_data->fd);
		i++;
	}
}

int	execute_pipeline(t_ast_node *pipeline, char **envp)
{
	t_data	prg_data;
	int		status;

	// TODO :  make a function that initialize variables
	prg_data.old_fd = -1;
	prg_data.out_fd = 1;
	prg_data.fd[0] = -1;
	prg_data.fd[1] = -1;
	prg_data.cmd_nbr = pipeline->children->length;
	prg_data.lst_cmd = parse_cmd_list(prg_data.cmd_nbr,
			(t_ast_node **)pipeline->children->items, envp);
	// above parsing or creation of reidr list and heper things 
	prg_data.envp = envp;
	pipe_exec(&prg_data);
	wait_for_prc(prg_data.lst_cmd, prg_data.cmd_nbr);
	status = prg_data.lst_cmd[prg_data.cmd_nbr - 1].exit_status;
	free_garbeg(&prg_data);
	return (status);
}

int	execute_cmd_line(t_ast_node *root, char **envp)
{
	size_t		i;
	int			status;
	t_ast_node	*cmd;
	t_ast_node	*op;

	i = 0;
	status = 0;
	if (!root || !root->children || root->children->length == 0)
		return (0);
	while (i < root->children->length)
	{
		cmd = (t_ast_node *)root->children->items[i];
		status = execute_pipeline(cmd, envp);
		if (++i < root->children->length)
		{
			op = (t_ast_node *)root->children->items[i];
			if ((status == 0 && op->type == AST_AND) || (status != 0 && op->type == AST_OR))
			{
				++i;
				continue ;
			}
			else
				return (status);
		}
		else
			break ;
	}
	return (status);
}

int	execution(t_ast_node *root, t_env *env)
{
	int		n;
	char	**envp;

	envp = extract_envp(env);
	// expand_ast(root); // i am still waiting for my partener to make this one
	n = execute_cmd_line(root, envp);
	return (n);
}


// TODO
// what i should do now :
//1- implement builting in execution !!!!!!!!! //TODO   ===> "understand what i do"
//2- upgrade heredoc code			 !!!!!!!!! have some errors and linked to 7
//3- extract envp before rederection
//4- check for imbiguse rederictions
//5- check for save derefrencing
//6- implement subshell				!!!!!!!!!! tomorow
//7- handel exit status code we have five (also in built-in);
//8- test execuiton
//9- remove paranteses in parsing
//10- test everything //TODO when expansion is finished


//PROBLEMS
// we got a lot of problems that should be fixed