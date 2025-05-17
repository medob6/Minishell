/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:58:58 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/16 22:01:47 by mbousset         ###   ########.fr       */
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

	i = 0;
	if (!not_valid(s[0]) || (s[0] >= '0' && s[0] <= '9'))
		return (0);
	while (s[i] && s[i] != '=')
	{
		if (s[i] == '+' && s[i + 1] == '=')
			return (1);
		if (!not_valid(s[i]))
			return (0);
		i++;
	}
	return (1);
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
