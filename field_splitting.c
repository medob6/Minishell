/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_splitting.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbousset <mbousset@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:21:24 by salahian          #+#    #+#             */
/*   Updated: 2025/04/27 13:39:00 by mbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char    *applicate_field_split(char *str)
{
    char    *tmp;
    char    *new_str;
    int     j;
    int     start;

    j = 0;
    new_str = NULL;
    start = 0;
    tmp = NULL;
    if (!str[0])
        new_str = str;
    while (str && str[j])
    {
        while (str[j] && (str[j] == ' ' || str[j] == '\t' || str[j] == '\n'))
            j++;
        if (!str[j])
            break;
        start = j;
        while (str[j] && str[j] != ' ' && str[j] != '\t' && str[j] != '\n')
            j++;
        tmp = ft_substr(str, start, j - start);
        if (new_str)
            new_str = ft_strjoin(ft_strjoin(new_str, " "), tmp);
        else
            new_str = ft_strdup(tmp);
    }
    return (new_str);
}


int	check_first_char(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_');
}

int	check_the_last_arg(char *tmp)
{
	int i;

	i = (tmp[0] == '$');
	if (!check_first_char(tmp[i]))
		return (0);
	i++;
	while (tmp[i])
	{
		if (!((tmp[i] >= '0' && tmp[i] <= '9') ||
			(tmp[i] >= 'A' && tmp[i] <= 'Z') ||
			(tmp[i] >= 'a' && tmp[i] <= 'z') ||
			tmp[i] == '_'))
			break;
		i++;
	}
	return (i);
}

int skip_single_quotes(const char *s, int i)
{
	char c;

	if (ft_strchr("'", s[i]) != NULL)
	{
		c = s[i];
		i++;
		while (s[i] && s[i] != c)
			i++;
		if (s[i] == c)
			i++;
	}
	return (i);
}

int     check_for_field_split(char *tmp)
{
    int     i;
    int     dollar;
    int     qout;

    i = 0;
    qout = 0;
    dollar = 0;
    while (tmp[i])
    {
        if (tmp[i] == '\'')
            i = skip_single_quotes(tmp, i);
        if (tmp[i] == '"')
        {
            i++;
            while (tmp[i] && tmp[i] != '"')
            {
                if (tmp[i] == '$')
                    return (0);
                i++;
            }
            //i++;
        }
        if (tmp[i] == '$')
            dollar = 1;
        if (!tmp[i])
            break ;
        i++;
    }
    return (dollar);
}

// int     check_for_last_exp(t_ast_node *node)
// {
//     size_t     i;
//     int     j;
//     int     index;
//     int     arr[100];
//     char    *tmp;

//     i = 0;
//     index = 0;
//     while (i < node->children->length)
//     {
//         tmp = (char *)node->children->items[i];
//         j = 0;
//         while (tmp[j])
//         {
//             if (tmp[j] == '$')
//             {
//               arr[index] = i;
//               index++;
//               break;
//             }
//             j++;
//         }
//         i++;
//     }
//     return (arr[index]);
// }

int check_for_last_exp(t_ast_node *node)
{
    size_t i;
    int j;
    int last_index;
    char *tmp;

    last_index = -1;
    i = 0;
    if (!node->children)
        return (last_index);
    while (i < node->children->length)
    {
        tmp = (char *)node->children->items[i];
        j = 0;
        while (tmp[j])
        {
            if (tmp[j] == '$')
            {
                last_index = i;
                break;
            }
            j++;
        }
        i++;
    }
    return (last_index);
}


// int field_splitting(t_ast_node *node)
// {
//     size_t     i;
//     char    *tmp;

//     i = 0;
//     //printf("%d\n", check_for_last_exp(node));
//     tmp = (char *)node->children->items[i];
//     if ((size_t)check_the_last_arg(tmp) != ft_strlen(tmp))
//         return (0);
//     while (i < node->children->length)
//     {
//         tmp = (char *)node->children->items[i];
//         if (check_for_field_split(tmp))
//         {
//             return (1);
//             applicate_field_split(node->children, i);
//         }
//         i++;
//     }
// }