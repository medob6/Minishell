/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 07:45:54 by salahian          #+#    #+#             */
/*   Updated: 2025/05/06 15:48:28 by mbousset         ###   ########.fr       */
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
int print_str_fd(char *s,int fd)
{
	int n;
	if (fd < 0 || !s)
		return (0);
	n = write(fd, s, ft_strlen(s));
	if (n < 0)
	{
		ft_putstr_fd("bash: echo: write error: No space left on device\n", 2);
		return (0);
	}
	return (1);
}

int	ft_echo(char **args, int fd)
{
	int	i;
	int	new_line;
	
	new_line = 1;
	i = 1;
	// if (fd < 0)// this check must be done if and only if write fails cus fd fails handeled in execution ok
	// 	ft_print("bash: echo: write error: No space left on device\n", 2);
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
			if (!print_str_fd(args[i], fd))
			{
				close(fd);
				return (1);
			}
			print_str_fd(" ", fd);
		}
		i++;
	}
	if (new_line)
		print_str_fd("\n", fd);
	if (fd != 1)
		close(fd);
	return (0);
}
