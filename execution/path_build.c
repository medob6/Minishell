/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_build.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 13:47:34 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/06 13:47:35 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	*get_path(char **paths)
{
	int	i;

	i = 0;
	while (paths && paths[i])
	{
		if (!ft_strncmp("PATH=", paths[i], 5))
			return (paths[i]);
		i++;
	}
	return (NULL);
}

char	*build_cmd_path(char *cmd, char *path)
{
	char	*cmd_path;
	char	*tmp;

	cmd_path = NULL;
	tmp = ft_strjoin(path, "/");
	cmd_path = ft_strjoin(tmp, cmd);
	ft_free(tmp);
	return (cmd_path);
}

int	is_dir(char *path)
{
	char	*new_path;
	int		ans;

	ans = 0;
	new_path = ft_strjoin(path, "/");
	if (access(new_path, F_OK) == 0)
		ans = 1;
	ft_free(new_path);
	return (ans);
}

char	*get_cmd_path(char *cmd, char **envp)
{
	char	*paths;
	char	**args;
	char	*cmd_path;
	int		i;

	i = 0;
	cmd_path = NULL;
	paths = get_path(envp);
	if (paths == NULL)
		return (NULL);
	args = ft_split(&paths[5], ':');
	while (args[i])
	{
		cmd_path = build_cmd_path(cmd, args[i]);
		if (access(cmd_path, F_OK) == 0 && !is_dir(cmd_path))
			break ;
		ft_free(cmd_path);
		cmd_path = NULL;
		i++;
	}
	free_pointers(args);
	return (cmd_path);
}
