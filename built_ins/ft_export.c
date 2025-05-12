/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:10:17 by salahian          #+#    #+#             */
/*   Updated: 2025/05/12 13:54:56 by mbousset         ###   ########.fr       */
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
	i = 0;
	while (s[i] && s[i] != '=')
	{
		if (!not_valid(s[i]))
			return (0);
		i++;
	}
	return (1);
}

t_env	*create_new(char *s, int sep)
{
	t_env	*node;

	node = ft_malloc(sizeof(t_env), 1);
	node->key = ft_substr(s, 0, sep);
	node->value = NULL;
	if (s[sep] == '=')
	{
		node->value = ft_substr(s, sep + 1, ft_strlen(s) - sep - 1);
		node->value_set = true;
	}
	node->value_set = false;
	node->next = NULL;
	return (node);
}

void	add_the_new(t_env **env, t_env *new)
{
	t_env	*tmp;

	if (!*env)
		*env = new;
	else
	{
		tmp = *env;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

int	is_valid_identifier(char *s)
{
	int	i;

	i = 0;
	if (!not_valid(s[0]) || (s[0] >= '0' && s[0] <= '9'))
		return (0);
	while (s[i] && s[i] != '=')
	{
		if (!not_valid(s[i]))
			return (0);
		i++;
	}
	return (1);
}

int	find_equal_pos(char *s)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	return (i);
}

void	update_existing_env(t_env *env, char *arg, int sep)
{
	while (env)
	{
		if (ft_strncmp(arg, env->key, sep) == 0 && env->key[sep] == '\0')
		{
			if (arg[sep] == '=')
			{
				ft_free(env->value);
				env->value = ft_strdup(&arg[sep + 1]);
			}
			break ;
		}
		env = env->next;
	}
}

void	add_or_update_env(char *arg, int sep, t_env **env)
{
	t_env	*tmp;

	tmp = *env;
	update_existing_env(tmp, arg, sep);
	if (!tmp || (tmp && ft_strncmp(arg, tmp->key, sep) != 0))
	{
		if (arg[sep] == '=')
			add_the_new(env, create_new(arg, sep));
	}
}

void	print_lst(t_env **env, int fd)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		ft_print("declare -x ", fd);
		ft_print(tmp->key, fd);
		if (tmp->value_set)
		{
			ft_print("=", fd);
			ft_print(tmp->value, fd);
		}
		ft_print("\n", fd);
		tmp = tmp->next;
	}
}

int	ft_export(char **args, t_env **env, int fd)
{
	int	i;
	int	sep;
	int	status;

	status = 0;
	i = 1;
	if (!args[1])
		print_lst(env, fd);
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_print("minishell: export: not a valid identifier\n", 2);
			i++;
			status = 1;
			continue ;
		}
		status = 0;
		sep = find_equal_pos(args[i]);
		add_or_update_env(args[i], sep, env);
		i++;
	}
	return (status);
}

