/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbeg_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 17:43:57 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/05 17:43:58 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

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
}
