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
void	redirect(t_token *file_obj)
{
	int	fd;

	fd = open_file(file_obj);
	if (fd != -1)
	{
		if (file_obj->type == TOKEN_APPEND
			|| file_obj->type == TOKEN_REDIRECT_OUT)
			dup2(fd, STDOUT_FILENO);
		else
			dup2(fd, STDIN_FILENO);
	}
	close(fd);
}

void	perforem_redirections(t_data *data, int n)
{
	int		i;
	t_token	**redir_lst;

	i = 0;
	// all good for one cmd
	redir_lst = data->lst_cmd[n].redirlist;
	if (n != data->cmd_nbr - 1)
		close(data->fd[0]);
	if (data->old_fd != -1)
	{
		dup2(data->old_fd, STDIN_FILENO);
		close(data->old_fd);
	}
	while (redir_lst && redir_lst[i])
	{
		printf(" i = %d \n",i);
		redirect(redir_lst[i]);
		i++;
	}
	if (n != data->cmd_nbr - 1)
	{
		dup2(data->out_fd, STDOUT_FILENO);
		close(data->out_fd);
	}
}

void	parent(int *old_fd, int *fd)
{
	close(fd[1]);
	if (*old_fd >= 0)
		close(*old_fd);
	*old_fd = fd[0];
}

void	child(t_data *prg_data, int index)
{
	perforem_redirections(prg_data, index);
	execute_cmd(prg_data->lst_cmd[index], prg_data);
	ft_exit(prg_data, 1);
}

void	excute_child(t_data *prg_data, int index)
{
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
			excute_child(prg_data, i);
		else
			parent(&prg_data->old_fd, prg_data->fd);
		i++;
	}
}

bool	parse_cmd(t_cmd *cmd, t_ast_node *node, char **envp)
{
	char	**cmd_args;

	cmd_args = NULL;
	if ((t_token **)node->children)
		cmd_args = (char **)node->children->items;
	if ((t_token **)node->redirect_list)
		cmd->redirlist = (t_token **)node->redirect_list->items;
	if (!cmd_args || cmd_args[0] == NULL)
	{
		if (!cmd_args[0])
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
		// TODO: perform redirections in subshell
		// }
		i++;
	}
	return (cmd_lst);
}

int	execute_pipeline(t_ast_node *pipeline, char **envp)
{
	t_data	prg_data;
	int		status;

	// TODO :  make a function that initialize var
	prg_data.old_fd = -1;
	prg_data.out_fd = -1;
	prg_data.fd[0] = -1;
	prg_data.fd[1] = -1;

	prg_data.cmd_nbr = pipeline->children->length;
	prg_data.lst_cmd = parse_cmd_list(prg_data.cmd_nbr,
			(t_ast_node **)pipeline->children->items, envp);
	prg_data.envp = envp;
	pipe_exec(&prg_data);
	wait_for_prc(prg_data.lst_cmd, prg_data.cmd_nbr);
	status = prg_data.lst_cmd[prg_data.cmd_nbr - 1].exit_status;
	free_garbeg(&prg_data);
	return (status);
}

int execute_cmd_line(t_ast_node *root, char **envp)
{
    size_t i = 0;
    int status = 0;

    if (!root || !root->children || root->children->length == 0)
        return (0);

    while (i < root->children->length) {
        t_ast_node *cmd = (t_ast_node *)root->children->items[i];
        status = execute_pipeline(cmd, envp);

        if (++i < root->children->length) {
            t_ast_node *op = (t_ast_node *)root->children->items[i];

            if ((status == 0 && op->type == AST_AND) ||(status != 0 && op->type == AST_OR))
			{
                ++i;
                continue;
            } else {
                return status;
            }
        } else {
            break;
        }
    }
    return status;
}

int	execution(t_ast_node *root, t_env *env)
{
	int		n;
	char	**envp;

	envp = extract_envp(env);
	// expand_ast(root); // i am still waiting for my partener to make this one
	n = execute_cmd_line(root, envp);// the problem of prompt not shoing out is becuase of this line
	return (n);
}
