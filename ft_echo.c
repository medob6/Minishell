/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 07:45:54 by salahian          #+#    #+#             */
/*   Updated: 2025/04/16 11:13:04 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_the_string(char *s)
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

int	ft_echo(char **args)
{
	int	i;
	int	new_line;

	new_line = 1;
	i = 1;
	if (args[1] && ft_strncmp(args[1], "-n", 2) == 0
		&& check_the_string(args[1]))
	{
		i = 2;
		new_line = 0;
	}
	while (args && args[i])
	{
		ft_print(args[i], 1);
		i++;
	}
	if (new_line)
		ft_print("\n", 1);
	return (0);
}

// int main(void)
// {
// 	char *test1[] = {"echo", "hello", "world", NULL};
// 	char *test2[] = {"echo", "-n", "no ", "newline", NULL};
// 	char *test3[] = {"echo", NULL};

// 	//printf("Test 1:\n");
// 	//ft_echo(test1); // Output: hello world\n

// 	printf("Test 2:\n");
// 	ft_echo(test2); // Output: no newline

// 	// printf("Test 3:\n");
// 	// ft_echo(test3); // Output: \n

// 	return (0);
// }