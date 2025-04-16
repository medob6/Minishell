/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:34:48 by salahian          #+#    #+#             */
/*   Updated: 2025/04/15 16:08:23 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_env **env)
{
	t_env	*tmp;

	if (!env)
		return (1);
	tmp = *env;
	while (tmp)
	{
		ft_print(tmp->key, 1);
		ft_print(tmp->value, 1);
		ft_print("\n", 1);
		tmp = tmp->next;
	}
	return (0);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	(void)argc;
// 	(void)argv;
// 	return (ft_env(envp));
// }
