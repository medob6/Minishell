/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:40:08 by salahian          #+#    #+#             */
/*   Updated: 2025/05/08 15:49:19 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_pwd(t_env **env_list)
{
	char	*cwd;
	t_env	*cur;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("getcwd");
		return (0);
	}
	cur = *env_list;
	ft_print(ft_strjoin(cwd, "\n"), 1);
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
	return (1);
}

int	update_pwd(t_env **env_list, char *cwd)
{
	t_env	*cur;
	char	*old;

	cur = *env_list;
	while (cur)
	{
		if (ft_strncmp(cur->key, "PWD", ft_strlen("PWD")) == 0)
		{
			old = cur->value;
			cur->value = ft_strdup(cwd);
		}
		else if (ft_strncmp(cur->key, "OLDPWD", ft_strlen("OLDPWD")) == 0)
		{
			cur->value = old;
			return (1);
		}
		cur = cur->next;
	}
	return (0);
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
		return (printf("cd: %s %s\n",strerror(errno),path), 1);
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		printf("cd: %s %s\n", strerror(errno), path);
		return (1);
	}
	if (!update_pwd(env_list, cwd))
	{
		ft_print("cd: failed to update PWD\n", 2);
		return (free(cwd),1);
	}
	free(cwd);
	return (0);
}
