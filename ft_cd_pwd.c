/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:40:08 by salahian          #+#    #+#             */
/*   Updated: 2025/04/13 15:10:43 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_pwd(t_env **env_list)
{
    char *cwd;
    t_env *cur;

    cwd = getcwd(NULL, 0);
    if (!cwd)
    {
        perror("getcwd");
        return (0);
    }
    cur = *env_list;
    while (cur)
    {
        if (strcmp(cur->key, "PWD") == 0)
        {
            free(cur->value);
            cur->value = strdup(cwd);
            break;
        }
        cur = cur->next;
    }
    free(cwd);
    return (1);
}

int update_pwd(t_env **env_list, char *cwd)
{
	t_env *cur;

    cur = *env_list;
	while (cur)
	{
		if (strcmp(cur->key, "PWD") == 0)
		{
			free(cur->value);
			cur->value = strdup(cwd);
			if (!cur->value)
			{
				perror("strdup");
				return (0);
			}
			return (1);
		}
		cur = cur->next;
	}
	return (0);
}

int ft_cd(char *path, t_env **env_list)
{
	char *cwd;

	if (!path)
		return (write(1, "cd: path required\n", 19), 1);
	if (chdir(path) != 0)
		return (perror("cd"), 1);
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("cd: getcwd failed (directory structure might be broken)");
		return (1);
	}
	if (!update_pwd(env_list, cwd))
	{
		write(2, "cd: failed to update PWD\n", 26);
		free(cwd);
		return (1);
	}
	free(cwd);
	return 0;
}
