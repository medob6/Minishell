/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_the_main_list.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 14:43:01 by salahian          #+#    #+#             */
/*   Updated: 2025/04/17 14:33:49 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


t_env *create_node(char *s, int sep)
{
    t_env *node;

    node = ft_malloc(sizeof(t_env), 1);
    node->key = ft_substr(s, 0, sep);
    node->value = ft_substr(s, sep + 1, ft_strlen(s) - (sep + 1));
    node->next = NULL;
    return (node);
}

t_env *fill_the_list(char **envp)
{
    int i;
    int j;
    t_env *new_node;
    t_env *head;
    t_env *last;

    head = NULL;
    last = NULL;
    i = 0;
    while (envp[i])
    {
        j = 0;
        while (envp[i][j])
        {
            if (envp[i][j] == '=')
            {
                new_node = create_node(envp[i], j);
                if (!head)
                    head = new_node;
                else
                    last->next = new_node;
                last = new_node;
                break;
            }
            j++;
        }
        i++;
    }
    return (head);
}

t_env    *create_the_main_list(char **envp)
{
    return (fill_the_list(envp));
}

// void print_env_list(t_env *env)
// {
//     while (env)
//     {
//         printf("[key: %s], [value: %s]\n", env->key, env->value);
//         env = env->next;
//     }
// }

// int main(int argc, char **argv, char **envp)
// {
//     t_env *env_list;

//     env_list = create_the_main_list(envp);

//     //printf("Initial env list:\n");
//     //print_env_list(env_list);
//     if (argc >= 3 && strcmp(argv[1], "cd") == 0)
//     {
//         if (ft_cd(argv[2], &env_list) == 0)
//             printf("\nChanged directory to: %s\n", argv[2]);
//         else
//             printf("cd failed.\n");
//     }
//     else if (argc >= 2 && strcmp(argv[1], "pwd") == 0)
//     {
//         ft_pwd(&env_list);
//         printf("\nAfter pwd:\n");
//         //print_env_list(env_list);
//     }

//     // Final working dir
//     char cwd[1024];
//     if (getcwd(cwd, sizeof(cwd)) != NULL)
//         printf("\nCurrent working dir: %s\n", cwd);
//     else
//         perror("getcwd");

//     return 0;
// }



