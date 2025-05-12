/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:34:48 by salahian          #+#    #+#             */
/*   Updated: 2025/05/12 13:47:11 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_env(t_env **env, int fd)
{
	t_env	*tmp;
	char 	*str;

	if (!env)
		return (1);
	tmp = *env;
	while (tmp)
	{
		if (tmp->value_set)
		{
			str = ft_strjoin(ft_strjoin(tmp->key, "="), tmp->value);
			ft_print(str, fd);
			ft_print("\n", fd);
		}
		tmp = tmp->next;
	}
	return (0);
}

//TODO : if SHLVL more the 0-999 or not int , the next shlvl must be 0
//TODO : if env run in child it must must be previos shlvl - 1 allways 
//TODO: left one () for simple command  


// int	main(int argc, char **argv, char **envp)
// {
// 	(void)argc;
// 	(void)argv;
// 	return (ft_env(envp));
// }
