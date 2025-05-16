/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:59:14 by mbousset          #+#    #+#             */
/*   Updated: 2025/05/16 20:26:19 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution.h"

char	**extract_envp(t_env *env)
{
	t_env	*head;
	char	*tmp;
	char	**envp;
	int		i;

	head = env;
	envp = ft_malloc(sizeof(char **), lst_size(env) + 1);
	if (!env)
		return (NULL);
	i = 0;
	while (head)
	{
		tmp = ft_strjoin(head->key, "=");
		envp[i++] = ft_strjoin(tmp, head->value);
		ft_free(tmp);
		head = head->next;
	}
	envp[i] = NULL;
	return (envp);
}

bool	cmd_built_in(char *cmd)
{
	if (!ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "echo") || !ft_strcmp(cmd,
			"cd") || !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit"))
		return (true);
	return (false);
}

char	**extract_cmd_args(t_str **strs)
{
	int		total_len;
	char	**args;
	int		j;
	int		i;
	int		k;

	i = 0;
	k = 0;
	total_len = get_length(strs);
	args = (char **)ft_malloc((total_len + 1), sizeof(char *));
	while (strs[i])
	{
		j = 0;
		while (strs[i]->value && strs[i]->value[j])
		{
			args[k++] = strs[i]->value[j++];
		}
		i++;
	}
	args[k] = NULL;
	return (args);
}

bool	build_cmd(t_cmd *cmd, t_ast_node *node, t_env *env)
{
	char	**cmd_args;

	if (!init_cmd_structure(cmd, node))
		return (false);
	cmd_args = NULL;
	if (node->children)
	{
		node->children->length = get_length((t_str **)node->children->items);
		cmd_args = extract_cmd_args((t_str **)node->children->items);
	}
	if (!set_cmd_args_and_path(cmd, cmd_args, env))
		return (false);
	if (cmd_built_in(cmd_args[0]))
		cmd->is_built_in = 1;
	return (true);
}

t_cmd	*build_cmd_list(int cmd_nbr, t_ast_node **cmd_node, t_env *envp)
{
	t_cmd	*cmd_lst;
	int		i;

	cmd_lst = ft_calloc(cmd_nbr, sizeof(t_cmd));
	i = 0;
	while (i < cmd_nbr)
	{
		if (cmd_node[i]->type == AST_SIMPLE_CMD)
			build_cmd(&cmd_lst[i], cmd_node[i], envp);
		else if (cmd_node[i]->type == AST_SUBSHELL)
		{
			cmd_lst[i].is_subshell = true;
			cmd_lst[i].subshell_node = cmd_node[i];
		}
		i++;
	}
	return (cmd_lst);
}
