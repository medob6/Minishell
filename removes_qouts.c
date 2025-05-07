/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   removes_qouts.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:17:31 by salahian          #+#    #+#             */
/*   Updated: 2025/05/07 09:08:42 by salahian         ###   ########.fr       */
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

int     take_inside_qouts(char **s, char *field, char *str, int j)
{
    char    c;

    c = str[j];
    j++;
    while (str[j] && (str[j] != c && field[j] != '0'))
    {
        *s = append_char(*s, str[j]);
        j++;
    }
    if (str[j] == c && field[j] == '0')
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

//int get_the_qout(char *str, char c)
//{
    
//}

char    *remove_q(char **field, char *str, size_t i)
{
    char    *new_str;
    int     j;

    j = 0;
    new_str = ft_strdup("");
    while (str[j])
    {
        if (str[j] == '\'' && field[i][j] == '0')//&& check_for_next_one(str, j) && (field[i][j] != '5' && field[i][j] != '1'))
            j = take_inside_qouts(&new_str, field[i], str, j);
        else if (str[j] == '"' && field[i][j] == '0') //&& check_for_next_one(str, j) && (field[i][j] != '5' && field[i][j] != '1'))
            j = take_inside_qouts(&new_str, field[i], str, j);
        else
            new_str = append_char(new_str, str[j++]);
    }
    return (new_str);
}


void    removes_qouts_cmd(t_expansion *expand)
{
    size_t  i;
    char    **tmp;
    int     j;

    if (!expand->node->children)
        return ;
    i = 0;
    //if (ft_strncmp(tmp, "export", ft_strlen(tmp)) == 0)
    //    return ;
    while (i < expand->node->children->length)
    {
        tmp = ((t_str *)expand->node->children->items[i])->value;
        if (tmp && *tmp)
        {
            j = 0;
            while (tmp && tmp[j])
            {
                tmp[j] = remove_q(expand->field, tmp[j], i);
                //printf("[%s]\n", tmp[j]);
                j++;
            }
        }
        ((t_str *)expand->node->children->items[i])->value = tmp;
        i++;
    }
}

void    removes_qouts_red(t_expansion *expand)
{
    size_t  i;
    int     j;
    char    **tmp;

    if (!expand->node->redirect_list)
        return ;
    i = 0;
    while (i < expand->node->redirect_list->length)
    {
        if (((t_str *)expand->node->redirect_list->items[i])->type == TOKEN_HEREDOC)
        {
            i++;
            continue;
        }
        tmp = ((t_str *)expand->node->redirect_list->items[i])->value;
        if (tmp && *tmp)
        {
            j = 0;
            while (tmp && tmp[j])
            {
                tmp[j] = remove_q(expand->field, tmp[j], i);
                //printf("[%s]\n", tmp[j]);
                j++;
            }
        }
        ((t_str *)expand->node->redirect_list->items[i])->value = tmp;
        i++;
    }
}
