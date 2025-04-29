/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:10:17 by salahian          #+#    #+#             */
/*   Updated: 2025/04/29 11:46:27 by salahian         ###   ########.fr       */
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
		node->value = ft_substr(s, sep + 1, ft_strlen(s) - sep - 1);
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

// int ft_export(char **args, t_env **env)
// {
//     int i;
//     int sep;
//     t_env *tmp;

//     i = 1;
//     while (args[i])
//     {
//         if (!check_is_okey(args[i]))
//         {
//             ft_print("minishell: export: not a valid identifier\n", 2);
//             i++;
//             continue ;
//         }
//         sep = 0;
//         while (args[i][sep] && args[i][sep] != '=')
//             sep++;

//         tmp = *env;
//         while (tmp)
//         {
//             if (ft_strncmp(args[i], tmp->key, sep) == 0
//	&& tmp->key[sep] == '\0')
//             {
//                 if (args[i][sep] == '=')
//                     tmp->value = ft_strdup(&args[i][sep + 1]);
//                 break ;
//             }
//             tmp = tmp->next;
//         }
//         if (!tmp && args[i][sep] == '=')
//             add_the_new(env, create_new(args[i], sep));

//         i++;
//     }
//     return (0);
// }

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
				free(env->value);
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

// // ? testing function

// static void	print_lst(t_env **env)
// {
// 	t_env	*head;

// 	head = *env;
// 	while (head)
// 	{
// 		if (!strcmp(head->key, "var"))
// 		{
// 			printf("\033[0;32mvalue added succesfully \033[0m\n");
// 			printf("key = %s , value = %s \n", head->key, head->value);
// 		}
// 		// else
// 		// 	printf("key = %s , value = %s \n", head->key, head->value);
// 		head = head->next;
// 	}
// 	if (head)
// 		printf("\033[0;32mkey was not added\033[0m\n");
// 	return ;
// }

void	print_lst(t_env **env)
{
	t_env	*tmp;

	tmp = *env;
	while (tmp)
	{
		ft_print("declare -x ", 1);
		ft_print(tmp->key, 1);
		ft_print("=", 1);
		ft_print(tmp->value, 1);
		ft_print("\n", 1);
		tmp = tmp->next;
	}
}

int	ft_export(char **args, t_env **env)
{
	int	i;
	int	sep;

	i = 1;
	 if (!args[1])
	 	print_lst(env);
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_print("minishell: export: not a valid identifier\n", 2);
			i++;
			continue ;
		}
		sep = find_equal_pos(args[i]);
		add_or_update_env(args[i], sep, env);
		i++;
	}
	
	return (0);
}
