#include "execution.h"

// here i am gona code a set of functions that execute commands

//	TODO IN EXPANSION //

// TODO: expaned before excute ; i exepect that the tree nodes are exepanded before


// TODO IN EXEC //

// TODO: in exection should check if (redirectin list == 1) if not imbigios redirection or any err in redirction
// TODO: in case of a singel simple command in subshell it runs in the parent shell not in a child but built-ins behave diferntlly
// TODO: in execution i should first travers on the tree from top to bottom first
// so it will be something like this
// root == parent
// and_or (childerens of root with are pipe and+or ..), each one of them is a child that will retun an exit status , 
//        i will take that exit status and check the next nod is || && to determin if i creat the next child
// pipeline = (child of and_or); each pipe line consist of simpel command , each command is a child of pipeline
// simpl_cmd = here we should consume and execut cmd and check of errors like mentioned above


//PART 2: make execution of the pipeline


// void	out_file_open(t_data *prg_data, int in_her_doc, char *path, int *out_fd)
// {
// 	if (!in_her_doc)
// 		*out_fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	else
// 		*out_fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 	if (*out_fd == -1)
// 	{
// 		print_err(strerror(errno), path);
// 		ft_exit(prg_data, 1);
// 	}
// }

void	child(t_data *prg_data, int index)
{
	// close(prg_data->fd[0]);
	// if (prg_data->old_fd != -1)
	// {
	// 	dup2(prg_data->old_fd, STDIN_FILENO);
	// 	close(prg_data->old_fd);
	// }
	// dup2(prg_data->fd[1], STDOUT_FILENO);
	// close(prg_data->fd[1]);
	perforem_redirections(prg_data,index);
	execute_cmd(prg_data->lst_cmd[index], prg_data);//TODO: handel execution of built-ins
	ft_exit(prg_data, 1);
}

// void	last_child(t_cmd cmd, t_data *prg_data, int rd_fd, int out_fd)
// {
// 	if (rd_fd != -1)
// 	{
// 		dup2(rd_fd, STDIN_FILENO);
// 		close(rd_fd);
// 	}
// 	dup2(out_fd, STDOUT_FILENO);
// 	close(out_fd);
// 	execute_cmd(cmd, prg_data);
// 	ft_exit(prg_data, 1);
// }

