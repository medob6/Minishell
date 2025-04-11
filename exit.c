/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 08:41:04 by salahian          #+#    #+#             */
/*   Updated: 2025/04/11 15:43:59 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int	ft_atoi(char *str, int  *flag)
// {
// 	long	re;
// 	int		i;
// 	int		sign;

// 	i = 0;
// 	re = 0;
// 	sign = 1;
// 	if (str[i] == '-' || str[i] == '+')
// 	{
// 		if (str[i] == '-')
// 			sign = -1;
// 		i++;
// 	}
// 	while (str[i])
// 	{
//         if (str[i] >= '0' && str[i] <= '9')
//         {
// 		    re = (re * 10) + (str[i] - '0');
// 		    if ((sign == 1 && re > INT_MAX) || (sign == -1 && (re * -1) < INT_MIN))
// 			    *flag = 1;
// 		}
//         else
//             *flag = 1; 
// 		i++;
// 	}
// 	return (re * sign);
// }

// int	ft_exit(char **args, int last_status)
// {
//     int     i;
//     int     flag;
//     int     num;

//     i = 0;
//     flag = 0;
//     if (args[1] && args[2])
//     {
//         write(2, "exit\nminishell: exit: too many arguments\n", 42);
//         return (1);
//     }
//     if (args[1] == NULL)
//         return (last_status);
//     num = ft_atoi(args[1], &flag);
//     if (flag)
//     {
//         write(2, "exit\nminishell: exit: numeric argument required\n", 49);
//         return (2);
//     }
//     write(1, "exit\n", 5);
//     return (num);
// }

#include "minishell.h"
#include <limits.h>
#include <unistd.h>

long     ft_atoi(char *str, int *flag)
{
        long    re;
        int             i;
        int             sign;

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
                if ((sign == 1 && re > (LONG_MAX - (str[i] - '0')) / 10) ||
    			(sign == -1 && re > ((unsigned long)LONG_MAX + 1 - (str[i] - '0')) / 10))
                        return (*flag = 1, 0);
                re = (re * 10) + (str[i] - '0');
                i++;
        }
        return (re * sign);
}

int	ft_exit(char **args, long last_status)
{
	int		flag;
	long		num;

	flag = 0;
	if (args[1] && args[2])
	{
		write(2, "exit\nminishell: exit: too many arguments\n", 42);
		return (1);
	}
	if (!args[1])
	{
		write(1, "exit\n", 5);
		exit(last_status);
	}
	num = ft_atoi(args[1], &flag);
	if (flag)
	{
		write(2, "exit\nminishell: exit: numeric argument required\n", 49);
		exit(2);
	}
	//printf("h\n");
	write(1, "exit\n", 5);
	exit((unsigned char)num);
}

int	main(void)
{
    char *test1[] = {"exit", NULL};
	char *test2[] = {"exit", "9223372036854775808", NULL};
	char *test3[] = {"echo", NULL};
	long	code = ft_exit(test2, 0);
	printf("%ld\n", code);
	return code;
}
