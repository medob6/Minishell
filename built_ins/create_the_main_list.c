/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_the_main_list.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:43:01 by salahian          #+#    #+#             */
/*   Updated: 2025/05/14 09:34:33 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// TODO add to utils

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

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
//////////////////////////
bool	is_correct_nbr(char *number)
{
	int	i;

	i = 0;
	if (number[i] == '-' || number[i] == '+')
		i++;
	while (number[i])
	{
		if (ft_isdigit(number[i]))
			i++;
		else
			return (false);
	}
	return (true);
}

int	check_value(char *number)
{
	int	n;

	if (number && is_correct_nbr(number))
	{
		n = ft_atoi(number);
		if (n < 0 || n > 999)
			return (-1);
		return (n);
	}
	return (-1);
}

void	update_shlvl(t_env *env_list)
{
	int	current_lvl;

	while (env_list)
	{
		if (ft_strcmp(env_list->key, "SHLVL") == 0)
		{
			current_lvl = check_value(env_list->value);
			ft_free(env_list->value);
			env_list->value = ft_itoa(current_lvl + 1);
			return ;
		}
		env_list = env_list->next;
	}
}

t_env	*create_node(char *s, int sep)
{
	t_env	*node;

	// TODO this shit cuse buffer over flow
	node = ft_malloc(sizeof(t_env), 1);
	node->key = ft_substr(s, 0, sep);
	node->value = ft_substr(s, sep + 1, ft_strlen(s) - (sep + 1));
	node->value_set = true;
	node->next = NULL;
	return (node);
}

t_env	*fill_the_list(char **envp)
{
	int		i;
	int		j;
	t_env	*new_node;
	t_env	*head;
	t_env	*last;

	head = NULL;
	last = NULL;
	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j])
		{
			if (envp[i][j] == '=')
			{
				new_node = create_node(envp[i], j);
				if (!head)
					head = new_node;
				else
					last->next = new_node;
				last = new_node;
				break ;
			}
			j++;
		}
		i++;
	}
	update_shlvl(head);
	return (head);
}

t_env	*default_envp(int shlvl)
{
	t_env	*head;
	t_env	*shlvl_node;
	char	*cwd;

	head = ft_malloc(sizeof(t_env), 1);
	shlvl_node = ft_malloc(sizeof(t_env), 1);
	head->key = ft_strdup("PWD");
	cwd = getcwd(NULL, 0);
	head->value = ft_strdup(cwd);
	free(cwd);
	head->value_set = true;
	head->next = shlvl_node;
	shlvl_node->key = ft_strdup("SHLVL");
	shlvl_node->value = ft_itoa(shlvl);
	shlvl_node->next = NULL;
	return (head);
}

t_env	*create_the_main_list(char **envp, int shlvl)
{
	if (!*envp)
	{
		return (default_envp(shlvl));
	}
	return (fill_the_list(envp));
}
