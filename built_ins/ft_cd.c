/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:40:08 by salahian          #+#    #+#             */
/*   Updated: 2025/05/19 08:39:22 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	update_pwd_value(t_env **env_list, char *cwd, char **old_pwd)
{
	t_env	*cur;

	cur = *env_list;
	while (cur)
	{
		if (!ft_strcmp(cur->key, "PWD"))
		{
			*old_pwd = cur->value;
			cur->value = ft_strdup(cwd);
			return (1);
		}
		cur = cur->next;
	}
	return (0);
}

int	update_oldpwd_value(t_env **env_list, char *old_pwd)
{
	t_env	*cur;

	cur = *env_list;
	while (cur)
	{
		if (!ft_strcmp(cur->key, "OLDPWD"))
		{
			cur->value = old_pwd;
			return (1);
		}
		cur = cur->next;
	}
	return (0);
}

int	update_pwd(t_env **env_list, char *cwd)
{
	char	*old_pwd;
	int		status;

	old_pwd = NULL;
	status = update_pwd_value(env_list, cwd, &old_pwd);
	if (status && old_pwd)
		status += update_oldpwd_value(env_list, old_pwd);
	return (status);
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
