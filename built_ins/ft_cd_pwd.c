/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:40:08 by salahian          #+#    #+#             */
/*   Updated: 2025/05/10 14:30:58 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd(t_env **env_list,int fd)
{
	char	*cwd;
	t_env	*cur;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (0);
	cur = *env_list;
	ft_print(ft_strjoin(cwd, "\n"), fd);
	while (cur)
	{
		if (ft_strncmp(cur->key, "PWD", ft_strlen("PWD")) == 0)
		{
			cur->value = ft_strdup(cwd);
			break ;
		}
		cur = cur->next;
	}
	free(cwd);
	return (0);
}

int	update_pwd(t_env **env_list, char *cwd)
{
	t_env	*cur;
	char	*old;
	int		i;

	cur = *env_list;
	i = 0;
	while (cur)
	{
		if (ft_strncmp(cur->key, "PWD", ft_strlen("PWD")) == 0)
		{
			i = 1;
			old = cur->value;
			cur->value = ft_strdup(cwd);
		}
		else if (ft_strncmp(cur->key, "OLDPWD", ft_strlen("OLDPWD")) == 0)
		{
			i = 1;
			cur->value = old;
		}
		cur = cur->next;
	}
	return (i);
}

int	ft_cd(char *path, t_env **env_list)
{
	char	*cwd;

	if (path && path[0] == '\0')
		return (0);
	if (!path)
	{
		path = expand_the_value("$HOME", env_list);
		if (!path || path[0] == '\0')
			return (0);
	}
	if (chdir(path) != 0)
		return (printf("cd: %s %s\n", strerror(errno), path), 1);
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		printf("cd: %s %s\n", strerror(errno), path);
		return (1);
	}
	if (!update_pwd(env_list, cwd))
	{
		ft_print("cd: failed to update PWD\n", 2);
		return (free(cwd), 1);
	}
	free(cwd);
	return (0);
}
