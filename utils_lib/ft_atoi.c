/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:44:05 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/16 22:10:21 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(char *str)
{
	long	res;
	int		sign;
	int		i;

	sign = 1;
	i = 0;
	res = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] != '\0' && ft_isdigit(str[i]))
	{
		if (res > (LONG_MAX - (str[i] - '0')) / 10)
			return (-sign * (sign == 1));
		res = res * 10 + ((str[i] - '0'));
		i++;
	}
	return ((int)res * sign);
}
