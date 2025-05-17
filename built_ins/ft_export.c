/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:10:17 by salahian          #+#    #+#             */
/*   Updated: 2025/05/16 22:06:01 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*duplicate_env(t_env *env)
{
	t_env	*new_list;
	t_env	*current;
	t_env	*new_node;

	new_list = NULL;
	current = env;
	while (current)
	{
		new_node = ft_malloc(sizeof(t_env), 1);
		new_node->key = ft_strdup(current->key);
		if (current->value)
			new_node->value = ft_strdup(current->value);
		else
			new_node->value = NULL;
		new_node->value_set = current->value_set;
		new_node->next = NULL;
		add_the_new(&new_list, new_node);
		current = current->next;
	}
	return (new_list);
}

void	sort_env_list(t_env **env)
{
	t_env	*i;
	t_env	*j;

	i = *env;
	while (i && i->next)
	{
		j = i->next;
		while (j)
		{
			if (ft_strcmp(i->key, j->key) > 0)
				swap_env_nodes(i, j);
			j = j->next;
		}
		i = i->next;
	}
}

int	print_sorted_lst(t_env *env, int fd)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, "_"))
		{
			tmp = tmp->next;
			continue ;
		}
		if (!print_str_fd("declare -x ", fd))
			return (0);
		print_str_fd(tmp->key, fd);
		if (tmp->value_set && tmp->value)
		{
			print_str_fd("=\"", fd);
			print_str_fd(tmp->value, fd);
			print_str_fd("\"", fd);
		}
		print_str_fd("\n", fd);
		tmp = tmp->next;
	}
	return (1);
}

int	export_env(char **args, t_env **env)
{
	int	i;
	int	sep;
	int	status;

	i = 1;
	sep = 0;
	status = 0;
	while (args[i])
	{
		if (!check_is_okey(args[i]))
		{
			print_str_fd("minishell: export: not a valid identifier\n", 2);
			status = 1;
		}
		else
		{
			sep = find_equal_pos(args[i]);
			add_or_update_env(args[i], sep, env);
		}
		i++;
	}
	return (status);
}

int	ft_export(char **args, t_env **env, int fd)
{
	t_env	*sorted_env;
	int		status;

	status = 0;
	if (!args[1])
	{
		sorted_env = duplicate_env(*env);
		sort_env_list(&sorted_env);
		if (!print_sorted_lst(sorted_env, fd))
			return (1);
		free_env_list(sorted_env);
		return (0);
	}
	status = export_env(args, env);
	return (status);
}
