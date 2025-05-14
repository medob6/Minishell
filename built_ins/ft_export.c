/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:10:17 by salahian          #+#    #+#             */
/*   Updated: 2025/05/13 18:11:32 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	not_valid(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0'
			&& c <= '9') || c == '_');
}

int	check_is_okey(char *s)
{
	int	i;

	if (!not_valid(s[0]) || (s[0] >= '0' && s[0] <= '9'))
		return (0);
	for (i = 0; s[i] && s[i] != '='; i++)
	{
		if (s[i] == '+' && s[i + 1] == '=')
			return (1);
		if (!not_valid(s[i]))
			return (0);
	}
	return (1);
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

void	add_the_new(t_env **env, t_env *new)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp && tmp->next)
		tmp = tmp->next;
	if (!tmp)
		*env = new;
	else
		tmp->next = new;
}

int	find_equal_pos(char *s)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == '=' || (s[i] == '+' && s[i + 1] == '='))
			return (i);
		i++;
	}
	return (-1);
}

t_env	*update_existing_env(t_env *env, char *arg, int sep)
{
	while (env)
	{
		if ((sep != -1 && (!ft_strncmp(arg, env->key, sep)
					&& env->key[sep] == '\0')) || (sep == -1 && !ft_strcmp(arg,
					env->key)))
		{
			if (sep != -1 && arg[sep] == '=')
			{
				ft_free(env->value);
				env->value = ft_strdup(&arg[sep + 1]);
				env->value_set = true;
			}
			return (env);
		}
		env = env->next;
	}
	return (NULL);
}

void	add_or_update_env(char *arg, int sep, t_env **env)
{
	t_env	*tmp;
	t_env	*existing;
	char	*new_value;
	t_env	*new;

	tmp = *env;
	existing = update_existing_env(tmp, arg, sep);
	if (existing)
	{
		if (sep != -1 && arg[sep] == '+' && arg[sep + 1] == '=')
		{
			new_value = ft_strjoin(existing->value ? existing->value : "",&arg[sep + 2]);
			ft_free(existing->value);
			existing->value = new_value;
			existing->value_set = true;
		}
		else if (sep != -1 && arg[sep] == '=')
		{
			ft_free(existing->value);
			existing->value = ft_strdup(&arg[sep + 1]);
			existing->value_set = true;
		}
	}
	else
	{
		if (sep != -1 && arg[sep] == '+' && arg[sep + 1] == '=')
		{
			new = create_new(arg, sep);
			new->value = ft_strdup(&arg[sep + 2]);
			new->value_set = true;
			add_the_new(env, new);
		}
		else
		{
			add_the_new(env, create_new(arg, sep));
		}
	}
}

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
		new_node->value = current->value ? ft_strdup(current->value) : NULL;
		new_node->value_set = current->value_set;
		new_node->next = NULL;
		add_the_new(&new_list, new_node);
		current = current->next;
	}
	return (new_list);
}

void	sort_env_list(t_env **env)
{
	bool	tmp_value_set;

	t_env *i, *j;
	char *tmp_key, *tmp_value;
	for (i = *env; i && i->next; i = i->next)
	{
		for (j = i->next; j; j = j->next)
		{
			if (ft_strcmp(i->key, j->key) > 0)
			{
				tmp_key = i->key;
				i->key = j->key;
				j->key = tmp_key;
				tmp_value = i->value;
				i->value = j->value;
				j->value = tmp_value;
				tmp_value_set = i->value_set;
				i->value_set = j->value_set;
				j->value_set = tmp_value_set;
			}
		}
	}
}

int	print_sorted_lst(t_env *env, int fd)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
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

int	ft_export(char **args, t_env **env, int fd)
{
	t_env	*sorted_env;

	int i, sep, status;
	i = 1;
	sep = 0;
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
