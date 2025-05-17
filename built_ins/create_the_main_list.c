/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_the_main_list.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:43:01 by salahian          #+#    #+#             */
/*   Updated: 2025/05/17 16:03:57 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*create_node(char *s, int sep)
{
	t_env	*node;

	node = ft_malloc(sizeof(t_env), 1);
	node->key = ft_substr(s, 0, sep);
	node->value = ft_substr(s, sep + 1, ft_strlen(s) - (sep + 1));
	node->value_set = true;
	node->next = NULL;
	return (node);
}

void	process_env_entry(t_env **head, t_env **last, char *entry, int eq_pos)
{
	t_env	*new_node;

	new_node = create_node(entry, eq_pos);
	if (!(*head))
		*head = new_node;
	else
		(*last)->next = new_node;
	*last = new_node;
}

t_env	*fill_the_list(char **envp)
{
	int		i;
	int		j;
	t_env	*head;
	t_env	*last;

	head = NULL;
	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j])
		{
			if (envp[i][j] == '=')
			{
				process_env_entry(&head, &last, envp[i], j);
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
	shlvl_node->value_set = true;
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
