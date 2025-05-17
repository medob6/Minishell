/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:40:08 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 16:18:42 by mbousset         ###   ########.fr       */
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

int	update_pwd(t_env **env_list, char *cwd)
{
	t_env	*cur;
	char	*old;
	int		i;

	cur = *env_list;
	i = 0;
	while (cur)
	{
		if (!ft_strcmp(cur->key, "PWD"))
		{
			i = 1;
			old = cur->value;
			cur->value = ft_strdup(cwd);
		}
		else if (!ft_strcmp(cur->key, "OLDPWD"))
		{
			i = 1;
			cur->value = old;
		}
		cur = cur->next;
	}
	return (i);
}

void	print_error_cd(char *message, char *path)
{
	print_str_fd("cd: ", 2);
	print_str_fd(message, 2);
	if (path)
	{
		print_str_fd(": ", 2);
		print_str_fd(path, 2);
	}
	print_str_fd("\n", 2);
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
			return (print_str_fd("minishell: cd: HOME not set\n", 2), 1);
	}
	if (chdir(path) != 0)
		return (print_error_cd(strerror(errno), path), 1);
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		print_error_cd(strerror(errno), path);
		return (1);
	}
	if (!update_pwd(env_list, cwd))
	{
		print_str_fd("cd: failed to update PWD\n", 2);
		return (free(cwd), 1);
	}
	free(cwd);
	return (0);
}
