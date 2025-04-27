/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 07:45:54 by salahian          #+#    #+#             */
/*   Updated: 2025/04/27 18:26:56 by mbousset         ###   ########.fr       */
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

int	ft_echo(char **args,int fd)
{
	int	i;
	int	new_line;

	new_line = 1;
	i = 1;
	// printf("out_fd = %d\n",fd);
	if (args[1] && ft_strncmp(args[1], "-n", 2) == 0 && check_string(args[1]))
	{
		i = 2;
		new_line = 0;
	}
	ft_print(args[i++], fd);
	while (args && args[i])
	{
		ft_print(" ", fd);
		ft_print(args[i], fd);
		i++;
	}
	if (new_line)
		ft_print("\n", fd);
	if (fd != 1)
		close(fd);
	return (0);
}

