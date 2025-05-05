#include "execution.h"



void	perforem_subshell_redirs(t_data *data, int n)
{
	size_t	i;
	t_token	**redir_lst;

	i = 0;
	if (data->lst_cmd[n].subshell_node->redirect_list)
		redir_lst = (t_token **)data->lst_cmd[n].subshell_node->redirect_list->items;
	else
		redir_lst = NULL;
	if (n != data->cmd_nbr - 1)
		close(data->fd[0]);
	if (data->old_fd != -1)
	{
		dup2(data->old_fd, STDIN_FILENO);
		close(data->old_fd);
	}
	if (n != data->cmd_nbr - 1)
	{
		dup2(data->fd[1], STDOUT_FILENO);
		close(data->fd[1]);
	}
	while (redir_lst && redir_lst[i])
	{
		redirect(data, redir_lst[i]);
		i++;
	}
}