int open_file(t_token *file_token)
{
	if (file_token->type == TOKEN_REDIRECT_IN)
		return ( open(file_token->value, O_RDONLY));
	else if (file_token->type == TOKEN_REDIRECT_OUT)
		return (open(file_token->value, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	else if (file_token->type == TOKEN_APPEND)
		return (open(file_token->value, O_WRONLY | O_CREAT | O_APPEND, 0644));
	else if (file_token->type == TOKEN_HEREDOC)
		return (file_token->value);
}
void redirect(t_token *file_obj)
{
	int fd;
	fd = open_file(file_obj);
	if (fd != -1)
	{
		if (file_obj->type == TOKEN_APPEND || file_obj->type == TOKEN_REDIRECT_OUT)
			dup2(fd, STDOUT_FILENO);
		else
			dup2(fd,STDIN_FILENO);
	}
	close(fd);
}

void perforem_redirections(t_data *data,int n)
{
	int i;

	i = 0;
	int fd;
	t_token **redir_lst = data->lst_cmd[n].redirlist;
	if (i != data->cmd_nbr - 1) // in the last cmd we dont have to close the read end of th pipe cus 
		close(data->fd[0]);
	if (data->old_fd != -1)
	{
		dup2(data->old_fd, STDIN_FILENO);
		close(data->old_fd);
	}
	while (redir_lst[i])
	{
		redirect(redir_lst[i]);
		i++;
	}
	if (n != data->cmd_nbr - 1) // for the last cmd we dont have to put output to fd
	{
		dup2(data->out_fd, STDOUT_FILENO);
		close(data->out_fd);
	}
}

void	excute_child(t_data *prg_data,  int index) //char **av, int in_her_doc, removed some params
{

	//TODO: always perfom redirection first before executing 
	// if (index == 0)
	// {
	// 	if (in_her_doc)
	// 	{
	// 		if (prg_data->old_fd == -1)
	// 			ft_exit(prg_data, 1); //TODO: change ft_exit with other func that do the same job and add garbge list to data for freeing
	// 	}
	// 	else
	// 		in_file_open(prg_data, av[1], &(prg_data->old_fd));
	// }
	// if (index == prg_data->cmd_nbr - 1)
	// {

	// 	out_file_open(prg_data, in_her_doc, av[prg_data->ac - 1], &prg_data->out_fd);// prform redirctions all of them 
	// 	last_child(prg_data->lst_cmd[index], prg_data, prg_data->old_fd,
	// 		prg_data->out_fd);
	// }
	// else
	child(prg_data, index);
}


void	pipe_exec(t_data *prg_data)
{
	int	i;

	i = 0;
	while (i < prg_data->cmd_nbr)
	{
		if (i != prg_data->cmd_nbr - 1)
			pipe(prg_data->fd);
		prg_data->lst_cmd[i].pid = fork();
		if (prg_data->lst_cmd[i].pid == 0)
			excute_child(prg_data, i);// we pass av here for file names , but we have it in prg_data->cmd.redirlist ok 
		else
			parent(&prg_data->old_fd, prg_data->fd); // the oldfd/ outfd swaping  , with pipe fds
		i++;
	}
}


//PART 1: parssing to make a cmd_lst IN_PROGGRES .... ... ...
//TODO: edit parse_command function !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

bool	parse_cmd(t_cmd *cmd, t_ast_node *node, char **envp)
{
	char	**cmd_args;

	cmd_args = (char **)node->children->items;
	cmd->redirlist = (t_token **)node->redirect_list->items;
	if (!cmd_args || cmd_args[0] == NULL)
	{
		if (!cmd_args[0])
			free(cmd_args);
		cmd->args = NULL;
		cmd->path = NULL;
		return (false);
	}
	cmd->args = cmd_args;
	if (cmd_is_path(cmd_args[0]))
		cmd->path = ft_strdup(cmd_args[0]);
	else
		cmd->path = get_cmd_path(cmd_args[0], envp);
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
		else if (cmd_node[i]->type = AST_SUBSHELL)
		{
			//TODO: here i should fork for the subshell  ... I should
			execute_cmd_line(cmd_node, envp);
			//TODO: perform redirections in subshell 
		}
		i++;
	}
	return (cmd_lst);
}

int execute_pipeline(t_ast_node *pipeline, char **envp)
{
	t_data	prg_data;
	int		status;

	// prg_data.old_fd = -1;
	// first i should loop over the file descriptor list and find the in and herdoce and redir dup them to input
	// if (in_her_doc)
	// 	read_from_herdoc(av[2], &prg_data.old_fd);
	prg_data.cmd_nbr = pipeline->children->length; // in + out + pipex but in my case its the length of the pipeline childes
	prg_data.lst_cmd = parse_cmd_list(prg_data.cmd_nbr, pipeline->children->items,envp); //TODO : check if this right
	prg_data.envp = envp; // change it after expansion
	pipe_exec(&prg_data);
	wait_for_prc(prg_data.lst_cmd, prg_data.cmd_nbr);
	status = prg_data.lst_cmd[prg_data.cmd_nbr - 1].exit_status;
	// free_garbeg(&prg_data);
	return (status);
}


int	execute_cmd_line(t_ast_node *root, char **envp)
{
	int	i;
	// 1) Expansion pass on the entire tree
	// error handeling happens in the execution not expansion
	// the root childs are pipelines
	i = 0;
	// expand_ast(root);
	// after expansion envp var would be changed
	while ( (root && root->children) && i < root->children->length)
	{
		status = execute_pipeline(root->children->items[i],envp);
		if (!status) //in case of success
		{
			if (((t_ast_node *)root->children->items[i++])->type == AST_AND)
				status = execute_pipeline(root->children->items[i++],envp);
			else
				return (status);
		}
		else //in case of failuer
		{
			if (((t_ast_node *)root->children->items[i++])->type == AST_OR)
				status = execute_pipeline(root->children->items[i++],envp);
			else
				return (status);
		}
	}
	return (status);
}

int lst_size(t_env *head)
{
	int i;
	i = 0;
	while (head)
	{
		i++;
		head = head->next;
	}
	return i;
}

char **extract_envp(t_env *env)
{
	t_env *head = env;
	char *tmp;
	char **envp;
	int i;

	envp = ft_malloc(sizeof(char **), lst_size(env) + 1);
	i = 0;
	while (head)
	{
		tmp = ft_strjoin(head->key,"=");
		envp[i++] = ft_strjoin(tmp,head->value);
		head = head->next;
	}
	envp[i] = NULL;
	return envp;
}

int execution(t_ast_node *root,t_env *env)
{
	char **envp;
	envp = extract_envp(env);
	expand_ast(root);
	return execute_cmd_line(root , envp);
}

// {key : o , value : ls} , strjoin(strjoin(o, =),ls)