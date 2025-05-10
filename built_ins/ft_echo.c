/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 07:45:54 by salahian          #+#    #+#             */
/*   Updated: 2025/05/10 14:46:55 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_n_flag(char *s)
{
	int	i;

	i = 1;
	if (s[0] != '-')
		return (0);
	while (s[i])
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	print_str_fd(char *s, int fd)
{
	int	n;

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

	i = 1;
	new_line = 1;
	while (args[i] && check_n_flag(args[i]))
	{
		new_line = 0;
		i++;
	}
	while (args[i])
	{
		if (!print_str_fd(args[i], fd))
			return (1);
		if (args[i + 1])
			print_str_fd(" ", fd);
		i++;
	}
	if (new_line)
		print_str_fd("\n", fd);
	if (fd != 1)
		close(fd);
	return (0);
}
