/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_pwd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:40:08 by salahian          #+#    #+#             */
/*   Updated: 2025/04/12 16:44:14 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void update_pwd(t_env **env_list)
{
    char *cwd;
    t_env *cur;

    cwd = getcwd(NULL, 0);
    if (!cwd)
    {
        perror("getcwd");
        return;
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
}

int ft_cd(char *path, t_env **env_list)
{
    if (!path)
        return (printf("cd: path required\n"), 1);
    if (chdir(path) != 0)
        return (perror("cd"), 1);
    update_pwd(env_list);
    return 0;
}
