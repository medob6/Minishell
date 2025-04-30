#include "execution.h"

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
bool	cmd_built_in(char *cmd)
{
	if (!ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "echo") || !ft_strcmp(cmd,"cd") || !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit"))
		return (true);
	return (false);
}
bool	parse_cmd(t_cmd *cmd, t_ast_node *node, t_env *env)
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
		cmd->path = get_cmd_path(cmd_args[0], extract_envp(env));
	if (cmd_built_in(cmd_args[0]))
		cmd->is_built_in = 1;
	return (true);
}

t_cmd	*parse_cmd_list(int cmd_nbr, t_ast_node **cmd_node, t_env *envp)
{
	t_cmd	*cmd_lst;
	int		i;

	cmd_lst = ft_calloc(cmd_nbr, sizeof(t_cmd));
	i = 0;
	while (i < cmd_nbr)
	{
		if (cmd_node[i]->type == AST_SIMPLE_CMD)
		{
			//printf("cmd_index = %d ;lenght = %zu \n",i,cmd_node[i]->children->length);
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

int	open_file(t_token *file_token)
{
	if (file_token->type == TOKEN_REDIRECT_IN)
		return (open(file_token->value.str_value, O_RDONLY));
	else if (file_token->type == TOKEN_REDIRECT_OUT)
		return (open(file_token->value.str_value, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	else if (file_token->type == TOKEN_APPEND)
		return (open(file_token->value.str_value, O_WRONLY | O_CREAT | O_APPEND, 0644));
	else if (file_token->type == TOKEN_HEREDOC)
		return (file_token->value.fd_value);
	return (-1);
}

void	redirect(t_token *file_obj)
{
	int	fd;

	if (file_obj->value.fd_value == AMBIGUOUS_REDIRECTION)
	{
		// print_err("ambiguous redirect", file_obj->value.str_value);
		ft_putstr_fd("minishell: ambiguous redirect\n",2);
		// TODO  FIX heredoc value is not  a path
		exit(1);
	}
	fd = open_file(file_obj);
	if (fd != -1)
	{
		if (file_obj->type == TOKEN_APPEND
			|| file_obj->type == TOKEN_REDIRECT_OUT)
			dup2(fd, STDOUT_FILENO);
		else
			dup2(fd, STDIN_FILENO);
	}
	else
	{
		print_err(strerror(errno), file_obj->value.str_value);
		// TODO  FIX heredoc value is not  a path
		// exit_status(data,1);
	}
	// printf("fd = %d\n",fd);
	close(fd);
}

void	perforem_redirections(t_data *data, int n)
{
	size_t	i;
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
		// data->old_fd = redir_lst[i]->value.fd_value;
		redirect(redir_lst[i]);
		i++;
	}
	// data->fd[1] = -5;
	if (n != data->cmd_nbr - 1)
	{
		dup2(data->fd[1], STDOUT_FILENO);
		close(data->fd[1]);
	}
}

void	parent(int *old_fd, int *fd)
{
	// printf("%d\n",fd[1]);
	close(fd[1]);
	if (*old_fd >= 0)
		close(*old_fd);
	*old_fd = fd[0];
}

// TODO FIX built-ins runing

void	execute_built_in(t_cmd cmd, t_data *data)
{
	(void)data;
	if (!ft_strcmp(cmd.args[0], "export"))
		ft_export(cmd.args,&data->env);
	else if (!ft_strcmp(cmd.args[0], "echo"))
		ft_echo(cmd.args, data->out_fd);
	else if (!ft_strcmp(cmd.args[0], "cd"))
		ft_cd(cmd.args[1],&data->env);
	else if (!ft_strcmp(cmd.args[0], "pwd"))
		ft_pwd(&data->env);
	else if (!ft_strcmp(cmd.args[0], "unset"))
		ft_unset(cmd.args,&data->env);
	else if (!ft_strcmp(cmd.args[0], "env"))
		ft_env(&data->env);
	else if (!ft_strcmp(cmd.args[0], "exit"))
		ft_exit(cmd.args,cmd.exit_status); //  i dont know if it is last status or not
	return;
}

// void	redirection_builtins(t_data *data, int n)
// {
// 	size_t		i;
// 	t_token	**redir_lst;

// 	i = 0;
// 	redir_lst = data->lst_cmd[n].redirlist;

// 	// here where i conect the pipes
// 	data->old_fd = data->fd[0];
// 	/// up is very important
// 	if (data->old_fd != -1)
// 	{
// 		// dup2(data->old_fd, STDIN_FILENO); // i dont use input inbuilt-ins
// 		close(data->old_fd);
// 	}

// 	while (i < data->lst_cmd[n].redir_ars_nbr)
// 	{
// 		if (data->out_fd != 1)
// 			close(data->out_fd);
// 		// TODO I am here !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// 		if (redir_lst[i]->type == TOKEN_APPEND || redir_lst[i]->type == TOKEN_REDIRECT_OUT)
// 			data->out_fd = open_file(redir_lst[i]);
// 		i++;
// 	}
// 	if (n != data->cmd_nbr - 1)
// 	{
// 		if (data->out_fd != -1)
// 			data->out_fd = data->fd[1]; // here if there was no
// 		else
// 			close(data->fd[1]);
			
// 	}
// 	// parent(&data->old_fd, data->fd);
// }

void	redirection_builtins(t_data *data, int n)
{
	size_t	i;
	t_token	**redir_lst;

	i = 0;
	redir_lst = data->lst_cmd[n].redirlist;
	if (n != data->cmd_nbr - 1)
		close(data->fd[0]);
	// else if (data->lst_cmd[n].is_built_in && data->cmd_nbr == 1)
	// 	data->old_fd = data->fd[0];
	if (data->old_fd != -1)
	{
		dup2(data->old_fd, STDIN_FILENO);
		close(data->old_fd);
	}
	while (i < data->lst_cmd[n].redir_ars_nbr)
	{
		if (data->out_fd != 1)
			close(data->out_fd);
		if (redir_lst[i]->value.fd_value == AMBIGUOUS_REDIRECTION)
		{
			ft_putstr_fd("minishell: ambiguous redirect\n",2);
			data->out_fd = -1;
			return;
		}
		if (redir_lst[i]->type == TOKEN_APPEND
			|| redir_lst[i]->type == TOKEN_REDIRECT_OUT)
			data->out_fd = open_file(redir_lst[i]);
		else
			close(open_file(redir_lst[i]));
		i++;
	}
	if (n != data->cmd_nbr - 1)
	{
		if (data->out_fd != -1)
			data->out_fd = data->fd[1];
		else
			close(data->fd[1]);
	}
}
void	child(t_data *prg_data, int index)
{
	
	if (!prg_data->lst_cmd[index].is_built_in)
	{
		// check imbeguis redir // TODO recheck 
		perforem_redirections(prg_data, index);
		execute_cmd(prg_data->lst_cmd[index], prg_data);
	}
	else
	{
		// check imbeguis redir // TODO recheck 
		redirection_builtins(prg_data, index);
		execute_built_in(prg_data->lst_cmd[index], prg_data);
	}
	if (!prg_data->lst_cmd[index].is_built_in || (prg_data->cmd_nbr > 1))
		exit_status(prg_data, 1);
}

void close_here_docs(t_token **redir_list)
{
	int i;
	i = 0;
	while (redir_list && redir_list[i])
	{
		if (redir_list[i]->type == TOKEN_HEREDOC)
			close(redir_list[i]->value.fd_value);
		i++;
	}
	return;
}

void	pipe_execution(t_data *prg_data)
{
	int	i;

	i = 0;
	while (i < prg_data->cmd_nbr)
	{
		if (i != prg_data->cmd_nbr - 1)
			pipe(prg_data->fd);
		
		if (!prg_data->lst_cmd[i].is_built_in || (prg_data->cmd_nbr > 1))
			prg_data->lst_cmd[i].pid = fork();
		if (prg_data->lst_cmd[i].pid == 0)
			child(prg_data, i);
		else
		{
			parent(&prg_data->old_fd, prg_data->fd);
			close_here_docs(prg_data->lst_cmd[i].redirlist);
		}
		i++;
	}
}

static void	init_program_data(t_data *data, t_ast_node *pipeline, t_env *env)
{
	data->old_fd = -1;
	data->out_fd = 1;
	data->fd[0] = -1;
	data->fd[1] = -1;
	data->env = env;
	expand_pipeline(pipeline, &data->env);// expand here// TODO
	data->cmd_nbr = pipeline->children->length;
	printf("cmd_nbr = %d\n",data->cmd_nbr);
	data->lst_cmd = parse_cmd_list(data->cmd_nbr, (t_ast_node **)pipeline->children->items, env);
	data->env = env;
}
int	execute_pipeline(t_ast_node *pipeline, t_env *env)
{
	t_data	prg_data;
	int		status;

	// expansion must happen here
	// export  will happen only if export is the only cmd in the pipeline  
	init_program_data(&prg_data, pipeline, env);
	pipe_execution(&prg_data);
	wait_for_prc(prg_data.lst_cmd, prg_data.cmd_nbr);
	status = prg_data.lst_cmd[prg_data.cmd_nbr - 1].exit_status;
	// free_garbeg(&prg_data); //TODO when freeing farbeg i do lost some data , like envp
	return (status);
}


int	execute_cmd_line(t_ast_node *root, t_env *env)
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
		status = execute_pipeline(cmd, env);
		if (++i < root->children->length)
		{
			op = (t_ast_node *)root->children->items[i];
			if ((status == 0 && op->type == AST_AND) || (status != 0
					&& op->type == AST_OR))
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
// // ? testing function

// static void	print_lst(t_env **env)
// {
// 	t_env	*head;

// 	head = *env;
// 	while (head)
// 	{
// 		if (!strcmp(head->key, "var"))
// 		{
// 			printf("\033[0;32mvalue added succesfully \033[0m\n");
// 			printf("key2 = %s , value = %s \n", head->key, head->value);
// 		}
// 		else
// 			printf("key2 = %s , value = %s \n", head->key, head->value);
// 		head = head->next;
// 	}
// 	if (head)
// 		printf("\033[0;32mkey was not added\033[0m\n");
// 	return ;
// }
int	execution(t_ast_node *root, t_env *env)
{
	int	n;
	// expand_ast(root); // maybe if i could expand only what could be expanded
	// print_lst(&env);
	n = execute_cmd_line(root, env);
	return (n);
}

// TODO

// i should pull new tokenizer code : //!DONE
// what i should do now :
// 1- implement builting in execution !!!!!!!!! // TODO   ===> "understand what i do"  
//! DONE
//TODO check if file descriptors are closed and only needed ones opened
// 2- upgrade heredoc code 			!!!!!!!!! have some errors and linked to 7 //! DONE
// 3- extract envp before rederection //! DONE
// 4- check for imbiguse rederictions //? in progeress
// 5- check for save derefrencing
// 6- implement subshell				!!!!!!!!!! tomorow //TODO today
// 7- handel exit status code we have five (also in built-in);
// 8- test execuiton
// 9- remove paranteses in parsing
// 10- test everything //TODO when expansion is finished

// PROBLEMS
// we got a lot of problems that should be fixed

// if i have : export f="f" && echo $f
// if i exported before parsing it will change f to ""
// so i need to expand before runing the command
// first extract them enpv and let them in there linked list  then before runing export.... ,extract them to envp char ** ; then expand :::: this is a prblm soo
// TODO i will expand for each node of simple_cmd alone ......................... tomorow cus it will take a lot of time
// second export will add to the list , and we will do the same again

// TODO FOR solaiman
//! FIX built-ins  :
// ignor comments as argments #somthin
// -nnnn for echo should be ignored even after first flag
// += append for export
// should pass NULL to cat in case of empthy expansion not '' or ""
// if runing export without args or with a comment it must show the env vars
// for cd it must if given more the one arg show err "to many args" and do nothing // test more , and change terminal path
// pwd doesnt work at all it must print out the current working dir path , and it must take a file descriptor in accoredens with the execution
// unset seems fine but need further testing and err handeling
// env is fine , should be tested with unset , and export
// last status need to be fixed in exit , testing with args
//! DO in echo handell case if write fails perror("minishell: ");

// in expansion if an expansion resulted a empthy string and nothing was linked to the arg expanded , it must be removed rather then stored as a empthy str , ask mahedi
// if i expand a file name in redirections and
