/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   removes_qouts.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salahian <salahian@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 09:17:31 by salahian          #+#    #+#             */
/*   Updated: 2025/05/01 17:10:46 by salahian         ###   ########.fr       */
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

//int get_the_qout(char *str, char c)
//{
    
//}

void    remove_q(t_ast_node *node, size_t i, int flag)
{
    char    *str;
    char    *new_str;
    int     j;
//"*"
    if (flag)
        str = node->children->items[i];
    else
        str = ((t_token *)node->children->items[i])->value.str_value;
    j = 0;
    new_str = ft_strdup("");
    while (str[j])
    {
        if (str[j] == '\'' && check_for_next_one(str, j) && (node->field[i][j] != 5 && node->field[i][j] != 1))
            j = take_inside_qout(&new_str, str, j);
        else if (str[j] == '"' && check_for_next_one(str, j) && (node->field[i][j] != 5 && node->field[i][j] != 1))
        {
            printf("[%d]\n", node->field[i][j]);
            j = take_inside_qout(&new_str, str, j);
        }
        else
            new_str = append_char(new_str, str[j++]);
    }
    if (flag)
        node->children->items[i] = new_str;
    else
        ((t_token *)node->children->items[i])->value.str_value = new_str;
}


void    removes_qouts_cmd(t_ast_node *node)
{
    size_t  i;
    char    *tmp;

    if (!node->children)
        return ;
    i = 0;
    tmp = (char *)node->children->items[0];
    //if (ft_strncmp(tmp, "export", ft_strlen(tmp)) == 0)
    //    return ;
    while (i < node->children->length)
    {
        tmp = (char *)node->children->items[i];
        if (tmp)
        {
            remove_q(node, i, 1);
        }
        i++;
    }
}

//void    removes_qouts_red(t_ast_node *node)
//{
//    size_t  i;
//    char    *tmp;

//    if (!node->redirect_list)
//        return ;
//    i = 0;
//    while (i < node->redirect_list->length)
//    {
//        if (((t_token *)node->redirect_list->items[i])->type == TOKEN_HEREDOC)
//		{
//			i++;
//			continue;
//		}
//		tmp = ((t_token *)node->redirect_list->items[i])->value.str_value;
//        if (tmp)
//        {
//            remove_q(node->redirect_list, i, 0);
//        }
//        i++;
//    }
//}
