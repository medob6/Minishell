/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 17:10:17 by salahian          #+#    #+#             */
/*   Updated: 2025/04/13 18:01:30 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int not_valid(char c)
// {
//     return ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') ||
//     (c >= 'a' && c <= 'z') || c == '_');
// }
// int check_is_okey(char *s)
// {
//     int     i;

//     i = 0;
//     while (s[i])
//     {
//         if (s[i] == '=')
//             break ;
//         if (!not_valid(s[i]))
//             return (0);
//         i++;
//     }
//     return (1);
// }

// t_env    *create_new(char *s, int sep)
// {
//     t_env   *node;
    
//     node = ft_malloc(sizeof(t_env), 1);
//     node->key = ft_substr(s, 0, sep);
//     node->value = ft_substr(s, sep + 1, ft_strlen(s) - (sep + 1));
//     node->next = NULL;
//     return (node);
// }

// void    add_the_new(t_env **env, t_env *new)
// {
//     t_env   *tmp;

//     tmp = env;
//     while (tmp)
//         tmp = tmp->next;
//     tmp->next = new;
// }

// int	ft_export(char **args, t_env **env)
// {
//     int     i;
//     int     j;
//     char    *s;
//     t_env   *tmp;

//     i = 1;
//     if (!args[1])
//         return (print_env(env), 0);
//     if ((args[1][0] >= '0' && args[1][0] <= '9') || !check_is_okey(args[1]))
//     {
//         write(2, "minishell: export: not a valid identifier\n", 43);
//         return (1);
//     }
//     tmp = *env;
//     while (tmp)
//     {
//         j = 0;
//         while (args[i][j])
//         {
//             if (args[i][j] == '=')
//             {
//                 if (ft_strncmp(args[i], tmp->key, ft_strlen(tmp->key) == 0))
//                 {
//                     s = tmp->value;
//                     tmp->value = ft_strdup(&args[i][j + 1]);
//                     free(s);
//                 }
//                 else
//                     add_the_new(env, create_new(args[i], j));
//                 return (0);
//             }
//             j++;
//         }
//         tmp = tmp->next;
//     }
//     return (0);
// }

int not_valid(char c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9') || c == '_');
}

int check_is_okey(char *s)
{
    int i = 0;

    if (!not_valid(s[0]) || (s[0] >= '0' && s[0] <= '9')) // can't start with digit
        return (0);
    while (s[i] && s[i] != '=')
    {
        if (!not_valid(s[i]))
            return (0);
        i++;
    }
    return (1);
}

t_env *create_new(char *s, int sep)
{
    t_env *node;

    node = ft_malloc(sizeof(t_env), 1);
    node->key = ft_substr(s, 0, sep);
    node->value = (s[sep] == '=') ? ft_substr(s, sep + 1, ft_strlen(s) - sep - 1) : NULL;
    node->next = NULL;
    return (node);
}

void add_the_new(t_env **env, t_env *new)
{
    t_env *tmp;

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

int ft_export(char **args, t_env **env)
{
    int i = 1;
    int sep;
    t_env *tmp;

    while (args[i])
    {
        if (!check_is_okey(args[i]))
        {
            write(2, "minishell: export: not a valid identifier\n", 43);
            i++;
            continue;
        }

        sep = 0;
        while (args[i][sep] && args[i][sep] != '=')
            sep++;

        tmp = *env;
        while (tmp)
        {
            if (ft_strncmp(args[i], tmp->key, sep) == 0 && tmp->key[sep] == '\0')
            {
                if (args[i][sep] == '=')
                {
                    free(tmp->value);
                    tmp->value = ft_strdup(&args[i][sep + 1]);
                }
                break;
            }
            tmp = tmp->next;
        }
        if (!tmp)
            add_the_new(env, create_new(args[i], sep));
        i++;
    }
    return (0);
}
