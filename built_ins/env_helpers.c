/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:42:05 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/16 21:57:25 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	free_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		ft_free(tmp->key);
		if (tmp->value)
			ft_free(tmp->value);
		ft_free(tmp);
	}
}

t_env	*create_new(char *s, int sep)
{
	t_env	*node;

	node = ft_malloc(sizeof(t_env), 1);
	node->key = ft_substr(s, 0, sep);
	node->value = NULL;
	node->value_set = false;
	if (sep != -1 && s[sep] == '=')
	{
		node->value = ft_substr(s, sep + 1, ft_strlen(s) - sep - 1);
		node->value_set = true;
	}
	node->next = NULL;
	return (node);
}
