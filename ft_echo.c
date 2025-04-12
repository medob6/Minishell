/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 07:45:54 by salahian          #+#    #+#             */
/*   Updated: 2025/04/11 10:08:01 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_error(int a)
{
	static int	c;

	if (a != 0)
		c = a;
	return (c);
}

int	ft_print(char c)
{
	if (ft_error(0) != -1)
		ft_error(write(1, &c, 1));
	return (ft_error(0));
}

void    print(char *s)
{
    int     i;

    i = 0;
    while (s[i])
    {
        ft_print(s[i]);
        i++;
    }
}

int     check_the_string(char *s)
{
    int     i;

    i = 2;
    while (s[i])
    {
        if (s[i] != 'n')
            return (0);
        i++;
    }
    return (1);
}

int     ft_echo(char **args)
{
    int     i;
    int     new_line;

    new_line = 1;
    i = 1;
    ft_error(1);
    if (args[1] && ft_strncmp(args[1], "-n", 2) == 0 && check_the_string(args[1]))
    {
        i = 2;
        new_line = 0;
    }
    while(args && args[i])
    {
        print(args[i]);
        i++;
    }
    if (new_line)
       ft_print('\n');
    if (ft_error(0) == -1)
		return (1);
    return (0); 
}

int main(void)
{
	char *test1[] = {"echo", "hello", "world", NULL};
	char *test2[] = {"echo", "-n", "no ", "newline", NULL};
	char *test3[] = {"echo", NULL};

	//printf("Test 1:\n");
	//ft_echo(test1); // Output: hello world\n

	printf("Test 2:\n"); 
	ft_echo(test2); // Output: no newline

	// printf("Test 3:\n");
	// ft_echo(test3); // Output: \n

	return 0;
}