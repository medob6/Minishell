/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:21:16 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/05 17:22:07 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	save_old_fd(int *old_fd, int *fd)
{
	close(fd[1]);
	if (*old_fd >= 0)
		close(*old_fd);
	*old_fd = fd[0];
}

void	reset_fds(t_data *data, int n)
{
	if (n != data->cmd_nbr - 1)
		close(data->fd[0]);
	if (data->old_fd != -1)
	{
		dup2(data->old_fd, STDIN_FILENO);
		close(data->old_fd);
	}
	if (n != data->cmd_nbr - 1)
	{
		if (data->out_fd != -1)
			data->out_fd = data->fd[1];
		else
			close(data->fd[1]);
	}
}

void	close_here_docs(t_token **redir_list)
{
	int	i;

	i = 0;
	while (redir_list && redir_list[i])
	{
		if (redir_list[i]->type == TOKEN_HEREDOC)
			close(redir_list[i]->value.fd_value);
		i++;
	}
	return ;
}

// TODO
// i should pull new tokenizer code : //!DONE
// what i should do now :
// 1- implement builting in execution !!!!!!!!!	//! DONE
// 2- upgrade heredoc code 			!!!!!!!!! have some errors and linked to 7	//! DONE
// 3- extract envp before rederection	//! DONE
// 4- check for imbiguse rederictions	//! DONE
// 5- implement subshell				!!!!!!!!!!  //! DONE
// 6- handel exit status code we have five (also in built-in); //! DONE
// 7- remove paranteses in parsing  //! DONE
// 8- check for save derefrencing     //?  after claening the code
// 9- test execuiton  //? in progress

// 10- test everything //TODO when expansion is finished
//! handel
// ```
// /tmp/hey$ unset a
// /tmp/hey$ $a ls
// minishell: command not found:
// ```
//! PROBLEMS
// we got a lot of problems that should be fixed

// if i have : export f="f" && echo $f
// if i exported before parsing it will change f to ""
// so i need to expand before runing the command
// first extract them enpv and let them in there linked list  then before runing export.... ,extract them to envp char ** ; then expand :::: this is a prblm soo
// TODO i will expand for each node of simple_cmd alone ......................... tomorow cus it will take a lot of time
// second export will add to the list , and we will do the same again
// export output e.g env content must not be completely empthy after unset ,it should have PWD and  SHLVL ...

// TODO FOR solaiman
//! FIX built-ins  :
// ignor comments as argments #somthin
// -nnnn for echo should be ignored even after first flag
// += append for export
// should pass NULL to cat in case of empthy expansion not '' or ""
// if runing export without args or with a comment it must show the env vars
// for cd it must if given more the one arg show err "to many args" and do nothing
// test more , and change terminal path
// pwd doesnt work at all it must print out the current working dir path ,and it must take a file descriptor in accoredens with the execution
// unset seems fine but need further testing and err handeling
// env is fine , should be tested with unset , and export
// last status need to be fixed in exit , testing with args
//! DO in echo handell case if write fails perror("minishell: ");

// in expansion if an expansion resulted a empthy string and nothing was linked to the arg expanded ,it must be removed rather then stored as a empthy str ,ask mahedi
// if i expand a file name in redirections and