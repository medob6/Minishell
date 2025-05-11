/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 08:41:04 by salahian          #+#    #+#             */
/*   Updated: 2025/05/10 17:26:50 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

long	ft_atoi_flag(char *str, int *flag)
{
	long	re;
	int		i;
	int		sign;

	i = 0;
	re = 0;
	sign = 1;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (!str[i])
		return (*flag = 1, 0);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if ((sign == 1 && re > (LONG_MAX - (str[i] - '0')) / 10) || (sign == -1
				&& (unsigned long)re > ((unsigned long)LONG_MAX + 1 - (str[i]
						- '0')) / 10))
			return (*flag = 1, 0);
		re = (re * 10) + (str[i] - '0');
		i++;
	}
	return (re * sign);
}

int	ft_exit(char **args, long last_status )
{
	int		flag;
	long	num;

	flag = 0;
	if (args[1] && args[2])
	{
		num = ft_atoi_flag(args[1], &flag);
		if (flag)
		{
			ft_print("exit\nminishell: exit: numeric argument required\n", 2);
			exit(2);
		}
		ft_print("exit\nminishell: exit: too many arguments\n", 2);
		return (1);
	}
	if (!args[1])
	{
		ft_print("exit\n", 1);
		exit(last_status);
	}
	num = ft_atoi_flag(args[1], &flag);
	if (flag)
	{
		ft_print("exit\nminishell: exit: numeric argument required\n", 2);
		exit(2);
	}
	ft_print("exit\n", 1);
	exit((int)num);
}

// int	main(void)
// {
//     char *test1[] = {"exit", NULL};
// 	char *test2[] = {"exit", "9223372036854775808", NULL};
// 	char *test3[] = {"echo", NULL};
// 	long	code = ft_exit(test2, 0);
// 	printf("%ld\n", code);
// 	return (code);
// }
