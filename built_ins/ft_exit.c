/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 08:41:04 by salahian          #+#    #+#             */
/*   Updated: 2025/05/16 21:35:09 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_nbr(char *number)
{
	int	n;

	if (number && is_correct_nbr(number))
	{
		n = ft_atoi(number);
		return ((unsigned char)n);
	}
	return (-1);
}

int	ft_exit(char **args)
{
	long	num;

	if (!args[1])
		return (*get_last_status());
	num = get_nbr(args[1]);
	if (args[2])
	{
		if (num == -1)
		{
			print_str_fd("minishell: exit: numeric argument required\n", 2);
			return (2);
		}
		print_str_fd("minishell: exit: too many arguments\n", 2);
		return (-1);
	}
	if (num == -1)
	{
		print_str_fd("minishell: exit: numeric argument required\n", 2);
		return (2);
	}
	return (num);
}
