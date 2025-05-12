/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:10:17 by salahian          #+#    #+#             */
/*   Updated: 2025/05/12 17:25:20 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

int not_valid(char c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_');
}

int check_is_okey(char *s)
{
    int i;

    if (!not_valid(s[0]) || (s[0] >= '0' && s[0] <= '9'))
        return (0);

    for (i = 0; s[i] && s[i] != '='; i++)
    {
        if (s[i] == '+' && s[i + 1] == '=') // Allow +=
            return (1);
        if (!not_valid(s[i]))
            return (0);
    }

    return (1);
}

t_env *create_new(char *s, int sep)
{
    t_env *node = ft_malloc(sizeof(t_env), 1);
    node->key = ft_substr(s, 0, sep);
    node->value = NULL; // Initialize with NULL for better handling
    node->value_set = false;

    if (sep != -1 && s[sep] == '=')
    {
        node->value = ft_substr(s, sep + 1, ft_strlen(s) - sep - 1);
        node->value_set = true;
    }

    node->next = NULL;
    return (node);
}

void add_the_new(t_env **env, t_env *new)
{
    t_env *tmp = *env;

    while (tmp && tmp->next)
        tmp = tmp->next;

    if (!tmp)
        *env = new;
    else
        tmp->next = new;
}

int find_equal_pos(char *s)
{
    int i = 0;

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

void print_lst(t_env **env, int fd)
{
    t_env *tmp = *env;

    while (tmp)
    {
        ft_print("declare -x ", fd);
        ft_print(tmp->key, fd);

        if (tmp->value_set && tmp->value)
        {
            ft_print("=\"", fd);
            ft_print(tmp->value, fd);
            ft_print("\"", fd);
        }

        ft_print("\n", fd);
        tmp = tmp->next;
    }
}


void add_or_update_env(char *arg, int sep, t_env **env)
{
    t_env *tmp = *env;
    t_env *existing = update_existing_env(tmp, arg, sep);

    if (existing)
    {
        if (sep != -1 && arg[sep] == '+' && arg[sep + 1] == '=')
        {
            char *new_value = ft_strjoin(existing->value ? existing->value : "", &arg[sep + 2]);
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
            t_env *new = create_new(arg, sep);
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

int ft_export(char **args, t_env **env, int fd)
{
    int i = 1, sep, status = 0;

    if (!args[1])
        print_lst(env, fd);

    while (args[i])
    {
        if (!check_is_okey(args[i]))
        {
            ft_print("minishell: export: not a valid identifier\n", 2);
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
