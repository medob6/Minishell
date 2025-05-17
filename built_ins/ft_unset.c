/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:44:05 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 19:11:14 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	remove_from_the_list(t_env **head, t_env *tmp, t_env *prev)
{
	if (prev == NULL)
		*head = tmp->next;
	else
		prev->next = tmp->next;
}

int	ft_unset(char **args, t_env **env)
{
	int		i;
	t_env	*tmp;
	t_env	*prev;

	env = get_env_head_addres(NULL);
	i = 1;
	while (args && args[i])
	{
		tmp = *env;
		prev = NULL;
		while (tmp)
		{
			if (!ft_strcmp(tmp->key, args[i]))
			{
				remove_from_the_list(env, tmp, prev);
				return (0);
			}
			prev = tmp;
			tmp = tmp->next;
		}
		i++;
	}
	return (1);
}
