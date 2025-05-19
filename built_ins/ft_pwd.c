/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 08:38:42 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/19 08:39:00 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd(t_env **env_list, int fd)
{
	char	*cwd;
	t_env	*cur;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		print_str_fd(ft_strjoin(expand_the_value("$PWD", env_list), "\n"), fd);
		return (0);
	}
	cur = *env_list;
	if (!print_str_fd(ft_strjoin(cwd, "\n"), fd))
		return (1);
	while (cur)
	{
		if (!ft_strcmp(cur->key, "PWD"))
		{
			cur->value = ft_strdup(cwd);
			break ;
		}
		cur = cur->next;
	}
	free(cwd);
	return (0);
}
