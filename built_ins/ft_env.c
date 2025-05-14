/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 16:34:48 by salahian          #+#    #+#             */
/*   Updated: 2025/05/13 18:00:16 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// int	print_str_fd(char *s, int fd)
// {
// 	int	n;

// 	if (fd < 0 || !s)
// 		return (0);
// 	n = write(fd, s, ft_strlen(s));
// 	if (n < 0)
// 	{
// 		ft_putstr_fd("bash: echo: write error: No space left on device\n", 2);
// 		return (0);
// 	}
// 	return (1);
// }

int	ft_env(t_env **env, int fd)
{
	t_env	*tmp;
	char	*str;

	if (!env)
		return (1);
	tmp = *env;
	while (tmp)
	{
		if (tmp->value_set)
		{
			str = ft_strjoin(ft_strjoin(tmp->key, "="), tmp->value);
			if (!print_str_fd(str, fd))
				return (125);
			print_str_fd("\n", fd);
		}
		tmp = tmp->next;
	}
	return (0);
}

// TODO : if SHLVL more the 0-999 or not int , the next shlvl must be 0
// TODO : if env run in child it must must be previos shlvl - 1 allways
// TODO: left one () for simple command
