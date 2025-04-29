/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 07:45:54 by salahian          #+#    #+#             */
/*   Updated: 2025/04/29 11:28:29 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_string(char *s)
{
	int	i;

	i = 2;
	while (s[i])
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	ft_echo(char **args, int fd)
{
	int	i;
	int	new_line;

	new_line = 1;
	i = 1;
	//if (fd < 0)
	//ft_print("bash: echo: write error: No space left on device\n", 2);
	if (args[1] && ft_strncmp(args[1], "-n", 2) == 0 && check_string(args[1]))
	{
		i = 2;
		new_line = 0;
	}
	while (args && args[i])
	{
		if (args[i][0] == '#')
			break ;
		else if (args[i] && ft_strncmp(args[i], "-n", 2) == 0
			&& check_string(args[i]))
			i++;
		else
		{
			ft_print(args[i], fd);
			ft_print(" ", fd);
		}
		i++;
	}
	if (new_line)
		ft_print("\n", fd);
	if (fd != 1)
		close(fd);
	return (0);
}

