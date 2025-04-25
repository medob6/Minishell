/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   removes_qouts.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:17:31 by salahian          #+#    #+#             */
/*   Updated: 2025/04/25 15:49:58 by salahian         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     take_inside_qout(char **s, char *str, int j)
{
    char    c;

    c = str[j];
    j++;
    while (str[j] && str[j] != c)
    {
        *s = append_char(*s, str[j]);
        j++;
    }
    if (str[j] == c)
        j++;
    return (j);
}

int     check_for_next_one(char *str, int j)
{
    char    c;

    c = str[j];
    j++;
    while (str[j])
    {
        if (str[j] == c)
            return (1);
        j++;
    }
    return (0);
}

void    remove_q(t_array *child, size_t i)
{
    char    *str;
    char    *new_str;
    int     j;

    str = child->items[i];
    j = 0;
    new_str = ft_strdup("");
    while (str[j])
    {
        if (str[j] == '\'') //&& check_for_next_one(str, j))
            j = take_inside_qout(&new_str, str, j);
        else if (str[j] == '"') //&& check_for_next_one(str, j))
            j = take_inside_qout(&new_str, str, j);
        else
            new_str = append_char(new_str, str[j]);
        if (str[j])
            j++;
    }
    if (new_str[0] != '\0')
        child->items[i] = new_str;
}

void    removes_qouts(t_ast_node *node)
{
    size_t  i;
    char    *tmp;

    if (!node->children)
        return ;
    i = 0;
    while (i < node->children->length)
    {
        tmp = (char *)node->children->items[i];
        if (tmp)
            remove_q(node->children, i);
        i++;
    }
}