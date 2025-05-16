/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_helper2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:59:26 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/16 22:06:24 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_existing_env(t_env *existing, char *arg, int sep)
{
	char	*new_value;
	char	*current_value;

	if (sep != -1 && arg[sep] == '+' && arg[sep + 1] == '=')
	{
		current_value = "";
		if (existing->value)
			current_value = existing->value;
		new_value = ft_strjoin(current_value, &arg[sep + 2]);
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

void	handle_new_env(t_env **env, char *arg, int sep)
{
	t_env	*new;

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

void	add_or_update_env(char *arg, int sep, t_env **env)
{
	t_env	*tmp;
	t_env	*existing;

	tmp = *env;
	existing = update_existing_env(tmp, arg, sep);
	if (existing)
		handle_existing_env(existing, arg, sep);
	else
		handle_new_env(env, arg, sep);
}

void	swap_env_nodes(t_env *a, t_env *b)
{
	char	*tmp_key;
	char	*tmp_value;
	bool	tmp_value_set;

	tmp_key = a->key;
	a->key = b->key;
	b->key = tmp_key;
	tmp_value = a->value;
	a->value = b->value;
	b->value = tmp_value;
	tmp_value_set = a->value_set;
	a->value_set = b->value_set;
	b->value_set = tmp_value_set;
}
